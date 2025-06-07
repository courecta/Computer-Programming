#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libelf.h>
#include <gelf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <endian.h>
#include <stdarg.h> // For variadic functions if needed for string building

// DWARF constants (subset)
#define DW_TAG_compile_unit         0x11
#define DW_TAG_subprogram           0x2e
#define DW_TAG_formal_parameter     0x05
#define DW_TAG_base_type            0x24
#define DW_TAG_pointer_type         0x0f
#define DW_TAG_const_type           0x26
#define DW_TAG_typedef              0x16
#define DW_TAG_structure_type       0x13
#define DW_TAG_union_type           0x17
#define DW_TAG_enumeration_type     0x04
#define DW_TAG_array_type           0x01
#define DW_TAG_subroutine_type      0x15 // Added for function pointers
#define DW_TAG_unspecified_parameters 0x1e // For "..." in function signatures

#define DW_AT_name                  0x03
#define DW_AT_comp_dir              0x1b
#define DW_AT_producer              0x25
#define DW_AT_low_pc                0x11
#define DW_AT_high_pc               0x12
#define DW_AT_stmt_list             0x10 // Offset in .debug_line
#define DW_AT_type                  0x49
#define DW_AT_decl_file             0x3a // Standard DWARF
#define DW_AT_decl_line             0x3b // Standard DWARF
#define DW_AT_declaration           0x3f // Flag
#define DW_AT_sibling               0x45
#define DW_AT_byte_size             0x0b
#define DW_AT_encoding              0x07
#define DW_AT_frame_base            0x40 // Location description (often exprloc)

// Forms
#define DW_FORM_addr                0x01
#define DW_FORM_block2              0x03 // Original code used this for strp, DWARF4: DW_FORM_data2
#define DW_FORM_block4              0x04 // Original code used this for strp, DWARF4: DW_FORM_data4
#define DW_FORM_data2               0x05
#define DW_FORM_data4               0x06
#define DW_FORM_data8               0x07
#define DW_FORM_string              0x08
#define DW_FORM_block               0x09
#define DW_FORM_block1              0x0a
#define DW_FORM_data1               0x0b
#define DW_FORM_flag                0x0c
#define DW_FORM_sdata               0x0d
#define DW_FORM_strp                0x0e // Offset in .debug_str
#define DW_FORM_udata               0x0f
#define DW_FORM_ref_addr            0x10 // Offset in .debug_info
#define DW_FORM_ref1                0x11
#define DW_FORM_ref2                0x12
#define DW_FORM_ref4                0x13 // Offset from CU start
#define DW_FORM_ref8                0x14
#define DW_FORM_ref_udata           0x15
#define DW_FORM_indirect            0x16
#define DW_FORM_sec_offset          0x17 // Offset in a section (e.g. .debug_line)
#define DW_FORM_exprloc             0x18
#define DW_FORM_flag_present        0x19
#define DW_FORM_strx                0x1a // DWARF 5: string index
#define DW_FORM_addrx               0x1b // DWARF 5: address index
#define DW_FORM_ref_sup4            0x1c // DWARF 5: reference to supplementary
#define DW_FORM_strp_sup            0x1d // DWARF 5: string pointer to supplementary
#define DW_FORM_data16              0x1e // DWARF 5: 16-byte data
#define DW_FORM_line_strp           0x1f // DWARF 5: offset in .debug_line_str
#define DW_FORM_ref_sig8            0x20
#define DW_FORM_implicit_const      0x21 // DWARF 5: value in abbrev
#define DW_FORM_loclistx            0x22 // DWARF 5: location list index
#define DW_FORM_rnglistx            0x23 // DWARF 5: range list index  
#define DW_FORM_ref_sup8            0x24 // DWARF 5: 8-byte reference to supplementary
#define DW_FORM_strx1               0x25 // DWARF 5: 1-byte string index
#define DW_FORM_strx2               0x26 // DWARF 5: 2-byte string index
#define DW_FORM_strx3               0x27 // DWARF 5: 3-byte string index
#define DW_FORM_strx4               0x28 // DWARF 5: 4-byte string index
#define DW_FORM_addrx1              0x29 // DWARF 5: 1-byte address index
#define DW_FORM_addrx2              0x2a // DWARF 5: 2-byte address index
#define DW_FORM_addrx3              0x2b // DWARF 5: 3-byte address index
#define DW_FORM_addrx4              0x2c // DWARF 5: 4-byte address index

// DWARF 5 Line Number Content Types (for directory and file entry formats)
#define DW_LNCT_path                0x1
#define DW_LNCT_directory_index     0x2
#define DW_LNCT_timestamp           0x3
#define DW_LNCT_size                0x4
#define DW_LNCT_MD5                 0x5

/* Helpers to read LEB128 values */
static uint64_t read_uleb(const uint8_t **p) {
    uint64_t result = 0; int shift = 0;
    while (1) {
        uint8_t byte = *(*p)++;
        result |= (uint64_t)(byte & 0x7f) << shift;
        if (!(byte & 0x80)) break;
        shift += 7;
    }
    return result;
}
static int64_t read_sleb(const uint8_t **p) {
    int64_t result = 0; int shift = 0; uint8_t byte;
    do {
        byte = *(*p)++;
        result |= (int64_t)(byte & 0x7f) << shift;
        shift += 7;
    } while (byte & 0x80);
    if (shift < 64 && (byte & 0x40))
        result |= -((int64_t)1 << shift);
    return result;
}

/* Abbrev structures */
typedef struct AttrSpec { 
    uint64_t name, form; 
    int64_t implicit_const_value; // For DW_FORM_implicit_const
    struct AttrSpec *next; 
} AttrSpec;
typedef struct Abbrev { uint64_t code; uint32_t tag; uint8_t has_children; AttrSpec *attrs; struct Abbrev *next; } Abbrev;

// Context for the current Compilation Unit
typedef struct FileEntry {
    char *name;
    uint64_t dir_index; // 1-based index into directories array
    // uint64_t mod_time;
    // uint64_t length;
} FileEntry;

typedef struct CUContext {
    const uint8_t *cu_compilation_unit_base; // Start of the CU header itself
    const uint8_t *cu_die_start; // Pointer to the first DIE in this CU
    uint32_t cu_length;          // Length of the CU (excluding the length field itself)
    uint16_t cu_version;
    uint32_t cu_abbrev_offset;
    uint8_t cu_address_size;

    const uint8_t *debug_info_base;
    const uint8_t *debug_str_base;
    const uint8_t *debug_line_str_base; // DWARF 5 line strings
    const uint8_t *debug_line_base; // Start of .debug_line section
    const uint8_t *debug_abbrev_base; // Start of .debug_abbrev section
    size_t debug_abbrev_size;         // Total size of .debug_abbrev section

    char **include_directories;
    uint64_t num_include_directories;
    FileEntry *file_entries;
    uint64_t num_file_entries;

    const char *cu_main_src_name;     // From DW_AT_name of CU
    const char *cu_main_src_comp_dir; // From DW_AT_comp_dir of CU
    int cu_idx; // For printing
    Abbrev *cu_abbrev_list_head;
    int func_idx_in_cu; // For numbering functions within this CU
} CUContext;

// Helper to free an abbrev list
static void free_abbrev_list(Abbrev *head) {
    while (head) {
        Abbrev *tmp = head;
        head = head->next;
        AttrSpec *as = tmp->attrs;
        while(as) {
            AttrSpec *tmp_as = as;
            as = as->next;
            free(tmp_as);
        }
        free(tmp);
    }
}

Abbrev* parse_cu_abbrevs(const uint8_t *abbrev_section_base, size_t abbrev_section_size, uint32_t cu_abbrev_table_offset) {
    Abbrev *cu_local_abbrev_head = NULL;
    const uint8_t *p = abbrev_section_base + cu_abbrev_table_offset;
    const uint8_t *section_end = abbrev_section_base + abbrev_section_size;

    if (p >= section_end) {
        fprintf(stderr, "Warning: CU abbrev offset 0x%x is out of .debug_abbrev bounds (size 0x%zx).\n", cu_abbrev_table_offset, abbrev_section_size);
        return NULL;
    }

    while (p < section_end) {
        uint64_t code = read_uleb(&p);
        if (code == 0) { // End of abbreviations for this CU
            break;
        }
        if (p >= section_end && code !=0) { // Avoid reading past end if last code was non-zero but incomplete
             fprintf(stderr, "Warning: Ran out of data in .debug_abbrev while parsing CU's table after code %lu.\n", code);
             break;
        }

        Abbrev *ab = calloc(1, sizeof(*ab));
        if (!ab) { perror("calloc Abbrev"); free_abbrev_list(cu_local_abbrev_head); return NULL; }
        ab->code = code;
        ab->tag = read_uleb(&p);
        ab->has_children = *p++;
        AttrSpec **aps = &ab->attrs;
        while (p < section_end) { // Check p before reading name/form
            uint64_t name = read_uleb(&p);
            uint64_t form = read_uleb(&p);
            if (name == 0 && form == 0) break; // End of attributes for this abbrev
            AttrSpec *as = calloc(1, sizeof(*as));
            if (!as) { perror("calloc AttrSpec"); free_abbrev_list(cu_local_abbrev_head); free(ab); return NULL; }
            as->name = name; as->form = form;
            
            // Handle DW_FORM_implicit_const which has a value in the abbrev
            if (form == DW_FORM_implicit_const) {
                as->implicit_const_value = read_sleb(&p);
            }
            
            *aps = as; aps = &as->next;
            if (p >= section_end && (name !=0 || form !=0)) { // Check after reading name/form
                 fprintf(stderr, "Warning: Ran out of data in .debug_abbrev while parsing attributes for abbrev code %lu.\n", ab->code);
                 // Partial abbrev, might be problematic. For now, we keep what we have.
                 break; 
            }
        }
        // Prepend to CU-local list
        ab->next = cu_local_abbrev_head;
        cu_local_abbrev_head = ab;
    }
    return cu_local_abbrev_head;
}

// Find abbrev in a CU-specific list
static Abbrev *find_abbrev(Abbrev *cu_abbrev_list_head, uint64_t code) {
    for (Abbrev *a = cu_abbrev_list_head; a; a = a->next)
        if (a->code == code) return a;
    // Warning is now more localized if an abbrev is not found for a specific CU
    // fprintf(stderr, "Warning: Abbreviation code %lu not found in current CU.\n", code);
    return NULL;
}

/* Read attribute value by DW_FORM */
// Returns raw value; interpretation (like string lookup) is separate
static uint64_t read_form_val(const uint8_t **p, uint64_t form, uint8_t addr_size, const uint8_t* cu_start_ptr, int64_t implicit_const_value) {
    switch (form) {
    case DW_FORM_addr:       *p += addr_size; return 0; // Value is the address itself, not handled here
    case DW_FORM_string:     { const char *s = (const char *)*p; *p += strlen(s) + 1; return (uint64_t)(uintptr_t)s; } // Inline string
    case DW_FORM_strp:       { uint32_t off = le32toh(*(uint32_t *)(*p)); *p += 4; return off; } // Offset in .debug_str
    case DW_FORM_data1:      return *(*p)++;
    case DW_FORM_data2:      { uint16_t v = le16toh(*(uint16_t *)(*p)); *p += 2; return v; }
    case DW_FORM_data4:      { uint32_t v = le32toh(*(uint32_t *)(*p)); *p += 4; return v; }
    case DW_FORM_data8:      { uint64_t v = *(uint64_t *)(*p); *p += 8; return v; } // Assuming host endian matches or direct copy
    case DW_FORM_sdata:      return (uint64_t)read_sleb(p);
    case DW_FORM_udata:      return read_uleb(p);
    case DW_FORM_ref1:       { uint8_t off = *(*p)++; return off; } // CU-relative offset
    case DW_FORM_ref2:       { uint16_t off = le16toh(*((uint16_t*)*p)); *p += 2; return off; } // CU-relative
    case DW_FORM_ref4:       { uint32_t off = le32toh(*((uint32_t*)*p)); *p += 4; return off; } // CU-relative
    case DW_FORM_ref8:       { uint64_t off = *((uint64_t*)*p); *p += 8; return off; } // CU-relative
    case DW_FORM_ref_addr:   { // Offset from start of .debug_info, size is address_size
        if (addr_size == 4) { uint32_t off = le32toh(*(uint32_t *)(*p)); *p += 4; return off; }
        else if (addr_size == 8) { uint64_t off = *(uint64_t *)(*p); *p += 8; return off; } // Assuming host endian
        *p += addr_size; return 0; // Fallback
    }
    case DW_FORM_ref_udata:  return read_uleb(p); // CU-relative offset
    case DW_FORM_sec_offset: { uint32_t off = le32toh(*(uint32_t *)(*p)); *p += 4; return off; } // Offset in another section
    case DW_FORM_flag:       return *(*p)++;
    case DW_FORM_flag_present: return 1; // Value is implicit
    case DW_FORM_strx:         { uint64_t idx = read_uleb(p); return idx; } // String index - simplified for now
    case DW_FORM_strx1:        { uint8_t idx = *(*p)++; return idx; } // 1-byte string index
    case DW_FORM_strx2:        { uint16_t idx = le16toh(*(uint16_t *)(*p)); *p += 2; return idx; } // 2-byte string index
    case DW_FORM_strx3:        { // 3-byte string index - read as little endian
        uint32_t idx = ((*p)[0]) | ((*p)[1] << 8) | ((*p)[2] << 16);
        *p += 3; return idx;
    }
    case DW_FORM_strx4:        { uint32_t idx = le32toh(*(uint32_t *)(*p)); *p += 4; return idx; } // 4-byte string index
    case DW_FORM_line_strp:    { uint32_t off = le32toh(*(uint32_t *)(*p)); *p += 4; return off; } // Offset in .debug_line_str
    case DW_FORM_addrx:        { uint64_t idx = read_uleb(p); return idx; } // Address index - simplified
    case DW_FORM_addrx1:       { uint8_t idx = *(*p)++; return idx; } // 1-byte address index
    case DW_FORM_addrx2:       { uint16_t idx = le16toh(*(uint16_t *)(*p)); *p += 2; return idx; } // 2-byte address index
    case DW_FORM_addrx3:       { // 3-byte address index
        uint32_t idx = ((*p)[0]) | ((*p)[1] << 8) | ((*p)[2] << 16);
        *p += 3; return idx;
    }
    case DW_FORM_addrx4:       { uint32_t idx = le32toh(*(uint32_t *)(*p)); *p += 4; return idx; } // 4-byte address index
    case DW_FORM_data16:       { *p += 16; return 0; } // 16-byte data block, skip for now
    case DW_FORM_implicit_const: return (uint64_t)implicit_const_value; // Value is in abbreviation declaration
    case DW_FORM_loclistx:     { uint64_t idx = read_uleb(p); return idx; } // Location list index
    case DW_FORM_rnglistx:     { uint64_t idx = read_uleb(p); return idx; } // Range list index
    case DW_FORM_exprloc:    { uint64_t len = read_uleb(p); *p += len; return 0; } // Skip exprloc block
    case DW_FORM_block:      { uint64_t len = read_uleb(p); *p += len; return 0; }
    case DW_FORM_block1:     { uint8_t len = *(*p)++; *p += len; return 0; }
    case DW_FORM_block2:     { uint16_t len = le16toh(*(uint16_t *)(*p)); *p += 2; *p += len; return 0; }
    case DW_FORM_block4:     { uint32_t len = le32toh(*(uint32_t *)(*p)); *p += 4; *p += len; return 0; }
    // DW_FORM_indirect is special, value is a ULEB specifying another form
    case DW_FORM_indirect: {
        uint64_t actual_form = read_uleb(p);
        return read_form_val(p, actual_form, addr_size, cu_start_ptr, implicit_const_value);
    }
    case 0x0: // NULL form or error condition
        return 0;
    case 0x2: // Might be an alternative DW_FORM_block2 or vendor extension
        { uint16_t len = le16toh(*(uint16_t *)(*p)); *p += 2; *p += len; return 0; }
    default:
        fprintf(stderr, "Warning: Unsupported DW_FORM 0x%lx\n", form);
        return 0;
    }
}

static const char *get_str_val(uint64_t val, uint64_t form, const uint8_t *str_base, const uint8_t *line_str_base) {
    if (form == DW_FORM_string) return (const char *)(uintptr_t)val;
    if (form == DW_FORM_strp) return (const char *)(str_base + val);
    if (form == DW_FORM_line_strp) return line_str_base ? (const char *)(line_str_base + val) : "<?no_line_str?>";
    // DWARF 5 string index forms - simplified handling (should lookup in string offsets table)
    if (form == DW_FORM_strx || form == DW_FORM_strx1 || form == DW_FORM_strx2 || 
        form == DW_FORM_strx3 || form == DW_FORM_strx4) {
        return "<?strx_not_impl?>"; // String index forms need proper implementation
    }
    return "";
}

// Forward declaration
char* resolve_type_to_string_recursive(CUContext *ctx, uint64_t type_die_cu_offset, int depth);
void parse_die(CUContext *ctx, const uint8_t **p_die_ptr, const uint8_t *unit_end_ptr, int recursion_depth);


void parse_line_program_header(CUContext *ctx, uint64_t line_offset_in_debug_line) {
    if (!ctx->debug_line_base) return;
    const uint8_t *p = ctx->debug_line_base + line_offset_in_debug_line;
    const uint8_t *line_prog_start = p;

    // Free old entries if any
    if (ctx->include_directories) {
        for (uint64_t i = 0; i < ctx->num_include_directories; ++i) free(ctx->include_directories[i]);
        free(ctx->include_directories);
        ctx->include_directories = NULL;
        ctx->num_include_directories = 0;
    }
    if (ctx->file_entries) {
        for (uint64_t i = 0; i < ctx->num_file_entries; ++i) free(ctx->file_entries[i].name);
        free(ctx->file_entries);
        ctx->file_entries = NULL;
        ctx->num_file_entries = 0;
    }

    // Line Program Header
    uint32_t total_length = le32toh(*(uint32_t*)p); p += 4;
    const uint8_t *header_end = line_prog_start + 4 + total_length; // End of this CU's line program
    uint16_t version = le16toh(*(uint16_t*)p); p += 2;
    if (version < 2 || version > 5) { // Support DWARF 2-5 line headers
        fprintf(stderr, "Warning: Unsupported .debug_line version %u for CU %d\n", version, ctx->cu_idx);
        return;
    }
    uint32_t header_length = le32toh(*(uint32_t*)p); p += 4;
    const uint8_t *prologue_end = p + header_length;
    
    // DWARF 5 has additional header fields
    if (version >= 5) {
        uint8_t address_size = *p++; // Address size  
        uint8_t segment_selector_size = *p++; // Segment selector size
    }
    
    /*uint8_t min_instruction_length = *p++; p++; // Skip default_is_stmt, line_base, line_range, opcode_base
    p++; p++; p++;*/
    // Skip min_instruction_length, maximum_ops_per_instruction (DWARF 4+),
    // default_is_stmt, line_base, line_range, opcode_base
    p++; // min_instruction_length
    if (version >= 4) p++; // maximum_ops_per_instruction
    p++; // default_is_stmt
    p++; // line_base
    p++; // line_range
    uint8_t opcode_base = *p++;
    // Skip standard_opcode_lengths
    p += opcode_base -1;


    // Directory and File parsing differs between DWARF 2-4 and DWARF 5
    if (version >= 5) {
        // DWARF 5 format with directory_entry_format and file_name_entry_format
        
        // Directory entry format
        uint8_t directory_entry_format_count = *p++;
        
        // Store directory format descriptors
        struct { uint64_t content_type; uint64_t form; } dir_formats[8]; // Max reasonable formats
        for (uint8_t i = 0; i < directory_entry_format_count && i < 8; i++) {
            dir_formats[i].content_type = read_uleb(&p);
            dir_formats[i].form = read_uleb(&p);
        }
        
        // Directories count
        ctx->num_include_directories = read_uleb(&p);
        if (ctx->num_include_directories > 0) {
            ctx->include_directories = calloc(ctx->num_include_directories, sizeof(char*));
            if (!ctx->include_directories) { 
                perror("calloc include_directories"); 
                ctx->num_include_directories = 0; 
                return; 
            }
            
            // Read each directory entry
            for (uint64_t i = 0; i < ctx->num_include_directories; i++) {
                for (uint8_t j = 0; j < directory_entry_format_count && j < 8; j++) {
                    if (dir_formats[j].content_type == DW_LNCT_path) {
                        // This is the directory path
                        if (dir_formats[j].form == DW_FORM_string) {
                            ctx->include_directories[i] = strdup((const char*)p);
                            p += strlen((const char*)p) + 1;
                        } else if (dir_formats[j].form == DW_FORM_line_strp) {
                            uint32_t offset = le32toh(*(uint32_t*)p);
                            p += 4;
                            if (ctx->debug_line_str_base) {
                                ctx->include_directories[i] = strdup((const char*)(ctx->debug_line_str_base + offset));
                            } else {
                                ctx->include_directories[i] = strdup("<?no_line_str?>");
                            }
                        } else {
                            // Unknown form, skip
                            read_form_val(&p, dir_formats[j].form, ctx->cu_address_size, ctx->cu_compilation_unit_base, 0);
                            ctx->include_directories[i] = strdup("<?unknown_dir_form?>");
                        }
                        if (!ctx->include_directories[i]) {
                            perror("strdup include_dir");
                            return;
                        }
                    } else {
                        // Skip other content types
                        read_form_val(&p, dir_formats[j].form, ctx->cu_address_size, ctx->cu_compilation_unit_base, 0);
                    }
                }
                if (!ctx->include_directories[i]) {
                    ctx->include_directories[i] = strdup(""); // Default empty if no path found
                }
            }
        }
        
        // File name entry format
        uint8_t file_name_entry_format_count = *p++;
        
        // Store file format descriptors  
        struct { uint64_t content_type; uint64_t form; } file_formats[8]; // Max reasonable formats
        for (uint8_t i = 0; i < file_name_entry_format_count && i < 8; i++) {
            file_formats[i].content_type = read_uleb(&p);
            file_formats[i].form = read_uleb(&p);
        }
        
        // File names count
        ctx->num_file_entries = read_uleb(&p);
        if (ctx->num_file_entries > 0) {
            ctx->file_entries = calloc(ctx->num_file_entries, sizeof(FileEntry));
            if (!ctx->file_entries) { 
                perror("calloc file_entries"); 
                ctx->num_file_entries = 0; 
                return; 
            }
            
            // Read each file entry
            for (uint64_t i = 0; i < ctx->num_file_entries; i++) {
                ctx->file_entries[i].dir_index = 0; // Default
                for (uint8_t j = 0; j < file_name_entry_format_count && j < 8; j++) {
                    if (file_formats[j].content_type == DW_LNCT_path) {
                        // This is the file name
                        if (file_formats[j].form == DW_FORM_string) {
                            ctx->file_entries[i].name = strdup((const char*)p);
                            p += strlen((const char*)p) + 1;
                        } else if (file_formats[j].form == DW_FORM_line_strp) {
                            uint32_t offset = le32toh(*(uint32_t*)p);
                            p += 4;
                            if (ctx->debug_line_str_base) {
                                ctx->file_entries[i].name = strdup((const char*)(ctx->debug_line_str_base + offset));
                            } else {
                                ctx->file_entries[i].name = strdup("<?no_line_str?>");
                            }
                        } else {
                            // Unknown form, skip
                            read_form_val(&p, file_formats[j].form, ctx->cu_address_size, ctx->cu_compilation_unit_base, 0);
                            ctx->file_entries[i].name = strdup("<?unknown_file_form?>");
                        }
                        if (!ctx->file_entries[i].name) {
                            perror("strdup file_name");
                            return;
                        }
                    } else if (file_formats[j].content_type == DW_LNCT_directory_index) {
                        // This is the directory index
                        ctx->file_entries[i].dir_index = read_form_val(&p, file_formats[j].form, ctx->cu_address_size, ctx->cu_compilation_unit_base, 0);
                    } else {
                        // Skip other content types (timestamp, size, etc.)
                        read_form_val(&p, file_formats[j].form, ctx->cu_address_size, ctx->cu_compilation_unit_base, 0);
                    }
                }
                if (!ctx->file_entries[i].name) {
                    ctx->file_entries[i].name = strdup("<?no_filename?>"); // Default if no path found
                }
            }
        }
        
    } else {
        // DWARF 2-4 format (original implementation)
        
        // Include Directories
        // Count first
        const uint8_t *temp_p = p;
        while (*temp_p) {
            ctx->num_include_directories++;
            temp_p += strlen((const char*)temp_p) + 1;
        }
        temp_p++; // Skip final null terminator for directory list

        if (ctx->num_include_directories > 0) {
            ctx->include_directories = calloc(ctx->num_include_directories, sizeof(char*));
            if (!ctx->include_directories) { perror("calloc include_directories"); ctx->num_include_directories = 0; return; }
            for (uint64_t i = 0; i < ctx->num_include_directories; ++i) {
                ctx->include_directories[i] = strdup((const char*)p);
                if (!ctx->include_directories[i]) { perror("strdup include_dir"); /* partial cleanup? */ return; }
                p += strlen((const char*)p) + 1;
            }
        }
        p++; // Skip final null terminator for directory list

        // File Names
        // Count first
        temp_p = p;
        uint64_t file_count = 0;
        while(*temp_p) {
            file_count++;
            temp_p += strlen((const char*)temp_p) + 1; // file name
            read_uleb(&temp_p); // dir_index
            read_uleb(&temp_p); // time
            read_uleb(&temp_p); // length
        }

        ctx->num_file_entries = file_count;
        if (ctx->num_file_entries > 0) {
            ctx->file_entries = calloc(ctx->num_file_entries, sizeof(FileEntry));
            if (!ctx->file_entries) { perror("calloc file_entries"); ctx->num_file_entries = 0; return; }
            for (uint64_t i = 0; i < ctx->num_file_entries; ++i) {
                ctx->file_entries[i].name = strdup((const char*)p);
                if (!ctx->file_entries[i].name) { perror("strdup file_name"); return; }
                p += strlen((const char*)p) + 1;
                ctx->file_entries[i].dir_index = read_uleb(&p);
                read_uleb(&p); // Skip time
                read_uleb(&p); // Skip length
            }
        }
        p++; // Skip final null for file list
    }
    // Remainder is the line number program instructions, which we don't parse for this assignment.
}

char* resolve_type_to_string_recursive(CUContext *ctx, uint64_t type_die_cu_offset, int depth) {
    if (depth > 10) return strdup("..."); // Recursion guard
    if (type_die_cu_offset == 0) return strdup("void"); // Common convention for DW_AT_type missing or 0

    const uint8_t *p_type_die = ctx->cu_compilation_unit_base + type_die_cu_offset;
    if (p_type_die < ctx->debug_info_base || p_type_die >= ctx->debug_info_base + (ctx->debug_info_base == ctx->cu_compilation_unit_base ? ctx->cu_length + 11 : 1024*1024 /* some large bound if not same CU */ ) ) {
    if (p_type_die < ctx->cu_compilation_unit_base || p_type_die >= ctx->cu_compilation_unit_base + ctx->cu_length + 11 /*approx header size*/) {
            fprintf(stderr, "Warning: Type DIE offset 0x%lx (from CU base 0x%p) seems out of CU bounds (length 0x%x).\n",
                    type_die_cu_offset, (void*)ctx->cu_compilation_unit_base, ctx->cu_length);
            return strdup("<?type_offset_err?>");
       }
    }

    uint64_t abbrev_code = read_uleb(&p_type_die);
    Abbrev *ab = find_abbrev(ctx->cu_abbrev_list_head, abbrev_code);
    if (!ab) return strdup("<?unknown_type_abbrev?>");

    char *name_str = NULL;
    uint64_t base_type_offset = 0;
    // Attributes specific to types
    const char* type_name_attr = NULL; // From DW_AT_name
    uint64_t byte_size_attr = 0;       // For DW_AT_byte_size
    uint64_t encoding_attr = 0;        // For DW_AT_encoding

    const uint8_t *type_attr_scan_ptr = p_type_die; // p_type_die is already advanced past abbrev code
    for (AttrSpec *as = ab->attrs; as; as = as->next) {
        uint64_t val = read_form_val(&type_attr_scan_ptr, as->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, as->implicit_const_value);
        switch (as->name) {
            case DW_AT_type: base_type_offset = val; break;
            case DW_AT_name: type_name_attr = get_str_val(val, as->form, ctx->debug_str_base, ctx->debug_line_str_base); break;
            case DW_AT_byte_size: byte_size_attr = val; break;
            case DW_AT_encoding: encoding_attr = val; break;
        }
    }
    p_type_die = type_attr_scan_ptr;

    char *res = NULL;
    switch (ab->tag) {
        case DW_TAG_base_type:
            if (type_name_attr) res = strdup(type_name_attr);
            else res = strdup("<?unnamed_base_type?>");
            break;
        case DW_TAG_pointer_type: {
           char *underlying_type = resolve_type_to_string_recursive(ctx, base_type_offset, depth + 1);
            if (underlying_type) {
                // Heuristic: If underlying type is already a function pointer signature, don't add another '*'
                // A simple check: does it contain "(*)" ? More robust might be " (*)("
                if (strstr(underlying_type, "(*)") != NULL || strstr(underlying_type, " (*)(") != NULL) {
                    res = underlying_type; // Use as-is
                } else {
                    size_t len = strlen(underlying_type) + 2; // For " *" and null terminator
                    res = malloc(len);
                    if (res) snprintf(res, len, "%s*", underlying_type);
                    else res = strdup("?ptr_alloc_err?");
                    free(underlying_type);
                }
            } else {
                res = strdup("void*"); // Fallback if underlying type fails
            }
            break;
        }
        case DW_TAG_const_type: {
            char *underlying_type = resolve_type_to_string_recursive(ctx, base_type_offset, depth + 1);
            size_t len = strlen(underlying_type) + 7; // for "const "
            res = malloc(len);
            if (res) snprintf(res, len, "const %s", underlying_type);
            else res = strdup("?const_alloc_err?");
            free(underlying_type);
            break;
        }
        case DW_TAG_typedef:
            // Resolve the typedef to its actual type
            if (type_name_attr && depth == 0) { // Only show typedef name at top level if requested by assignment logic
                 // The assignment asks to decode until base type. So we recurse.
            }
            free(res); // free any previous allocation for res
            res = resolve_type_to_string_recursive(ctx, base_type_offset, depth + 1); // Effectively skip the typedef name
            break;
        case DW_TAG_structure_type:
        case DW_TAG_union_type:
        case DW_TAG_enumeration_type: {
            const char* tag_prefix = (ab->tag == DW_TAG_structure_type) ? "struct " :
                                     (ab->tag == DW_TAG_union_type) ? "union " : "enum ";
            if (type_name_attr && type_name_attr[0] != '\0') {
                size_t prefix_len = strlen(tag_prefix);
                size_t name_len = strlen(type_name_attr);
                res = malloc(prefix_len + name_len + 1);
                if (res) { strcpy(res, tag_prefix); strcat(res, type_name_attr); }
                else res = strdup("?struct_alloc_err?");
            } else {
                 res = strdup( (ab->tag == DW_TAG_structure_type) ? "struct" :
                                     (ab->tag == DW_TAG_union_type) ? "union" : "enum" );
            }
            break;
        }
        case DW_TAG_array_type: {
             char *element_type = resolve_type_to_string_recursive(ctx, base_type_offset, depth + 1);
             // Further parsing of DW_TAG_subrange_type children needed for bounds. Simplified:
             size_t len = strlen(element_type) + 3; // for "[]"
             res = malloc(len);
             if (res) snprintf(res, len, "%s[]", element_type);
             else res = strdup("?array_alloc_err?");
             free(element_type);
             break;
        }
        case DW_TAG_subroutine_type: {
            char *return_type_str = strdup("void"); // Default return type
            if (base_type_offset != 0) { // DW_AT_type specifies the return type
                free(return_type_str);
                return_type_str = resolve_type_to_string_recursive(ctx, base_type_offset, depth + 1);
            }

            char params_str[512] = {0};
            int first_param = 1;

            // Subroutine types have children DW_TAG_formal_parameter
            // The p_type_die currently points *after* the attributes of the DW_TAG_subroutine_type DIE.
            // Children DIEs follow directly.
            if (ab->has_children) {
                const uint8_t *children_ptr = p_type_die; // Start of children DIEs
                // Calculate end of current DIE to avoid reading past its children
                // This is tricky without knowing the full size of the current DIE.
                // For simplicity, we'll assume children are processed until a null DIE.
                // A more robust way would be to know the end of the current DIE.

                while (*children_ptr != 0) { // Iterate over children DIEs
                    const uint8_t *param_die_start_ptr = children_ptr;
                    uint64_t param_abbrev_code = read_uleb(&children_ptr);
                    if (!param_abbrev_code) break; // End of children for this subroutine type

                    Abbrev *param_ab = find_abbrev(ctx->cu_abbrev_list_head, param_abbrev_code);
                    if (!param_ab) {
                        strncat(params_str, "<?err_param_abbrev?>", sizeof(params_str) - strlen(params_str) -1);
                        break; // Error finding abbrev for child
                    }

                    uint64_t param_type_offset_attr = 0;
                    const uint8_t *param_attr_ptr = children_ptr; // Pointer to attributes of child
                    for (AttrSpec *pas = param_ab->attrs; pas; pas = pas->next) {
                        uint64_t pval = read_form_val(&param_attr_ptr, pas->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, pas->implicit_const_value);
                        if (pas->name == DW_AT_type) param_type_offset_attr = pval;
                    }
                    children_ptr = param_attr_ptr; // Advance past param attributes

                    if (param_ab->tag == DW_TAG_formal_parameter || param_ab->tag == DW_TAG_unspecified_parameters) { // DW_TAG_unspecified_parameters for "..."
                        if (!first_param) strncat(params_str, ", ", sizeof(params_str) - strlen(params_str) - 1);
                        
                        if (param_ab->tag == DW_TAG_unspecified_parameters) {
                            strncat(params_str, "...", sizeof(params_str) - strlen(params_str) - 1);
                        } else {
                            char *param_type_str = resolve_type_to_string_recursive(ctx, param_type_offset_attr, depth + 1);
                            strncat(params_str, param_type_str, sizeof(params_str) - strlen(params_str) - 1);
                            free(param_type_str);
                        }
                        first_param = 0;
                    }

                    // Skip children of this formal_parameter DIE if any (not typical)
                    if (param_ab->has_children) {
                        const uint8_t* temp_child_ptr = children_ptr;
                        // Simplified skip: advance children_ptr past the children of this param
                        while (*temp_child_ptr != 0) {
                            uint64_t temp_code = read_uleb(&temp_child_ptr);
                            if (!temp_code) break;
                            Abbrev *temp_ab = find_abbrev(ctx->cu_abbrev_list_head, temp_code);
                            if (!temp_ab) { temp_child_ptr++; continue; } // Basic skip
                            for(AttrSpec* temp_as = temp_ab->attrs; temp_as; temp_as = temp_as->next) {
                                read_form_val(&temp_child_ptr, temp_as->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, temp_as->implicit_const_value);
                            }
                            if (temp_ab->has_children) { // If param's child has children, skip them too
                                while(*temp_child_ptr != 0) temp_child_ptr++; // Skip until null DIE
                            }
                            if (*temp_child_ptr == 0) temp_child_ptr++; // Skip the null DIE itself
                        }
                         children_ptr = temp_child_ptr;
                    }
                }
            }
            
            size_t ret_len = strlen(return_type_str);
            size_t params_len = strlen(params_str);
            // Format: return_type (*)(param1, param2)
            res = malloc(ret_len + 6 + params_len + 1); // " (*)( )" is 6 chars + \0
            if (res) {
                sprintf(res, "%s (*)(%s)", return_type_str, params_str);
            } else {
                res = strdup("?subroutine_alloc_err?");
            }
            free(return_type_str);
            break;
        }
        default: {
            char default_type_str[64];
            snprintf(default_type_str, sizeof(default_type_str), "<?type_tag_0x%x%s%s?>",
                     ab->tag, type_name_attr ? ":" : "", type_name_attr ? type_name_attr : "");
            res = strdup(default_type_str);
            break;
        }
    }
    return res ? res : strdup("<?unknown?>");
}


/* Recursive DIE parsing */
void parse_die(CUContext *ctx, const uint8_t **p_die_ptr, const uint8_t *unit_end_ptr, int recursion_depth) {
    if (recursion_depth > 50) { // Safeguard for deep or erroneous DWARF
        fprintf(stderr, "Warning: Max recursion depth reached in parse_die.\n");
        // To prevent infinite loops, try to advance p_die_ptr past a potential bad DIE.
        // This is a guess; a robust solution would require more context or error recovery.
        if (*p_die_ptr < unit_end_ptr) (*p_die_ptr)++; 
        return;
    }

    const uint8_t *current_die_start = *p_die_ptr;
    uint64_t code = read_uleb(p_die_ptr);

    if (!code || *p_die_ptr > unit_end_ptr) { // 0 code is null DIE, marks end of siblings
        return;
    }

    Abbrev *ab = find_abbrev(ctx->cu_abbrev_list_head, code);
    if (!ab) {
        fprintf(stderr, "Warning: Could not find abbrev for code %lu at offset %ld. Skipping DIE.\n",
                code, (long)(current_die_start - ctx->debug_info_base));
        // Attempt to skip this DIE. This is hard without knowing its size.
        // If it has children, this won't work well.
        // For now, we might be stuck or misinterpret subsequent data.
        // A more robust parser would try to find the next sibling using DW_AT_sibling if available,
        // or rely on the null DIE terminator.
        return;
    }

    // Attributes for the current DIE
    const char *name = "", *comp_dir = "", *producer = "";
    uint64_t low_pc=0, high_pc=0;
    uint64_t decl_file_idx=0, decl_line_num=0; // Using original code's interpretation of 3b/3c
    uint64_t stmt_list_offset = 0;
    uint64_t type_offset = 0; // For return type of subprogram or type of variable
    int is_declaration_attr = 0; // From DW_AT_declaration attribute
    int has_frame_base = 0;      // Presence of DW_AT_frame_base

    // Store attribute values
   const uint8_t *attr_scan_ptr = *p_die_ptr; 
    for (AttrSpec *as = ab->attrs; as; as = as->next) {
        uint64_t val = read_form_val(&attr_scan_ptr, as->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, as->implicit_const_value);
        switch (as->name) {
            case DW_AT_name: name     = get_str_val(val, as->form, ctx->debug_str_base, ctx->debug_line_str_base); break;
            case DW_AT_comp_dir: comp_dir = get_str_val(val, as->form, ctx->debug_str_base, ctx->debug_line_str_base); break;
            case DW_AT_producer: producer = get_str_val(val, as->form, ctx->debug_str_base, ctx->debug_line_str_base); break;
            case DW_AT_low_pc: low_pc   = val; break; 
            case DW_AT_high_pc: high_pc  = val; break; 
            case DW_AT_decl_file: decl_file_idx = val; break; 
            case DW_AT_decl_line: decl_line_num = val; break; 
            case DW_AT_stmt_list: stmt_list_offset = val; break;
            case DW_AT_type: type_offset = val; break; 
            case DW_AT_declaration: is_declaration_attr = (val != 0); break;
            case DW_AT_frame_base: has_frame_base = 1; break; // Just note its presence
        }
    }
    *p_die_ptr = attr_scan_ptr;


    if (ab->tag == DW_TAG_compile_unit) {
        ctx->cu_main_src_name = name;
        ctx->cu_main_src_comp_dir = comp_dir;
        ctx->func_idx_in_cu = 0; // Reset function counter for this new CU

        printf("CompilationUnit %d:\n", ctx->cu_idx);

        char qualified_cu_name[1024];
        if (name && name[0] == '/') { // DW_AT_name is already an absolute path
            snprintf(qualified_cu_name, sizeof(qualified_cu_name), "%s", name);
        } else if (name && comp_dir && comp_dir[0] != '\0') { // DW_AT_name is relative, comp_dir exists
            snprintf(qualified_cu_name, sizeof(qualified_cu_name), "%s/%s", comp_dir, name);
        } else if (name) { // DW_AT_name exists, but no comp_dir or name is absolute
             snprintf(qualified_cu_name, sizeof(qualified_cu_name), "%s", name);
        } else {
            snprintf(qualified_cu_name, sizeof(qualified_cu_name), "<?unnamed_cu?>");
        }

        printf("Name: %s\n", qualified_cu_name);
        // Only print CompileDir if it's distinct or provides additional context not in Name
        if (comp_dir && (name && name[0] != '/' && comp_dir[0] != '\0')) {
             printf("CompileDir: %s\n", comp_dir);
        } else if (comp_dir && !name) { // If CU has no name but has comp_dir
             printf("CompileDir: %s\n", comp_dir);
        }
        printf("Producer: %s\n", producer ? producer : "");


        if (stmt_list_offset > 0 && ctx->debug_line_base) {
            parse_line_program_header(ctx, stmt_list_offset);
        }
        printf("\n");
    } else if (ab->tag == DW_TAG_subprogram) {
        ctx->func_idx_in_cu++; // Increment function counter
        char *return_type_str = resolve_type_to_string_recursive(ctx, type_offset, 0);

        // Collect parameter types
        char params_str[1024] = {0}; // Buffer for parameter type strings
        int first_param = 1;
        if (ab->has_children) {
            const uint8_t *children_ptr = *p_die_ptr; // Save current position
            // Temporarily parse children for parameters
            while (children_ptr < unit_end_ptr && *children_ptr != 0) {
                const uint8_t *param_die_start = children_ptr;
                uint64_t param_abbrev_code = read_uleb(&children_ptr);
                if (!param_abbrev_code) break;
                Abbrev *param_ab = find_abbrev(ctx->cu_abbrev_list_head, param_abbrev_code);
                if (!param_ab) {
                    // Try to skip this problematic child DIE to avoid infinite loops or further errors
                    // This is a basic skip; might not be perfect if child has children.
                    const uint8_t* next_sibling_guess = children_ptr;
                    // A simple heuristic: assume it's small or look for next null if it has no children itself.
                    // This part is tricky without full error recovery for abbrevs.
                    // For now, we just break from parameter parsing.
                    fprintf(stderr, "Warning: Could not find abbrev for parameter DIE code %lu. Parameter list may be incomplete.\n", param_abbrev_code);
                    break; 
                }

                uint64_t param_type_offset = 0;
                const char* param_name_attr = NULL;

                const uint8_t *param_attr_ptr = children_ptr;
                for (AttrSpec *pas = param_ab->attrs; pas; pas = pas->next) {
                    uint64_t pval = read_form_val(&param_attr_ptr, pas->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, pas->implicit_const_value);
                    if (pas->name == DW_AT_type) param_type_offset = pval;
                    if (pas->name == DW_AT_name) param_name_attr = get_str_val(pval, pas->form, ctx->debug_str_base, ctx->debug_line_str_base);
                }
                children_ptr = param_attr_ptr; // Advance past param attributes

                if (param_ab->tag == DW_TAG_formal_parameter) {
                    if (!first_param) strncat(params_str, ", ", sizeof(params_str) - strlen(params_str) - 1);
                    char *param_type_str = resolve_type_to_string_recursive(ctx, param_type_offset, 0);
                    strncat(params_str, param_type_str, sizeof(params_str) - strlen(params_str) - 1);
                    // Optionally add parameter name: if (param_name_attr) { ... }
                    free(param_type_str);
                    first_param = 0;
                }
                // Skip children of this parameter DIE if any (not typical for params)
                if (param_ab->has_children) {
                     const uint8_t *grand_children_ptr = children_ptr;
                     while (grand_children_ptr < unit_end_ptr && *grand_children_ptr != 0) {
                        uint64_t gc_code = read_uleb(&grand_children_ptr);
                        if (!gc_code) break;
                        Abbrev *gc_ab = find_abbrev(ctx->cu_abbrev_list_head, gc_code);
                        if (!gc_ab) { grand_children_ptr++; continue;} // Basic skip
                        for(AttrSpec* gc_as = gc_ab->attrs; gc_as; gc_as = gc_as->next) {
                            read_form_val(&grand_children_ptr, gc_as->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, gc_as->implicit_const_value);
                        }
                        if (gc_ab->has_children) { // If param's child itself has children
                             while(grand_children_ptr < unit_end_ptr && *grand_children_ptr != 0) { // Skip until null DIE for gc's children
                                 // This is a simplified skip, might need full parsing to be accurate
                                 uint64_t ggc_code = read_uleb(&grand_children_ptr);
                                 if(!ggc_code) break;
                                 Abbrev *ggc_ab = find_abbrev(ctx->cu_abbrev_list_head, ggc_code);
                                 if(!ggc_ab) {grand_children_ptr++; continue;}
                                 for(AttrSpec* ggc_as = ggc_ab->attrs; ggc_as; ggc_as = ggc_as->next) {
                                     read_form_val(&grand_children_ptr, ggc_as->form, ctx->cu_address_size, ctx->cu_compilation_unit_base, ggc_as->implicit_const_value);
                                 }
                                 if(ggc_ab->has_children && grand_children_ptr < unit_end_ptr && *grand_children_ptr !=0) grand_children_ptr++; // Skip children's null terminator
                             }
                        }
                         if (grand_children_ptr < unit_end_ptr && *grand_children_ptr == 0) grand_children_ptr++; // Skip null terminator of param's children's children list
                     }
                     children_ptr = grand_children_ptr;
                }
            }
        }


        const char *file_str = "<?unknown_file?>";
        if (decl_file_idx > 0 && decl_file_idx <= ctx->num_file_entries) {
            FileEntry *fe = &ctx->file_entries[decl_file_idx - 1]; 
            if (fe->dir_index > 0 && fe->dir_index <= ctx->num_include_directories) {
                static char full_path[512]; 
                snprintf(full_path, sizeof(full_path), "%s/%s",
                         ctx->include_directories[fe->dir_index - 1], fe->name);
                file_str = full_path;
            } else {
                file_str = fe->name;
            }
        } else if (decl_file_idx == 1 && ctx->num_file_entries == 0 && ctx->cu_main_src_name) {
            // Fallback: Use CU's main source file name if decl_file_idx is 1 and .debug_line is empty
            static char full_path_cu[512];
            if (ctx->cu_main_src_comp_dir && ctx->cu_main_src_comp_dir[0] != '\0') {
                snprintf(full_path_cu, sizeof(full_path_cu), "%s/%s", ctx->cu_main_src_comp_dir, ctx->cu_main_src_name);
            } else {
                snprintf(full_path_cu, sizeof(full_path_cu), "%s", ctx->cu_main_src_name);
            }
            file_str = full_path_cu;
        } else if (decl_file_idx > 0) {
             static char missing_file_idx_str[128]; // Increased buffer size
             snprintf(missing_file_idx_str, sizeof(missing_file_idx_str), "<?invalid_file_idx_%lu (num_files:%lu)?>", decl_file_idx, ctx->num_file_entries);
             file_str = missing_file_idx_str;
        }

        int is_definition = 0;
        if (is_declaration_attr) {
            is_definition = 0; // DW_AT_declaration explicitly says it's a declaration
        } else {
            // No DW_AT_declaration, or it's false.
            // It's a definition if it has a low_pc, high_pc, or frame_base.
            if (low_pc != 0 || high_pc != 0 || has_frame_base) {
                is_definition = 1;
            } else {
                // Lacks explicit declaration flag and also lacks common definition indicators.
                // Treat as declaration (e.g. external function not defined in this CU).
                is_definition = 0;
            }
        }

        if (name && strcmp(name, "main") == 0 && !is_definition && !is_declaration_attr) {
            // Special heuristic for 'main' if it wasn't caught as a definition
            // and wasn't explicitly a DW_AT_declaration.
            // This is to match example outputs that might assume 'main' is always a definition
            // if it's the primary entry point in its own CU.
            // Be cautious with such heuristics as they override strict DWARF interpretation.
            // is_definition = 1; // Uncomment to apply heuristic
        }


        printf("%d. %s %s(%s) : %s(%lu) %s\n", // Added ctx->func_idx_in_cu
               ctx->func_idx_in_cu,
               return_type_str ? return_type_str : "void",
               name ? name : "<?unnamed_func?>",
               params_str,
               file_str, decl_line_num,
               is_definition ? "(definition)" : "(declaration)");
        free(return_type_str);
    }

    if (ab->has_children) {
        const uint8_t *children_processing_ptr = *p_die_ptr;
        while (children_processing_ptr < unit_end_ptr && *children_processing_ptr != 0) { 
            parse_die(ctx, &children_processing_ptr, unit_end_ptr, recursion_depth + 1);
        }
        if (children_processing_ptr < unit_end_ptr && *children_processing_ptr == 0) { 
            children_processing_ptr++;
        }
        *p_die_ptr = children_processing_ptr; // Update main die pointer
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <elf_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (elf_version(EV_CURRENT) == EV_NONE) {
        fprintf(stderr, "Error: libelf initialization failed: %s\n", elf_errmsg(-1));
        return EXIT_FAILURE;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return EXIT_FAILURE; }
    Elf *e = elf_begin(fd, ELF_C_READ, NULL);
    if (!e) {
        fprintf(stderr, "Error: elf_begin failed: %s\n", elf_errmsg(-1));
        close(fd);
        return EXIT_FAILURE;
    }

    size_t shstrndx;
    if (elf_getshdrstrndx(e, &shstrndx) != 0) {
        fprintf(stderr, "Error: elf_getshdrstrndx failed: %s\n", elf_errmsg(-1));
        elf_end(e); close(fd); return EXIT_FAILURE;
    }

    const uint8_t *debug_info_data=NULL, *debug_abbrev_data=NULL, *debug_str_data=NULL, *debug_line_data=NULL, *debug_line_str_data=NULL;
    size_t info_sz=0, abbrev_sz=0, str_sz=0, line_sz=0, line_str_sz=0;

    Elf_Scn *scn = NULL;
    while ((scn = elf_nextscn(e, scn)) != NULL) {
        GElf_Shdr sh;
        if (gelf_getshdr(scn, &sh) == NULL) {
            fprintf(stderr, "Warning: gelf_getshdr failed: %s\n", elf_errmsg(-1));
            continue;
        }
        char *name = elf_strptr(e, shstrndx, sh.sh_name);
        if (!name) continue;

        Elf_Data *d = elf_getdata(scn, NULL);
        if (!d) continue;

        if (!strcmp(name, ".debug_info"))       { debug_info_data = d->d_buf; info_sz   = d->d_size; }
        else if (!strcmp(name, ".debug_abbrev")) { debug_abbrev_data = d->d_buf; abbrev_sz = d->d_size; }
        else if (!strcmp(name, ".debug_str"))    { debug_str_data = d->d_buf; str_sz    = d->d_size; }
        else if (!strcmp(name, ".debug_line"))   { debug_line_data = d->d_buf; line_sz   = d->d_size; }
        else if (!strcmp(name, ".debug_line_str")) { debug_line_str_data = d->d_buf; line_str_sz = d->d_size; }
    }

    if (!debug_info_data || !debug_abbrev_data || !debug_str_data) {
        fprintf(stderr, "Error: Essential DWARF sections (.debug_info, .debug_abbrev, .debug_str) missing.\nCompile with -g.\n");
        elf_end(e); close(fd); return EXIT_FAILURE;
    }
    if (!debug_line_data) {
        fprintf(stderr, "Warning: .debug_line section missing. File and line information for functions might be incomplete.\n");
    }

    const uint8_t *p_cu_iterator = debug_info_data; // Use a distinct name for the main CU iterator
    const uint8_t *debug_info_end = debug_info_data + info_sz;
    int cu_count = 0;

    while (p_cu_iterator < debug_info_end) {
        CUContext ctx = {0}; // Fresh context for each CU
        
        ctx.debug_info_base = debug_info_data;
        ctx.debug_str_base = debug_str_data;
        ctx.debug_line_str_base = debug_line_str_data;
        ctx.debug_line_base = debug_line_data;
        ctx.debug_abbrev_base = debug_abbrev_data;
        ctx.debug_abbrev_size = abbrev_sz;
        ctx.cu_idx = ++cu_count;

        // --- Start CU Header Parsing ---
        const uint8_t *cu_header_start = p_cu_iterator;
        ctx.cu_compilation_unit_base = cu_header_start; // Base for CU-relative offsets (like DW_FORM_ref4)

        ctx.cu_length = le32toh(*(uint32_t*)p_cu_iterator); p_cu_iterator += 4;
        const uint8_t *next_cu_header_start = cu_header_start + 4 + ctx.cu_length; // Boundary for this CU's data
        
        if (next_cu_header_start > debug_info_end) {
            fprintf(stderr, "Error: CU %d length (0x%x) exceeds .debug_info section bounds.\n", ctx.cu_idx, ctx.cu_length);
            break;
        }

        ctx.cu_version = le16toh(*(uint16_t*)p_cu_iterator); p_cu_iterator += 2;
        
        // DWARF 5 has a different header format than DWARF 2-4
        if (ctx.cu_version >= 5) {
            // DWARF 5 format: unit_type (1 byte), address_size (1 byte), abbrev_offset (4 bytes)
            uint8_t unit_type = *p_cu_iterator++; // Usually DW_UT_compile (1)
            ctx.cu_address_size = *p_cu_iterator++;
            ctx.cu_abbrev_offset = le32toh(*(uint32_t*)p_cu_iterator); p_cu_iterator += 4;
        } else {
            // DWARF 2-4 format: abbrev_offset (4 bytes), address_size (1 byte)
            ctx.cu_abbrev_offset = le32toh(*(uint32_t*)p_cu_iterator); p_cu_iterator += 4;
            ctx.cu_address_size = *p_cu_iterator++;
        }
        ctx.cu_die_start = p_cu_iterator; // p_cu_iterator now points to the first DIE's abbrev code
        // --- End CU Header Parsing ---

        ctx.cu_abbrev_list_head = parse_cu_abbrevs(ctx.debug_abbrev_base, ctx.debug_abbrev_size, ctx.cu_abbrev_offset);
        if (!ctx.cu_abbrev_list_head && ctx.cu_abbrev_offset != 0 && abbrev_sz > 0) {
            fprintf(stderr, "Warning: Failed to parse abbrevs for CU %d at offset 0x%x. Subsequent DIE parsing may fail.\n", ctx.cu_idx, ctx.cu_abbrev_offset);
        }

        // p_cu_iterator is currently at the start of the first DIE's abbrev code.
        // parse_die will advance this pointer.
        const uint8_t *current_die_processing_ptr = ctx.cu_die_start; 

        // Iterate over top-level DIEs in this CU
        // The end condition is tricky: it's either the end of the CU's data (next_cu_header_start)
        // or a null DIE entry if the CU data is padded.
        // parse_die handles advancing current_die_processing_ptr.
        // The first DIE *is* the DW_TAG_compile_unit.
        if (current_die_processing_ptr < next_cu_header_start) {
             parse_die(&ctx, &current_die_processing_ptr, next_cu_header_start, 0);
        }
        // After parse_die processes the DW_TAG_compile_unit and all its children,
        // current_die_processing_ptr should point to the end of the CU's DIEs or next_cu_header_start.

        // Cleanup context allocations for this CU
        if (ctx.include_directories) {
            for (uint64_t i = 0; i < ctx.num_include_directories; ++i) free(ctx.include_directories[i]);
            free(ctx.include_directories);
        }
        if (ctx.file_entries) {
            for (uint64_t i = 0; i < ctx.num_file_entries; ++i) free(ctx.file_entries[i].name);
            free(ctx.file_entries);
        }
        free_abbrev_list(ctx.cu_abbrev_list_head);
        ctx.cu_abbrev_list_head = NULL;
        
        // Ensure the main iterator advances to the start of the next CU
        p_cu_iterator = next_cu_header_start;
    }

    elf_end(e);
    close(fd);
    return EXIT_SUCCESS;
}