/*
 * midi_editor.c
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <stdint.h>
 #include <string.h>
 #include <math.h> // For log2, round, fmin, fmax
 #include <ctype.h> // For tolower, isupper
 
// I swear if this is a stack overflow im going to crash out idgf anymore

 #define MAX_TRACKS 64         // Maximum number of tracks
 #define MAX_EVENTS_PER_TRACK 8192 // Maximum events per track
 #define MAX_COMMANDS 256
 #define MAX_LINE 256          // Increased for potentially longer command lines
 #define MAX_FILENAME 256
 
 // --- Data Structures ---
 
 typedef struct {
     uint32_t tick;
     uint8_t status; // Full status byte, including channel for channel messages
     uint8_t data1;
     uint8_t data2;
     uint8_t* extra;
     uint32_t extra_len;
     // Internal flag for processing, e.g., marking for deletion
     // Does not get written to MIDI file.
     uint8_t internal_flags; // 0x01: marked for deletion
 } MidiEvent;
 
 typedef struct {
     MidiEvent events[MAX_EVENTS_PER_TRACK];
     int nevents;
     uint8_t   track_channel_found; // Stores the channel (0-15) found/used in this track. 0xFF if not determined.
     // uint32_t track_chunk_len_read; // Original length from MTrk header (optional)
 } MidiTrack;
 
 typedef struct {
     uint16_t format;
     uint16_t ntrks_from_header; // Number of tracks as read from MThd
     uint16_t ppq;
     MidiTrack tracks[MAX_TRACKS];
     int actual_ntrks; // Number of tracks actually stored/processed
 } MidiFile;
 
 typedef enum {
     CMD_CHANGE_BPM,
     CMD_CHANGE_PROG,
     CMD_INC_PITCH,
     CMD_REVERSE,
     CMD_ADD_TRACK,
     CMD_DELETE_TRACK,
     CMD_ADD_NOTE,
     CMD_REM_NOTE,
     CMD_TRANSPOSE,
     CMD_CHANGE_TS,
     CMD_INC_VEL,
     CMD_UNKNOWN
 } CmdType;
 
 typedef struct {
     CmdType type;
     int args[5]; // For integer arguments like track, instrument, pitch_value, velocity
     double dbl_args[3]; // For double arguments like time, duration
     char str_args[2][32]; // For string arguments like pitch_name, tonality
 } Command;
 
 
 // --- Helper Function Prototypes ---
 void free_midi_event_extra(MidiEvent* ev);
 void cleanup_midi_track(MidiTrack* track);
 void cleanup_midi_file(MidiFile* mf);
 uint8_t get_or_assign_channel_for_track(MidiTrack* track, int track_idx_0_based);
 
 
 // --- MIDI File I/O Helpers (Big Endian & VLQ) ---
 
 uint32_t read_be32(FILE* f) {
     uint8_t b[4];
     if (fread(b, 1, 4, f) != 4) { /*fprintf(stderr, "Warning: Short read for be32\n");*/ return 0; }
     return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
 }
 
 uint16_t read_be16(FILE* f) {
     uint8_t b[2];
     if (fread(b, 1, 2, f) != 2) { /*fprintf(stderr, "Warning: Short read for be16\n");*/ return 0; }
     return ((uint16_t)b[0] << 8) | (uint16_t)b[1];
 }
 
 void write_be32(FILE* f, uint32_t v) {
     uint8_t b[4] = { (uint8_t)(v >> 24), (uint8_t)(v >> 16), (uint8_t)(v >> 8), (uint8_t)v };
     fwrite(b, 1, 4, f);
 }
 
 void write_be16(FILE* f, uint16_t v) {
     uint8_t b[2] = { (uint8_t)(v >> 8), (uint8_t)v };
     fwrite(b, 1, 2, f);
 }
 
 uint32_t read_vlq(FILE* f) {
     uint32_t value = 0;
     uint8_t byte_in;
     int count = 0;
     do {
         if (fread(&byte_in, 1, 1, f) != 1) {
             // fprintf(stderr, "Warning: Failed to read byte for VLQ.\n");
             return value; // Return what we have, might be incomplete
         }
         value = (value << 7) | (byte_in & 0x7F);
         count++;
         if (count > 5) { /*fprintf(stderr, "Warning: VLQ too long.\n");*/ return value; } // Protect against malformed VLQ
     } while (byte_in & 0x80);
     return value;
 }
 
 void write_vlq(FILE* f, uint32_t v) {
     uint8_t buffer[5];
     int n = 0;
     if (v == 0) {
         fputc(0x00, f);
         return;
     }
     do {
         buffer[n++] = v & 0x7F;
         v >>= 7;
     } while (v > 0 && n < 5);
 
     for (int i = n - 1; i >= 0; i--) {
         uint8_t out_byte = buffer[i];
         if (i != 0) { // If not the last byte (MSB in VLQ encoding terms)
             out_byte |= 0x80;
         }
         fputc(out_byte, f);
     }
 }
 
 // --- Time and Pitch Conversions ---
 
 double beats_to_ticks(double beats, uint16_t ppq) {
     return round(beats * ppq);
 }
 
 double ticks_to_beats(uint32_t ticks, uint16_t ppq) {
     return (double)ticks / ppq;
 }
 
 int pitch_to_midi(const char* s) {
     if (!s || strlen(s) < 2) return -1; // Basic validation
 
     int base = 0;
     char note_char = toupper(s[0]);
     int current_idx = 1;
 
     switch (note_char) {
         case 'C': base = 0; break;
         case 'D': base = 2; break;
         case 'E': base = 4; break;
         case 'F': base = 5; break;
         case 'G': base = 7; break;
         case 'A': base = 9; break;
         case 'B': base = 11; break;
         default: return -1; // Invalid note name
     }
 
     if (s[current_idx] == '#') {
         base++;
         current_idx++;
     } else if (s[current_idx] == 'B' || s[current_idx] == 'b') { // Handle 'b' for flat, though assignment said only natural/sharps
         // For simplicity, let's follow the "natural plus sharps" rule and ignore flats or treat them as error
         // If flats were allowed: base--; current_idx++;
         // For now, if it's not '#', assume it's start of octave
     }
     
     if (base < 0) base += 12; // Wrap around for flats if they were handled
     base %= 12;
 
 
     if (s[current_idx] == '\0') return -1; // Missing octave
     int octave = atoi(s + current_idx);
     
     return base + (octave + 1) * 12; // MIDI octave -1 to 9 maps to C0 (12) to C9 (120)
                                      // C4 is middle C, MIDI note 60. (4+1)*12 + 0 = 60.
 }
 
 
 // --- MIDI Loading and Saving ---
 
 int load_midi(const char* fname, MidiFile* mf) {
     FILE* f = fopen(fname, "rb");
     if (!f) {
         fprintf(stderr, "Error: Cannot open MIDI file '%s'\n", fname);
         return -1;
     }
 
     // Read MThd Header Chunk
     if (read_be32(f) != 0x4D546864) { // "MThd"
         fprintf(stderr, "Error: Not a valid MIDI file (MThd chunk ID missing).\n");
         fclose(f);
         return -1;
     }
     uint32_t header_len = read_be32(f);
     if (header_len < 6) {
         fprintf(stderr, "Error: MThd header too short.\n");
         fclose(f);
         return -1;
     }
 
     mf->format = read_be16(f);
     mf->ntrks_from_header = read_be16(f);
     mf->ppq = read_be16(f);
 
     // Skip any extra MThd data
     if (header_len > 6) {
         fseek(f, header_len - 6, SEEK_CUR);
     }
 
     if (mf->format > 1 && mf->format != 0) { // Supporting format 0 and 1. Format 2 is rare and more complex.
          fprintf(stderr, "Warning: MIDI format %d may not be fully supported (best effort for 0 or 1).\n", mf->format);
     }
     if (mf->ntrks_from_header == 0) {
         fprintf(stderr, "Warning: MIDI file has 0 tracks in header.\n");
         // fclose(f); return -1; // Or allow, maybe commands will add tracks
     }
 
 
     mf->actual_ntrks = 0;
     uint8_t last_status_byte_for_track[MAX_TRACKS] = {0}; // For running status, per track
 
     for (int i = 0; i < mf->ntrks_from_header && mf->actual_ntrks < MAX_TRACKS; ++i) {
         if (mf->actual_ntrks >= MAX_TRACKS) {
             fprintf(stderr, "Warning: Maximum track limit (%d) reached. Subsequent tracks ignored.\n", MAX_TRACKS);
             break;
         }
         
         MidiTrack* current_track = &mf->tracks[mf->actual_ntrks];
         current_track->nevents = 0;
         current_track->track_channel_found = 0xFF; // Not found yet
 
         if (read_be32(f) != 0x4D54726B) { // "MTrk"
             // It's possible to have other chunk types between MThd and MTrk or between MTrk chunks.
             // A robust parser would read the chunk ID, read its length, and skip if not "MTrk".
             // For this assignment, assume MTrk chunks follow.
             // If we are here, it means we expected an MTrk but didn't find it.
             // This could be end of file if ntrks_from_header was too high, or a malformed file.
             long current_pos = ftell(f);
             fseek(f, 0, SEEK_END);
             long end_pos = ftell(f);
             fseek(f, current_pos, SEEK_SET);
             if (current_pos >= end_pos -8) { // Heuristic: if very close to EOF
                  //fprintf(stderr, "Warning: Expected MTrk chunk for track %d, but found end of file or insufficient data.\n", i + 1);
                  break; // Stop trying to read more tracks
             }
             // Try to find next MTrk
             char chunk_id[5] = {0};
             uint32_t unknown_chunk_len = 0;
             int recovery_attempts = 0;
             while(recovery_attempts < 5) { // Try a few times to find MTrk
                 if(fread(chunk_id, 1, 4, f) != 4) break;
                 unknown_chunk_len = read_be32(f);
                 if (strncmp(chunk_id, "MTrk", 4) == 0) {
                     fseek(f, -8, SEEK_CUR); // Rewind to read "MTrk" and its length again
                     if (read_be32(f) != 0x4D54726B) break; // Should not happen
                     // fprintf(stderr, "Info: Skipped unknown chunk to find MTrk for track %d.\n", i + 1);
                     goto mtrk_found;
                 }
                 if(fseek(f, unknown_chunk_len, SEEK_CUR) != 0) break; // Error seeking
                 recovery_attempts++;
             }
             fprintf(stderr, "Error: Expected MTrk chunk for track %d, but found '%s' or other issue.\n", i + 1, chunk_id);
             // Depending on strictness, either break or try to skip. For now, break.
             break; 
         }
 mtrk_found:;
         uint32_t track_len = read_be32(f);
         long track_start_pos = ftell(f);
         long track_end_pos = track_start_pos + track_len;
         uint32_t current_tick_abs = 0;
 
         while (ftell(f) < track_end_pos && current_track->nevents < MAX_EVENTS_PER_TRACK) {
             MidiEvent* ev = &current_track->events[current_track->nevents];
             ev->extra = NULL;
             ev->extra_len = 0;
             ev->internal_flags = 0;
 
             uint32_t delta_time = read_vlq(f);
             current_tick_abs += delta_time;
             ev->tick = current_tick_abs;
 
             uint8_t status_byte_candidate;
             if (fread(&status_byte_candidate, 1, 1, f) != 1) break; // End of track data or error
 
             if (status_byte_candidate & 0x80) { // It's a new status byte
                 ev->status = status_byte_candidate;
                 last_status_byte_for_track[mf->actual_ntrks] = ev->status;
             } else { // Running status
                 ev->status = last_status_byte_for_track[mf->actual_ntrks];
                 fseek(f, -1, SEEK_CUR); // Rewind, this byte is data1
             }
             
             // Determine channel for track if not yet found
             if (current_track->track_channel_found == 0xFF && (ev->status & 0xF0) != 0xF0) {
                 current_track->track_channel_found = ev->status & 0x0F;
             }
 
 
             // Handle event data based on status
             // System Common Messages (0xF0-0xF7) and System Real-Time Messages (0xF8-0xFE)
             if (ev->status >= 0xF0 && ev->status <= 0xF7) { // System Common
                 if (ev->status == 0xF0 || ev->status == 0xF7) { // Sysex start or end
                     ev->extra_len = read_vlq(f);
                     ev->extra = (uint8_t*)malloc(ev->extra_len);
                     if (!ev->extra) { /*fprintf(stderr, "Malloc failed for sysex\n");*/ fclose(f); return -1;}
                     if (fread(ev->extra, 1, ev->extra_len, f) != ev->extra_len) { /* Handle error */ }
                 } else { // Other Fx messages might have 1 or 2 data bytes, or none.
                          // This simplified parser assumes they don't for non-sysex.
                          // For robust parsing, each Fx needs specific handling.
                          // For this assignment, meta events (0xFF) are more important.
                 }
             } else if (ev->status == 0xFF) { // Meta Event
                 if (fread(&ev->data1, 1, 1, f) != 1) break; // Meta event type
                 ev->extra_len = read_vlq(f);
                 if (ev->extra_len > 0) {
                     ev->extra = (uint8_t*)malloc(ev->extra_len);
                     if (!ev->extra) { /*fprintf(stderr, "Malloc failed for meta\n");*/ fclose(f); return -1;}
                     if (fread(ev->extra, 1, ev->extra_len, f) != ev->extra_len) {
                         // fprintf(stderr, "Short read for meta event data\n");
                         free(ev->extra); ev->extra = NULL; ev->extra_len = 0;
                         // continue or break, depends on how strict
                     }
                 }
                 if (ev->data1 == 0x2F) { // End of Track meta event
                      current_track->nevents++; // Add the EOT event
                      goto next_track_early; // Correctly go to next track after EOT
                 }
             } else { // Channel Voice or Mode Messages (0x8n to 0xEn)
                 uint8_t msg_type = ev->status & 0xF0;
                 if (msg_type == 0xC0 || msg_type == 0xD0) { // Program Change, Channel Pressure (1 data byte)
                     if (fread(&ev->data1, 1, 1, f) != 1) break;
                     ev->data2 = 0; // Not used
                 } else { // Note Off, Note On, Poly Pressure, Control Change, Pitch Bend (2 data bytes)
                     if (fread(&ev->data1, 1, 1, f) != 1) break;
                     if (fread(&ev->data2, 1, 1, f) != 1) break;
                 }
             }
             current_track->nevents++;
         }
 next_track_early:;
         // Ensure ftell is at track_end_pos, or report discrepancy
         long current_fpos = ftell(f);
         if (current_fpos < track_end_pos) {
             // fprintf(stderr, "Warning: Track %d not fully read, %ld bytes remaining. Skipping.\n", mf->actual_ntrks + 1, track_end_pos - current_fpos);
             fseek(f, track_end_pos, SEEK_SET);
         } else if (current_fpos > track_end_pos) {
             // This indicates an error in parsing, VLQ, or track length
             // fprintf(stderr, "Error: Read past end of track %d data. File might be corrupt.\n", mf->actual_ntrks + 1);
             // For robustness, might try to seek to where next track *should* have been.
             // But for now, this is a sign of trouble.
         }
 
 
         mf->actual_ntrks++;
     }
 
     fclose(f);
     return 0;
 }
 
 int cmp_evt(const void* a, const void* b) {
     MidiEvent* ev_a = (MidiEvent*)a;
     MidiEvent* ev_b = (MidiEvent*)b;
     if (ev_a->tick != ev_b->tick) {
         return ev_a->tick - ev_b->tick;
     }
     // Optional: secondary sort criteria for events at the same tick
     // E.g., Note Off before Note On, Control Changes before Notes, etc.
     // For simplicity, not implemented here, but crucial for complex MIDI.
     // Meta events like tempo/key sig often come first at a tick.
     if (ev_a->status == 0xFF && ev_b->status != 0xFF) return -1; // Meta events first
     if (ev_a->status != 0xFF && ev_b->status == 0xFF) return 1;
     if ((ev_a->status & 0xF0) == 0x80 && (ev_b->status & 0xF0) == 0x90) return -1; // Note Off before Note On
     if ((ev_a->status & 0xF0) == 0x90 && (ev_b->status & 0xF0) == 0x80) return 1;
 
     return ev_a->status - ev_b->status; // Fallback
 }
 
 
 int write_midi(const char* fname, MidiFile* mf) {
     FILE* f = fopen(fname, "wb");
     if (!f) {
         fprintf(stderr, "Error: Cannot open MIDI file '%s' for writing.\n", fname);
         return -1;
     }
 
     // Write MThd Header
     write_be32(f, 0x4D546864); // "MThd"
     write_be32(f, 6);          // Header length
     write_be16(f, mf->format);
     write_be16(f, mf->actual_ntrks); // Use actual number of tracks being written
     write_be16(f, mf->ppq);
 
     for (int i = 0; i < mf->actual_ntrks; ++i) {
         MidiTrack* current_track = &mf->tracks[i];
         
         // Sort events in track by tick
         qsort(current_track->events, current_track->nevents, sizeof(MidiEvent), cmp_evt);
 
         // Write MTrk Chunk Header (ID and placeholder for length)
         write_be32(f, 0x4D54726B); // "MTrk"
         long track_len_pos = ftell(f);
         write_be32(f, 0); // Placeholder for track length
 
         long track_data_start_pos = ftell(f);
         uint32_t last_tick_in_track = 0;
         int has_eot = 0;
 
         for (int j = 0; j < current_track->nevents; ++j) {
             MidiEvent* ev = &current_track->events[j];
             if (ev->internal_flags & 0x01) continue; // Skip marked for deletion
 
             write_vlq(f, ev->tick - last_tick_in_track);
             last_tick_in_track = ev->tick;
 
             fwrite(&ev->status, 1, 1, f);
             if (ev->status != 0xFF && (ev->status & 0x80) == 0) { // Running status was used in load, but we write full status
                 // This should not happen if ev->status is always full. If ev->status could be data1 of running status:
                 // then we'd need the *actual* previous status byte.
                 // Assuming ev->status is always a full status byte here.
             }
 
 
             if (ev->status == 0xFF) { // Meta Event
                 fwrite(&ev->data1, 1, 1, f); // Meta type
                 write_vlq(f, ev->extra_len);
                 if (ev->extra_len > 0 && ev->extra) {
                     fwrite(ev->extra, 1, ev->extra_len, f);
                 }
                 if (ev->data1 == 0x2F) has_eot = 1;
             } else if (ev->status >= 0xF0 && ev->status <= 0xF7) { // System Common
                  if (ev->status == 0xF0 || ev->status == 0xF7) { // Sysex
                     write_vlq(f, ev->extra_len);
                     if (ev->extra_len > 0 && ev->extra) {
                        fwrite(ev->extra, 1, ev->extra_len, f);
                     }
                  } // Other Fx messages: data bytes would be written if they existed and parser handled them
             } else { // Channel Voice Messages
                 uint8_t msg_type = ev->status & 0xF0;
                 if (msg_type == 0xC0 || msg_type == 0xD0) { // 1 data byte
                     fwrite(&ev->data1, 1, 1, f);
                 } else { // 2 data bytes
                     fwrite(&ev->data1, 1, 1, f);
                     fwrite(&ev->data2, 1, 1, f);
                 }
             }
         }
 
         // Add End Of Track (EOT) meta-event if not present
         if (!has_eot) {
             write_vlq(f, 0); // Delta time for EOT (usually 0 or small if last event was earlier)
             fputc(0xFF, f);  // Meta event status
             fputc(0x2F, f);  // EOT type
             fputc(0x00, f);  // EOT length
         }
 
         // Calculate and write track length
         long track_data_end_pos = ftell(f);
         uint32_t track_chunk_len = track_data_end_pos - track_data_start_pos;
         fseek(f, track_len_pos, SEEK_SET);
         write_be32(f, track_chunk_len);
         fseek(f, track_data_end_pos, SEEK_SET); // Go to end of written track
     }
 
     fclose(f);
     return 0;
 }
 
 // --- Command Parsing ---
 
 CmdType parse_cmd_name(const char* w) {
     if (strcmp(w, "changeBPM") == 0) return CMD_CHANGE_BPM;
     if (strcmp(w, "changeTrackInstrument") == 0) return CMD_CHANGE_PROG;
     if (strcmp(w, "increasePitch") == 0) return CMD_INC_PITCH;
     if (strcmp(w, "reverse") == 0) return CMD_REVERSE;
     if (strcmp(w, "addTrack") == 0) return CMD_ADD_TRACK;
     if (strcmp(w, "deleteTrack") == 0) return CMD_DELETE_TRACK;
     if (strcmp(w, "addNote") == 0) return CMD_ADD_NOTE;
     if (strcmp(w, "removeNote") == 0) return CMD_REM_NOTE;
     if (strcmp(w, "transposition") == 0) return CMD_TRANSPOSE;
     if (strcmp(w, "changeTimeSignature") == 0) return CMD_CHANGE_TS;
     if (strcmp(w, "increaseVelocity") == 0) return CMD_INC_VEL;
     return CMD_UNKNOWN;
 }
 
 int load_commands(const char* fname, Command cmds[], int* ncmds) {
     FILE* f = fopen(fname, "r");
     if (!f) {
         fprintf(stderr, "Error: Cannot open commands file '%s'\n", fname);
         return -1;
     }
 
     char line[MAX_LINE];
     *ncmds = 0;
     int line_num = 0;
 
     while (fgets(line, MAX_LINE, f) && *ncmds < MAX_COMMANDS) {
         line_num++;
         line[strcspn(line, "\r\n")] = 0; // Remove newline
 
         char* tokens[10]; // Max tokens expected for any command
         int ntokens = 0;
         char* p = strtok(line, " \t");
         while (p && ntokens < 10) {
             tokens[ntokens++] = p;
             p = strtok(NULL, " \t");
         }
 
         if (ntokens == 0) continue; // Skip empty lines
 
         Command* cmd = &cmds[*ncmds];
         cmd->type = parse_cmd_name(tokens[0]);
 
         // Basic validation for number of tokens
         // args[0] usually track, args[1] instrument/pitch_val/velocity_val
         // dbl_args[0] startTime/timestamp, dbl_args[1] endTime/duration
         // str_args[0] pitch_name/old_tonality/time_sig, str_args[1] new_tonality
 
         int expected_tokens = 0;
         switch (cmd->type) {
             case CMD_CHANGE_BPM: // changeBPM <new_bpm>
                 expected_tokens = 2;
                 if (ntokens == expected_tokens) cmd->args[0] = atoi(tokens[1]);
                 break;
             case CMD_CHANGE_PROG: // changeTrackInstrument <track> <new_instrument>
                 expected_tokens = 3;
                 if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->args[1] = atoi(tokens[2]); // new_instrument
                 }
                 break;
             case CMD_INC_PITCH: // increasePitch <track> <startTime> <endTime> <increase_value>
                 expected_tokens = 5;
                 if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->dbl_args[0] = atof(tokens[2]); // startTime
                     cmd->dbl_args[1] = atof(tokens[3]); // endTime
                     cmd->args[1] = atoi(tokens[4]); // increase_value
                 }
                 break;
             case CMD_REVERSE: // reverse <track> <startTime> <endTime>
                 expected_tokens = 4;
                 if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->dbl_args[0] = atof(tokens[2]); // startTime
                     cmd->dbl_args[1] = atof(tokens[3]); // endTime
                 }
                 break;
             case CMD_ADD_TRACK: // addTrack <instrument>
                 expected_tokens = 2;
                 if (ntokens == expected_tokens) cmd->args[0] = atoi(tokens[1]); // instrument
                 break;
             case CMD_DELETE_TRACK: // deleteTrack <track>
                 expected_tokens = 2;
                 if (ntokens == expected_tokens) cmd->args[0] = atoi(tokens[1]); // track
                 break;
             case CMD_ADD_NOTE: // addNote <track> <timestamp> <pitch> <duration> <velocity>
                 expected_tokens = 6;
                 if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->dbl_args[0] = atof(tokens[2]); // timestamp
                     strncpy(cmd->str_args[0], tokens[3], sizeof(cmd->str_args[0])-1);
                     cmd->str_args[0][sizeof(cmd->str_args[0])-1] = '\0'; // pitch
                     cmd->dbl_args[1] = atof(tokens[4]); // duration (node value)
                     cmd->args[1] = atoi(tokens[5]); // velocity
                 }
                 break;
             case CMD_REM_NOTE: // removeNote <track> <timestamp> <pitch>
                 expected_tokens = 4;
                 if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->dbl_args[0] = atof(tokens[2]); // timestamp
                     strncpy(cmd->str_args[0], tokens[3], sizeof(cmd->str_args[0])-1);
                      cmd->str_args[0][sizeof(cmd->str_args[0])-1] = '\0'; // pitch
                 }
                 break;
             case CMD_TRANSPOSE: // transposition <oldTonality> <newTonality>
                 expected_tokens = 3;
                 if (ntokens == expected_tokens) {
                     strncpy(cmd->str_args[0], tokens[1], sizeof(cmd->str_args[0])-1);
                     cmd->str_args[0][sizeof(cmd->str_args[0])-1] = '\0';
                     strncpy(cmd->str_args[1], tokens[2], sizeof(cmd->str_args[1])-1);
                     cmd->str_args[1][sizeof(cmd->str_args[1])-1] = '\0';
                 }
                 break;
             case CMD_CHANGE_TS: // changeTimeSignature <newTimeSignature> e.g. "4/4"
                 expected_tokens = 2;
                 if (ntokens == expected_tokens) {
                      strncpy(cmd->str_args[0], tokens[1], sizeof(cmd->str_args[0])-1);
                      cmd->str_args[0][sizeof(cmd->str_args[0])-1] = '\0';
                 }
                 break;
             case CMD_INC_VEL: // increaseVelocity <track> <startTime> <endTime> <increaseValue>
                 expected_tokens = 5;
                  if (ntokens == expected_tokens) {
                     cmd->args[0] = atoi(tokens[1]); // track
                     cmd->dbl_args[0] = atof(tokens[2]); // startTime
                     cmd->dbl_args[1] = atof(tokens[3]); // endTime
                     cmd->args[1] = atoi(tokens[4]); // increaseValue
                 }
                 break;
             case CMD_UNKNOWN:
                 fprintf(stderr, "Warning: Unknown command '%s' on line %d of commands file.\n", tokens[0], line_num);
                 continue; // Don't increment ncmds
         }
         if (cmd->type != CMD_UNKNOWN && ntokens != expected_tokens) {
             fprintf(stderr, "Warning: Incorrect number of arguments for command '%s' on line %d. Expected %d, got %d. Skipping command.\n", tokens[0], line_num, expected_tokens, ntokens);
             continue; // Don't increment ncmds
         }
 
         (*ncmds)++;
     }
 
     fclose(f);
     return 0;
 }
 
 // --- Command Execution ---
 
 // Helper to compact events in a track after marking some for deletion
 void compact_track_events(MidiTrack* track) {
     int write_idx = 0;
     for (int read_idx = 0; read_idx < track->nevents; ++read_idx) {
         if (!(track->events[read_idx].internal_flags & 0x01)) { // If not marked for deletion
             if (write_idx != read_idx) {
                 track->events[write_idx] = track->events[read_idx];
             }
             write_idx++;
         } else {
             // If marked for deletion, ensure its extra data is freed
             free_midi_event_extra(&track->events[read_idx]);
         }
     }
     track->nevents = write_idx;
 }
 
 
 uint8_t get_or_assign_channel_for_track(MidiTrack* track, int track_idx_0_based) {
     if (track->track_channel_found != 0xFF) {
         return track->track_channel_found;
     }
     // Try to find from existing events
     for (int i = 0; i < track->nevents; ++i) {
         uint8_t status_type = track->events[i].status & 0xF0;
         if (status_type >= 0x80 && status_type <= 0xE0) {
             track->track_channel_found = track->events[i].status & 0x0F;
             return track->track_channel_found;
         }
     }
     // If no channel message found, assign one based on track index
     track->track_channel_found = track_idx_0_based % 16;
     return track->track_channel_found;
 }
 
 
 void apply_commands(MidiFile* mf, Command cmds[], int ncmds) {
     for (int i = 0; i < ncmds; ++i) {
         Command* cmd = &cmds[i];
         int track_num_1_based;
         int target_track_idx = -1; // 0-based
         MidiTrack* target_track_ptr = NULL;
 
         // Validate track_num for commands that use it
         if (cmd->type != CMD_CHANGE_BPM && cmd->type != CMD_ADD_TRACK &&
             cmd->type != CMD_TRANSPOSE && cmd->type != CMD_CHANGE_TS) {
             track_num_1_based = cmd->args[0];
             if (track_num_1_based <= 0 || track_num_1_based > mf->actual_ntrks) {
                 fprintf(stderr, "Warning: Invalid track number %d for command type %d (max tracks %d). Skipping command.\n",
                         track_num_1_based, cmd->type, mf->actual_ntrks);
                 continue;
             }
             target_track_idx = track_num_1_based - 1;
             target_track_ptr = &mf->tracks[target_track_idx];
         }
 
         switch (cmd->type) {
             case CMD_CHANGE_BPM: {
                 int new_bpm = cmd->args[0];
                 if (new_bpm <= 0) {
                     fprintf(stderr, "Warning: Invalid BPM value %d. Skipping changeBPM.\n", new_bpm);
                     continue;
                 }
                 MidiTrack* tempo_track = &mf->tracks[0]; // Assume tempo map is track 0
                 if (mf->actual_ntrks == 0) { // Handle case where no tracks exist yet
                      // This scenario needs addTrack to be called first ideally.
                      // Or, if this is the first command, we can create a track 0.
                     if (mf->actual_ntrks < MAX_TRACKS) {
                         mf->tracks[0].nevents = 0;
                         mf->tracks[0].track_channel_found = 0xFF; // Tempo track usually no channel messages
                         mf->actual_ntrks = 1;
                         tempo_track = &mf->tracks[0];
                         // printf("Info: Created track 1 for tempo map due to changeBPM on empty MIDI.\n");
                     } else {
                         fprintf(stderr, "Warning: Cannot add tempo, max tracks reached and no tracks exist.\n");
                         continue;
                     }
                 }
 
 
                 // Remove old tempo events from tempo_track
                 for (int j = 0; j < tempo_track->nevents; ++j) {
                     if (tempo_track->events[j].status == 0xFF && tempo_track->events[j].data1 == 0x51) {
                         tempo_track->events[j].internal_flags |= 0x01; // Mark for deletion
                     }
                 }
                 compact_track_events(tempo_track);
 
                 // Insert new tempo event
                 if (tempo_track->nevents < MAX_EVENTS_PER_TRACK) {
                     uint32_t mpqn = 60000000 / new_bpm; // Microseconds per quarter note
                     MidiEvent tempo_ev = {0};
                     tempo_ev.tick = 0;
                     tempo_ev.status = 0xFF; // Meta event
                     tempo_ev.data1 = 0x51;  // Tempo event type
                     tempo_ev.extra_len = 3;
                     tempo_ev.extra = (uint8_t*)malloc(3);
                     if (!tempo_ev.extra) {fprintf(stderr,"Malloc failed for tempo event\n"); continue;}
                     tempo_ev.extra[0] = (mpqn >> 16) & 0xFF;
                     tempo_ev.extra[1] = (mpqn >> 8) & 0xFF;
                     tempo_ev.extra[2] = mpqn & 0xFF;
                     tempo_track->events[tempo_track->nevents++] = tempo_ev;
                 } else {
                     fprintf(stderr, "Warning: Track 0 full, cannot add tempo event.\n");
                 }
                 break;
             }
             case CMD_CHANGE_PROG: { // changeTrackInstrument <track> <new_instrument>
                 int instrument = cmd->args[1];
                 if (instrument < 0 || instrument > 127) {
                     fprintf(stderr, "Warning: Invalid instrument number %d. Skipping.\n", instrument);
                     continue;
                 }
                 if (!target_track_ptr) continue; // Should have been caught by initial check
 
                 uint8_t channel = get_or_assign_channel_for_track(target_track_ptr, target_track_idx);
 
                 if (target_track_ptr->nevents < MAX_EVENTS_PER_TRACK) {
                     MidiEvent prog_ev = {0};
                     prog_ev.tick = 0; // Program change usually at the start of track
                     prog_ev.status = 0xC0 | channel;
                     prog_ev.data1 = (uint8_t)instrument;
                     target_track_ptr->events[target_track_ptr->nevents++] = prog_ev;
                 } else {
                      fprintf(stderr, "Warning: Track %d full, cannot add program change.\n", track_num_1_based);
                 }
                 break;
             }
             case CMD_INC_PITCH: { // increasePitch <track> <startTime> <endTime> <increase_value>
                 if (!target_track_ptr) continue;
                 uint32_t start_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[0], mf->ppq);
                 uint32_t end_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[1], mf->ppq);
                 int increase_val = cmd->args[1];
 
                 for (int j = 0; j < target_track_ptr->nevents; ++j) {
                     MidiEvent* ev = &target_track_ptr->events[j];
                     uint8_t status_type = ev->status & 0xF0;
                     // Only change pitch for Note On/Off events on this track's channel
                     uint8_t event_channel = ev->status & 0x0F;
                     uint8_t track_channel = get_or_assign_channel_for_track(target_track_ptr, target_track_idx);
 
                     if ((status_type == 0x90 || status_type == 0x80) && event_channel == track_channel) {
                         if (ev->tick >= start_ticks && ev->tick <= end_ticks) {
                             int new_pitch = ev->data1 + increase_val;
                             if (new_pitch < 0) new_pitch = 0;
                             if (new_pitch > 127) new_pitch = 127;
                             ev->data1 = (uint8_t)new_pitch;
                         }
                     }
                 }
                 break;
             }
             case CMD_REVERSE: { // reverse <track> <startTime> <endTime>
                  if (!target_track_ptr) continue;
                 uint32_t start_ticks_range = (uint32_t)beats_to_ticks(cmd->dbl_args[0], mf->ppq);
                 uint32_t end_ticks_range = (uint32_t)beats_to_ticks(cmd->dbl_args[1], mf->ppq);
                 
                 uint32_t min_event_tick_in_range = UINT32_MAX;
                 uint32_t max_event_tick_in_range = 0;
                 int found_events_in_range = 0;
 
                 for (int j = 0; j < target_track_ptr->nevents; ++j) {
                     MidiEvent* ev = &target_track_ptr->events[j];
                     if (ev->tick >= start_ticks_range && ev->tick <= end_ticks_range) {
                         if (ev->tick < min_event_tick_in_range) min_event_tick_in_range = ev->tick;
                         if (ev->tick > max_event_tick_in_range) max_event_tick_in_range = ev->tick;
                         found_events_in_range = 1;
                     }
                 }
 
                 if (found_events_in_range) {
                     for (int j = 0; j < target_track_ptr->nevents; ++j) {
                         MidiEvent* ev = &target_track_ptr->events[j];
                         if (ev->tick >= start_ticks_range && ev->tick <= end_ticks_range) {
                              // Apply reversal based on the actual min/max ticks of events *selected by the time range*
                             ev->tick = min_event_tick_in_range + (max_event_tick_in_range - ev->tick);
                         }
                     }
                 }
                 break;
             }
             case CMD_ADD_TRACK: { // addTrack <instrument>
                 if (mf->actual_ntrks >= MAX_TRACKS) {
                     fprintf(stderr, "Warning: Cannot add new track, maximum tracks (%d) reached.\n", MAX_TRACKS);
                     continue;
                 }
                 int instrument = cmd->args[0];
                  if (instrument < 0 || instrument > 127) {
                     fprintf(stderr, "Warning: Invalid instrument number %d for new track. Using 0.\n", instrument);
                     instrument = 0;
                 }
 
                 MidiTrack* new_track = &mf->tracks[mf->actual_ntrks];
                 new_track->nevents = 0;
                 new_track->track_channel_found = 0xFF; // Will be set by get_or_assign or prog change
                 
                 uint8_t new_track_channel = get_or_assign_channel_for_track(new_track, mf->actual_ntrks);
 
                 if (new_track->nevents < MAX_EVENTS_PER_TRACK) {
                     MidiEvent prog_ev = {0};
                     prog_ev.tick = 0;
                     prog_ev.status = 0xC0 | new_track_channel;
                     prog_ev.data1 = (uint8_t)instrument;
                     new_track->events[new_track->nevents++] = prog_ev;
                 } else {
                     fprintf(stderr, "Warning: New track is full immediately? Cannot add program change.\n");
                 }
                 mf->actual_ntrks++;
                 // printf("Info: Added track %d with instrument %d on channel %d.\n", mf->actual_ntrks, instrument, new_track_channel);
                 break;
             }
             case CMD_DELETE_TRACK: { // deleteTrack <track_num_1_based>
                 // target_track_idx is already set and validated if we reach here
                 if (target_track_idx < 0 || target_track_idx >= mf->actual_ntrks) {
                      // This case should be caught by the initial check, but double-check
                     fprintf(stderr, "Internal Error or re-validation: Invalid track %d for delete.\n", cmd->args[0]);
                     continue;
                 }
                 
                 cleanup_midi_track(&mf->tracks[target_track_idx]); // Free any 'extra' data in events
 
                 for (int j = target_track_idx; j < mf->actual_ntrks - 1; ++j) {
                     mf->tracks[j] = mf->tracks[j + 1]; // Struct copy
                 }
                 mf->actual_ntrks--;
                 // printf("Info: Deleted track %d.\n", cmd->args[0]);
                 break;
             }
             case CMD_ADD_NOTE: { // addNote <track> <timestamp> <pitch_str> <duration> <velocity>
                 if (!target_track_ptr) continue;
                 uint32_t start_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[0], mf->ppq);
                 int pitch_val = pitch_to_midi(cmd->str_args[0]);
                 uint32_t duration_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[1], mf->ppq);
                 int velocity = cmd->args[1];
 
                 if (pitch_val < 0 || pitch_val > 127) {
                     fprintf(stderr, "Warning: Invalid pitch '%s'. Skipping addNote.\n", cmd->str_args[0]);
                     continue;
                 }
                 if (velocity < 0) velocity = 0;
                 if (velocity > 127) velocity = 127;
                 if (duration_ticks == 0) {
                     fprintf(stderr, "Warning: Note duration is 0 ticks. Skipping addNote.\n");
                     // continue; // Or allow, some systems might use 0-duration for special purposes
                 }
 
 
                 uint8_t channel = get_or_assign_channel_for_track(target_track_ptr, target_track_idx);
 
                 // Overlap check: "if this time interval already have a node with this pitch, ignore it"
                 // "this time interval" could mean exactly at start_ticks, or any overlap.
                 // For simplicity, checking for an existing Note ON at exact start_ticks and pitch.
                 int already_exists = 0;
                 for (int j = 0; j < target_track_ptr->nevents; ++j) {
                     MidiEvent* ev = &target_track_ptr->events[j];
                     if (ev->tick == start_ticks && (ev->status & 0xF0) == 0x90 &&
                         (ev->status & 0x0F) == channel && ev->data1 == pitch_val) {
                         already_exists = 1;
                         break;
                     }
                 }
                 if (already_exists) {
                     // printf("Info: Note at track %d, tick %u, pitch %d already exists. Ignoring addNote.\n", cmd->args[0], start_ticks, pitch_val);
                     continue;
                 }
 
                 if (target_track_ptr->nevents + 2 <= MAX_EVENTS_PER_TRACK) {
                     MidiEvent on_ev = {0}, off_ev = {0};
                     on_ev.tick = start_ticks;
                     on_ev.status = 0x90 | channel;
                     on_ev.data1 = (uint8_t)pitch_val;
                     on_ev.data2 = (uint8_t)velocity;
                     target_track_ptr->events[target_track_ptr->nevents++] = on_ev;
 
                     off_ev.tick = start_ticks + duration_ticks;
                     off_ev.status = 0x80 | channel;
                     off_ev.data1 = (uint8_t)pitch_val;
                     off_ev.data2 = 0; // Velocity for note off is often 0 or 64
                     target_track_ptr->events[target_track_ptr->nevents++] = off_ev;
                 } else {
                     fprintf(stderr, "Warning: Track %d full, cannot add note.\n", cmd->args[0]);
                 }
                 break;
             }
             case CMD_REM_NOTE: { // removeNote <track> <timestamp> <pitch_str>
                 if (!target_track_ptr) continue;
                 uint32_t target_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[0], mf->ppq);
                 int target_pitch = pitch_to_midi(cmd->str_args[0]);
 
                 if (target_pitch < 0 || target_pitch > 127) {
                     fprintf(stderr, "Warning: Invalid pitch '%s' for removeNote. Skipping.\n", cmd->str_args[0]);
                     continue;
                 }
                 
                 uint8_t channel = get_or_assign_channel_for_track(target_track_ptr, target_track_idx);
                 int note_on_idx = -1;
                 uint32_t note_on_tick = 0;
 
                 // Find Note On
                 for (int j = 0; j < target_track_ptr->nevents; ++j) {
                     MidiEvent* ev = &target_track_ptr->events[j];
                     if (!(ev->internal_flags & 0x01) && // not already marked
                         ev->tick == target_ticks &&
                         (ev->status & 0xF0) == 0x90 &&
                         (ev->status & 0x0F) == channel &&
                         ev->data1 == target_pitch) {
                         ev->internal_flags |= 0x01; // Mark Note On for deletion
                         note_on_idx = j;
                         note_on_tick = ev->tick; // Though it's target_ticks
                         break; 
                     }
                 }
 
                 if (note_on_idx != -1) {
                     // Find corresponding Note Off
                     // The corresponding Note Off has same pitch, same channel, and occurs at or after Note On tick.
                     // It's usually the *first* such Note Off.
                     for (int j = note_on_idx + 1; j < target_track_ptr->nevents; ++j) {
                          MidiEvent* ev = &target_track_ptr->events[j];
                          if (!(ev->internal_flags & 0x01) &&
                              ev->tick >= note_on_tick && // Note off must be at or after note on
                              (ev->status & 0xF0) == 0x80 &&
                              (ev->status & 0x0F) == channel &&
                              ev->data1 == target_pitch) {
                              ev->internal_flags |= 0x01; // Mark Note Off for deletion
                              break; // Found and marked the corresponding note-off
                          }
                     }
                     compact_track_events(target_track_ptr);
                 } else {
                     // "if this time interval didn’t have a node with this pitch, ignore it"
                     // printf("Info: Note to remove not found at track %d, tick %u, pitch %d. Ignoring.\n", cmd->args[0], target_ticks, target_pitch);
                 }
                 break;
             }
             case CMD_TRANSPOSE: { // transposition <oldTonality> <newTonality>
                 int old_p = pitch_to_midi(cmd->str_args[0]);
                 int new_p = pitch_to_midi(cmd->str_args[1]);
 
                 if (old_p == -1 || new_p == -1) {
                     fprintf(stderr, "Warning: Invalid tonality for transposition ('%s' or '%s'). Skipping.\n",
                             cmd->str_args[0], cmd->str_args[1]);
                     continue;
                 }
                 int interval = new_p - old_p;
 
                 for (int t_idx = 0; t_idx < mf->actual_ntrks; ++t_idx) {
                     MidiTrack* current_trk = &mf->tracks[t_idx];
                     uint8_t trk_channel = get_or_assign_channel_for_track(current_trk, t_idx); // Get channel for this track
                     for (int j = 0; j < current_trk->nevents; ++j) {
                         MidiEvent* ev = &current_trk->events[j];
                         uint8_t status_type = ev->status & 0xF0;
                         uint8_t event_channel = ev->status & 0x0F;
 
                         if ((status_type == 0x90 || status_type == 0x80) && event_channel == trk_channel) { // Only notes on this track's channel
                             int transposed_pitch = ev->data1 + interval;
                             if (transposed_pitch < 0) transposed_pitch = 0;
                             if (transposed_pitch > 127) transposed_pitch = 127;
                             ev->data1 = (uint8_t)transposed_pitch;
                         }
                     }
                 }
                 break;
             }
             case CMD_CHANGE_TS: { // changeTimeSignature <"N/D">
                 MidiTrack* ts_track = &mf->tracks[0]; // Assume time signature map is track 0
                  if (mf->actual_ntrks == 0) {
                     if (mf->actual_ntrks < MAX_TRACKS) {
                         mf->tracks[0].nevents = 0;
                         mf->tracks[0].track_channel_found = 0xFF;
                         mf->actual_ntrks = 1;
                         ts_track = &mf->tracks[0];
                         // printf("Info: Created track 1 for time signature due to changeTimeSignature on empty MIDI.\n");
                     } else {
                         fprintf(stderr, "Warning: Cannot add time signature, max tracks reached and no tracks exist.\n");
                         continue;
                     }
                 }
 
                 int num = 0, den = 0;
                 if (sscanf(cmd->str_args[0], "%d/%d", &num, &den) != 2 || num <= 0 || den <= 0) {
                     fprintf(stderr, "Warning: Invalid time signature format '%s'. Expected N/D. Skipping.\n", cmd->str_args[0]);
                     continue;
                 }
                 if ((den & (den - 1)) != 0 && den != 1) { // Check if den is a power of 2 (or 1)
                     // The log2 method for denominator expects power of 2.
                     // MIDI standard: denominator is stored as power of 2 (e.g. 2 for /4, 3 for /8)
                     // If "3" is given for 4/3, this requires different handling, but MIDI standard is specific.
                     // Assignment: "4/3, etc" implies non-power-of-2 denoms might be intended.
                     // However, MIDI standard FF 58 04 nn dd cc bb, dd is log2(denominator).
                     // For now, assume standard MIDI interpretation.
                     fprintf(stderr, "Warning: Time signature denominator %d is not a power of 2. MIDI standard requires power of 2. Skipping.\n", den);
                     continue;
                 }
 
 
                 // Remove old time signature events
                 for (int j = 0; j < ts_track->nevents; ++j) {
                     if (ts_track->events[j].status == 0xFF && ts_track->events[j].data1 == 0x58) {
                         ts_track->events[j].internal_flags |= 0x01;
                     }
                 }
                 compact_track_events(ts_track);
                 
                 if (ts_track->nevents < MAX_EVENTS_PER_TRACK) {
                     MidiEvent ts_ev = {0};
                     ts_ev.tick = 0; // Usually at the start
                     ts_ev.status = 0xFF; // Meta
                     ts_ev.data1 = 0x58;  // Time Signature type
                     ts_ev.extra_len = 4;
                     ts_ev.extra = (uint8_t*)malloc(4);
                     if (!ts_ev.extra) {fprintf(stderr,"Malloc failed for TS event\n"); continue;}
                     ts_ev.extra[0] = (uint8_t)num;
                     ts_ev.extra[1] = (uint8_t)(log2(den)); // Denominator as power of 2 (e.g., 2 for /4, 3 for /8)
                     ts_ev.extra[2] = 24; // MIDI clocks per metronome click (standard)
                     ts_ev.extra[3] = 8;  // Number of 32nd notes per 24 MIDI clocks (standard quarter note)
                     ts_track->events[ts_track->nevents++] = ts_ev;
                 } else {
                      fprintf(stderr, "Warning: Track 0 full, cannot add Time Signature event.\n");
                 }
                 break;
             }
             case CMD_INC_VEL: { // increaseVelocity <track> <startTime> <endTime> <increaseValue>
                 if (!target_track_ptr) continue;
                 uint32_t start_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[0], mf->ppq);
                 uint32_t end_ticks = (uint32_t)beats_to_ticks(cmd->dbl_args[1], mf->ppq);
                 int increase_val = cmd->args[1];
                 uint8_t track_channel = get_or_assign_channel_for_track(target_track_ptr, target_track_idx);
 
                 for (int j = 0; j < target_track_ptr->nevents; ++j) {
                     MidiEvent* ev = &target_track_ptr->events[j];
                     if ((ev->status & 0xF0) == 0x90 && (ev->status & 0x0F) == track_channel) { // Note On event for this track's channel
                         if (ev->tick >= start_ticks && ev->tick <= end_ticks) {
                             int new_vel = ev->data2 + increase_val;
                             if (new_vel < 0) new_vel = 0;
                             if (new_vel > 127) new_vel = 127;
                             ev->data2 = (uint8_t)new_vel;
                         }
                     }
                 }
                 break;
             }
             case CMD_UNKNOWN:
             default:
                 // Already handled in load_commands, or should not happen
                 break;
         }
     }
 }
 
 // --- Memory Cleanup ---
 void free_midi_event_extra(MidiEvent* ev) {
     if (ev && ev->extra) {
         free(ev->extra);
         ev->extra = NULL;
         ev->extra_len = 0;
     }
 }
 
 void cleanup_midi_track(MidiTrack* track) {
     if (!track) return;
     for (int i = 0; i < track->nevents; ++i) {
         free_midi_event_extra(&track->events[i]);
     }
     track->nevents = 0;
 }
 
 void cleanup_midi_file(MidiFile* mf) {
     if (!mf) return;
     for (int i = 0; i < mf->actual_ntrks; ++i) {
         cleanup_midi_track(&mf->tracks[i]);
     }
     mf->actual_ntrks = 0;
 }
 
 
 // --- Main Function ---
 int main() {
     char in_fname[MAX_FILENAME], out_fname[MAX_FILENAME], cmd_fname[MAX_FILENAME];
 
     printf("input file name: ");
     if (!fgets(in_fname, sizeof(in_fname), stdin)) { fprintf(stderr, "Error reading input file name.\n"); return 1; }
     in_fname[strcspn(in_fname, "\r\n")] = 0;
 
     printf("output file name: ");
     if (!fgets(out_fname, sizeof(out_fname), stdin)) { fprintf(stderr, "Error reading output file name.\n"); return 1; }
     out_fname[strcspn(out_fname, "\r\n")] = 0;
 
     printf("commands file name: ");
     if (!fgets(cmd_fname, sizeof(cmd_fname), stdin)) { fprintf(stderr, "Error reading commands file name.\n"); return 1; }
     cmd_fname[strcspn(cmd_fname, "\r\n")] = 0;
 
    // MidiFile mf = {0}; // Initialize all fields to zero/NULL
    MidiFile* mf_ptr = calloc(1, sizeof(MidiFile)); // NEW: Allocate on the heap
    if (!mf_ptr) {
        fprintf(stderr, "Critical: Failed to allocate memory for MidiFile structure.\n");
        return 1;
    }
     Command cmds[MAX_COMMANDS];
     int ncmds = 0;
 
     if (load_midi(in_fname, mf_ptr) != 0) { // Pass mf_ptr directly
        fprintf(stderr, "Failed to load MIDI file '%s'.\n", in_fname);
        cleanup_midi_file(mf_ptr); // Pass mf_ptr
        free(mf_ptr); // Don't forget to free the allocated memory
        return 1;
    }
     // printf("MIDI Loaded: Format %d, Header Tracks %d, Actual Tracks %d, PPQ %d\n",
     //        mf.format, mf.ntrks_from_header, mf.actual_ntrks, mf.ppq);
 
 
     if (load_commands(cmd_fname, cmds, &ncmds) != 0) {
        fprintf(stderr, "Failed to load commands from '%s'.\n", cmd_fname);
        cleanup_midi_file(mf_ptr); // Pass mf_ptr
        free(mf_ptr); // Free allocated memory
        return 1;
    }
     // printf("Loaded %d commands.\n", ncmds);
 
     apply_commands(mf_ptr, cmds, ncmds);
 
     if (write_midi(out_fname, mf_ptr) != 0) { // Pass mf_ptr
        fprintf(stderr, "Failed to write MIDI file to '%s'.\n", out_fname);
        cleanup_midi_file(mf_ptr); // Pass mf_ptr
        free(mf_ptr); // Free allocated memory
        return 1;
    }
 
     printf("Done. %d tracks written.\n", mf_ptr->actual_ntrks);
     
    cleanup_midi_file(mf_ptr); // Final cleanup of all allocated memory
    free(mf_ptr);

     return 0;
 }