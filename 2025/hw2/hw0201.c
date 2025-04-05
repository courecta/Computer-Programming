#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_CUES 1000
#define MAX_TEXT_LEN 1024

typedef struct {
    int time;         // normalized time in milliseconds
    char text[MAX_TEXT_LEN];
} Cue;

// Round a double to the nearest integer.
int round_ms(double ms) {
    return (int)round(ms);
}

// Skip whitespace characters.
char *skip_whitespace(char *s) {
    while (*s && isspace((unsigned char)*s))
        s++;
    return s;
}

// Extract a complete JSON block by matching brackets.
// It assumes the starting character is '{' or '['.
char *extract_json_block(const char *start) {
    char open = *start;
    char close = (open == '{') ? '}' : (open == '[' ? ']' : '\0');
    if (close == '\0')
        return NULL;
    
    const char *ptr = start;
    int count = 0;
    while (*ptr) {
        if (*ptr == open)
            count++;
        else if (*ptr == close) {
            count--;
            if (count == 0) {
                ptr++; // include the closing bracket
                break;
            }
        }
        ptr++;
    }
    size_t len = ptr - start;
    char *jsonBlock = malloc(len + 1);
    if (!jsonBlock) return NULL;
    strncpy(jsonBlock, start, len);
    jsonBlock[len] = '\0';
    return jsonBlock;
}

// Revised parser: Only process objects that are cues by looking for the cue marker.
int parse_cues(const char *json, Cue cues[], int maxCues) {
    int count = 0;
    const char *ptr = json;
    const char *cueMarker = "\"__typename\":\"Cue\"";
    while ((ptr = strstr(ptr, cueMarker)) != NULL && count < maxCues) {
        ptr += strlen(cueMarker);
        
        // Find "time": key within this cue object.
        const char *timeKey = strstr(ptr, "\"time\":");
        if (!timeKey)
            break;
        timeKey += strlen("\"time\":");
        timeKey = skip_whitespace((char *)timeKey);
        double time_val = atof(timeKey);
        cues[count].time = round_ms(time_val);

        // Find "text": key within this cue object.
        const char *textKey = strstr(ptr, "\"text\":");
        if (!textKey)
            break;
        textKey += strlen("\"text\":");
        textKey = skip_whitespace((char *)textKey);
        if (*textKey != '\"')
            break;
        textKey++; // skip the opening quote

        char rawText[MAX_TEXT_LEN] = {0};
        int i = 0;
        while (*textKey && *textKey != '\"' && i < MAX_TEXT_LEN - 1) {
            if (*textKey == '\\') {
                textKey++;
                if (*textKey == 'n') {
                    rawText[i++] = '\n';
                } else if (*textKey == 't') {
                    rawText[i++] = '\t';
                } else {
                    rawText[i++] = *textKey;
                }
            } else {
                rawText[i++] = *textKey;
            }
            textKey++;
        }
        rawText[i] = '\0';
        strncpy(cues[count].text, rawText, MAX_TEXT_LEN - 1);
        cues[count].text[MAX_TEXT_LEN - 1] = '\0';
        
        count++;
        ptr = textKey;
    }
    return count;
}

// Convert milliseconds to timestamp "HH:MM:SS,mmm".
void ms_to_timestamp(int ms, char *buffer, size_t bufsize, const char *delimiter) {
    int hours, minutes, seconds, milliseconds;
    hours = ms / (3600 * 1000);
    ms %= 3600 * 1000;
    minutes = ms / (60 * 1000);
    ms %= 60 * 1000;
    seconds = ms / 1000;
    milliseconds = ms % 1000;
    snprintf(buffer, bufsize, "%02d:%02d:%02d%s%03d", hours, minutes, seconds, delimiter, milliseconds);
}

int main(void) {
    char inputFileName[256], outputFileName[256];
    
    printf("Please enter the TED talk source HTML file: ");
    if (scanf("%255s", inputFileName) != 1) {
        fprintf(stderr, "Error reading input filename.\n");
        return 1;
    }
    printf("Please enter the subtitle output file name: ");
    if (scanf("%255s", outputFileName) != 1) {
        fprintf(stderr, "Error reading output filename.\n");
        return 1;
    }
    
    // Read the entire input HTML file.
    FILE *inFile = fopen(inputFileName, "rb");
    if (!inFile) {
        fprintf(stderr, "Could not open input file %s\n", inputFileName);
        return 1;
    }
    fseek(inFile, 0, SEEK_END);
    long fileSize = ftell(inFile);
    rewind(inFile);
    char *buffer = malloc(fileSize + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(inFile);
        return 1;
    }
    fread(buffer, 1, fileSize, inFile);
    buffer[fileSize] = '\0';
    fclose(inFile);
    
    // Find the "transcriptData": key in the file.
    char *transcriptPtr = strstr(buffer, "\"transcriptData\":");
    if (!transcriptPtr) {
        fprintf(stderr, "Could not find transcriptData in the file.\n");
        free(buffer);
        return 1;
    }
    
    // Capture initial text between "transcriptData": and the first JSON brace.
    char *tempPtr = transcriptPtr;
    while (*tempPtr && *tempPtr != '{' && *tempPtr != '[')
        tempPtr++;
    size_t initTextLen = tempPtr - transcriptPtr;
    char *initText = malloc(initTextLen + 1);
    if (!initText) {
        free(buffer);
        return 1;
    }
    strncpy(initText, transcriptPtr, initTextLen);
    initText[initTextLen] = '\0';
    // Trim leading whitespace and punctuation.
    char *trimmedInitText = skip_whitespace(initText);
    while (*trimmedInitText && ispunct((unsigned char)*trimmedInitText))
        trimmedInitText++;
    // Remove the key by finding the colon and skipping it.
    char *colonPos = strchr(trimmedInitText, ':');
    if (colonPos) {
        colonPos++; // skip colon
        colonPos = skip_whitespace(colonPos);
        trimmedInitText = colonPos;
    }
    
    // Extract the JSON block starting at tempPtr.
    char *jsonBlock = extract_json_block(tempPtr);
    if (!jsonBlock) {
        fprintf(stderr, "Failed to extract JSON block from transcriptData.\n");
        free(buffer);
        free(initText);
        return 1;
    }
    
    // Parse cues using the revised parser (cue marker based).
    Cue cues[MAX_CUES];
    int cueCount = parse_cues(jsonBlock, cues, MAX_CUES);
    if (cueCount == 0) {
        fprintf(stderr, "No cues found in transcriptData.\n");
        free(jsonBlock);
        free(buffer);
        free(initText);
        return 1;
    }
    
    // Prepend the initial text (if nonempty) to the first cue if it's not already there.
    if (trimmedInitText && strlen(trimmedInitText) > 0) {
        if (strncmp(cues[0].text, trimmedInitText, strlen(trimmedInitText)) != 0) {
            char combined[MAX_TEXT_LEN];
            snprintf(combined, MAX_TEXT_LEN, "%s %s", trimmedInitText, cues[0].text);
            strncpy(cues[0].text, combined, MAX_TEXT_LEN - 1);
            cues[0].text[MAX_TEXT_LEN - 1] = '\0';
        }
    }
    free(initText);
    
    // Normalize timestamps: subtract the time of the first cue.
    int offset = cues[0].time;
    for (int i = 0; i < cueCount; i++) {
        cues[i].time -= offset;
        if (cues[i].time < 0)
            cues[i].time = 0;
    }
    
    // Open the output SRT file.
    FILE *outFile = fopen(outputFileName, "w");
    if (!outFile) {
        fprintf(stderr, "Could not open output file %s\n", outputFileName);
        free(jsonBlock);
        free(buffer);
        return 1;
    }
    
    const char *timeDelimiter = ","; // SRT standard uses a comma.
    for (int i = 0; i < cueCount; i++) {
        int startTime = cues[i].time;
        int endTime = (i < cueCount - 1) ? (cues[i + 1].time - 1) : (cues[i].time + 5000);
        char startTimestamp[16], endTimestamp[16];
        ms_to_timestamp(startTime, startTimestamp, sizeof(startTimestamp), timeDelimiter);
        ms_to_timestamp(endTime, endTimestamp, sizeof(endTimestamp), timeDelimiter);
        
        fprintf(outFile, "%d\n", i + 1);
        fprintf(outFile, "%s --> %s\n", startTimestamp, endTimestamp);
        fprintf(outFile, "%s\n\n", cues[i].text);
    }
    
    printf("Subtitle file '%s' generated with %d subtitles.\n", outputFileName, cueCount);
    
    free(jsonBlock);
    free(buffer);
    fclose(outFile);
    return 0;
}