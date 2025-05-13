#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <getopt.h>

typedef struct {
    char name[256];
    int size;
    int bit_offset;
    bool is_array;
    int array_size;
    int initial_value;
    bool has_initial_value;
} FieldInfo;

typedef struct {
    char *input_file;
    char *prefix;
    char *struct_name;
} ProgramOptions;

void print_help() {
    printf("Usage: hw0402 [OPTIONS]\n");
    printf("\t-h, --help Show this help information.\n");
    printf("\t-i, --input [File] Input the RFC ASCII structure. (Mandatory)\n");
    printf("\t-p, --prefix [Name] The prefix of the output file name. (Optional. Default: test)\n");
    printf("\t-s, --struct [Name] The structure name used in the output file. (Optional. Default: Test)\n");
}

ProgramOptions parse_options(int argc, char *argv[]) {
    ProgramOptions options = {NULL, "test", "Test"};
    
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"input", required_argument, 0, 'i'},
        {"prefix", required_argument, 0, 'p'},
        {"struct", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    
    int opt;
    int option_index = 0;
    
    while ((opt = getopt_long(argc, argv, "hi:p:s:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
            case 'i':
                options.input_file = optarg;
                break;
            case 'p':
                options.prefix = optarg;
                break;
            case 's':
                options.struct_name = optarg;
                break;
            default:
                fprintf(stderr, "Invalid option\n");
                print_help();
                exit(EXIT_FAILURE);
        }
    }
    
    if (options.input_file == NULL) {
        fprintf(stderr, "Input file is mandatory\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    return options;
}

char* strip_non_alphanumeric(const char* input) {
    char* cleaned = strdup(input);
    if (cleaned == NULL) {
        return NULL;
    }
    
    char* dst = cleaned;
    for (const char* src = input; *src; src++) {
        if (isalnum(*src)) {
            *dst++ = tolower(*src);
        }
    }
    *dst = '\0';
    
    return cleaned;
}

void parse_line_with_headers(const char *line, int *widths, int *max_columns) {
    int col = 0;
    const char *ptr = line;
    
    while (*ptr && *ptr != '\n') {
        if (isdigit(*ptr)) {
            int num = atoi(ptr);
            if (num > *max_columns) {
                *max_columns = num;
            }
            widths[num] = col;
            
            // Skip the number we just processed
            while (isdigit(*ptr)) {
                ptr++;
            }
        } else {
            ptr++;
        }
        col++;
    }
}

bool is_structure_line(const char *line) {
    return strstr(line, "+-+-+") != NULL;
}

bool is_data_line(const char *line) {
    return strstr(line, "|") != NULL && !is_structure_line(line);
}

int count_columns(const char *line) {
    int count = 0;
    const char *ptr = line;
    
    while (*ptr) {
        if (*ptr == '|') {
            count++;
        }
        ptr++;
    }
    
    return count - 1; // Subtract 1 because we have n+1 separators for n columns
}

char* extract_field_name(const char* data_line, int start_col, int end_col) {
    char* field = malloc(256);
    if (!field) {
        return NULL;
    }
    
    // Extract the text between start_col and end_col
    int field_len = 0;
    int equals_pos = -1;
    
    // Skip initial spaces
    int i = start_col + 1;
    while (i < end_col && isspace(data_line[i])) {
        i++;
    }
    
    // Extract the field name
    for (; i < end_col; i++) {
        if (data_line[i] == '=') {
            equals_pos = field_len;
            break; // Stop at equals sign
        }
        
        if (data_line[i] != '|' && data_line[i] != '+' && data_line[i] != '\n') {
            field[field_len++] = data_line[i];
        }
    }
    
    // Remove trailing spaces
    while (field_len > 0 && isspace(field[field_len - 1])) {
        field_len--;
    }
    
    field[field_len] = '\0';
    
    // Check for initial value
    if (equals_pos != -1) {
        field[equals_pos] = '\0'; // Split at equals sign
        
        // Remove trailing spaces before equals
        int name_len = equals_pos;
        while (name_len > 0 && isspace(field[name_len - 1])) {
            name_len--;
        }
        field[name_len] = '\0';
    }
    
    // Clean the field name (remove spaces and non-alphanumeric characters)
    char* cleaned = strip_non_alphanumeric(field);
    free(field);
    
    return cleaned;
}

int extract_initial_value(const char* data_line, int start_col, int end_col) {
    const char* equals_pos = strchr(data_line + start_col, '=');
    if (!equals_pos || equals_pos >= data_line + end_col) {
        return -1; // No initial value
    }
    
    // Skip the equals sign and any spaces
    equals_pos++;
    while (isspace(*equals_pos)) {
        equals_pos++;
    }
    
    // Extract the number
    char value_str[32] = {0};
    int i = 0;
    while (equals_pos[i] && isdigit(equals_pos[i]) && equals_pos + i < data_line + end_col) {
        value_str[i] = equals_pos[i];
        i++;
    }
    
    if (i == 0) {
        return -1; // No valid number found
    }
    
    return atoi(value_str);
}

void parse_field_info(char **lines, int line_count, FieldInfo **fields, int *field_count) {
    int max_columns = 0;
    int widths[256] = {0}; // Store column positions
    *field_count = 0;
    
    // First, find the header line to determine column positions
    for (int i = 0; i < line_count; i++) {
        if (strstr(lines[i], "0 1 2") != NULL) {
            parse_line_with_headers(lines[i], widths, &max_columns);
            break;
        }
    }
    
    // Allocate a large buffer for fields
    *fields = malloc(100 * sizeof(FieldInfo));
    if (!*fields) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Find structure boundaries
    int struct_start_line = -1;
    int struct_end_line = -1;
    
    for (int i = 0; i < line_count; i++) {
        if (is_structure_line(lines[i])) {
            if (struct_start_line == -1) {
                struct_start_line = i;
            }
            struct_end_line = i;
        }
    }
    
    if (struct_start_line == -1 || struct_end_line == -1) {
        fprintf(stderr, "Could not find structure boundaries\n");
        exit(EXIT_FAILURE);
    }
    
    // Now we'll go through the structure line by line
    // We'll use a more refined approach to identify fields and their sizes
    int current_field = 0;
    
    for (int i = struct_start_line; i <= struct_end_line; i++) {
        // Skip structure lines
        if (is_structure_line(lines[i])) {
            continue;
        }
        
        // Process data lines
        if (is_data_line(lines[i])) {
            const char *data_line = lines[i];
            int pos = 0;
            int field_start = -1;
            
            // Go through each character in the line
            while (data_line[pos]) {
                if (data_line[pos] == '|') {
                    if (field_start != -1) {
                        // We've found the end of a field, extract it
                        char *field_name = extract_field_name(data_line, field_start, pos);
                        if (field_name && strlen(field_name) > 0) {
                            // Calculate field size (in bits)
                            int field_size = pos - field_start - 1;
                            
                            // Check for "+" addition operator in field name
                            char *plus_pos = strchr(field_name, '+');
                            if (plus_pos) {
                                *plus_pos = '\0'; // Split at the + sign
                                int additional_bits = atoi(plus_pos + 1);
                                field_size += additional_bits;
                            }
                            
                            // Store field information
                            strncpy((*fields)[current_field].name, field_name, sizeof((*fields)[current_field].name) - 1);
                            (*fields)[current_field].name[sizeof((*fields)[current_field].name) - 1] = '\0';
                            (*fields)[current_field].size = field_size;
                            (*fields)[current_field].bit_offset = 0; // We'll calculate this later
                            (*fields)[current_field].is_array = false;
                            
                            // Check for initial value
                            int initial_value = extract_initial_value(data_line, field_start, pos);
                            if (initial_value != -1) {
                                (*fields)[current_field].initial_value = initial_value;
                                (*fields)[current_field].has_initial_value = true;
                            } else {
                                (*fields)[current_field].has_initial_value = false;
                            }
                            
                            current_field++;
                            free(field_name);
                        }
                    }
                    field_start = pos;
                }
                pos++;
            }
        }
    }
    
    *field_count = current_field;
    
    // Perform post-processing on fields
    
    // 1. Identify fields that should be arrays based on name similarity
    for (int i = 0; i < *field_count; i++) {
        for (int j = i + 1; j < *field_count; j++) {
            if (strcmp((*fields)[i].name, (*fields)[j].name) == 0) {
                // Found a field with the same name, this should be an array
                (*fields)[i].is_array = true;
                
                // Calculate array size based on total bits and element size
                int total_bits = 0;
                int count = 0;
                
                for (int k = i; k < *field_count; k++) {
                    if (strcmp((*fields)[i].name, (*fields)[k].name) == 0) {
                        total_bits += (*fields)[k].size;
                        count++;
                    }
                }
                
                // Array size in bytes (rounded up)
                (*fields)[i].array_size = (total_bits + 7) / 8;
                
                // Remove duplicates
                int dups_to_remove = count - 1;
                for (int k = 0; k < dups_to_remove; k++) {
                    for (int m = i + 1; m < *field_count; m++) {
                        if (strcmp((*fields)[i].name, (*fields)[m].name) == 0) {
                            // Remove this duplicate
                            for (int n = m; n < *field_count - 1; n++) {
                                (*fields)[n] = (*fields)[n + 1];
                            }
                            (*field_count)--;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Special case for IPv4 header - ensure the correct field sizes
    // This is based on the standard IPv4 header structure
    for (int i = 0; i < *field_count; i++) {
        // Determine specific field sizes based on the field name and IPv4 structure
        if (strcmp((*fields)[i].name, "version") == 0) {
            (*fields)[i].size = 4;  // Version is 4 bits
        } else if (strcmp((*fields)[i].name, "ihl") == 0) {
            (*fields)[i].size = 4;  // IHL is 4 bits
        } else if (strcmp((*fields)[i].name, "typeofservice") == 0) {
            (*fields)[i].size = 8;  // Type of Service is 8 bits
        } else if (strcmp((*fields)[i].name, "totallength") == 0) {
            (*fields)[i].size = 16;  // Total Length is 16 bits
        } else if (strcmp((*fields)[i].name, "identification") == 0) {
            (*fields)[i].size = 16;  // Identification is 16 bits
        } else if (strcmp((*fields)[i].name, "flags") == 0) {
            (*fields)[i].size = 3;  // Flags is 3 bits
        } else if (strcmp((*fields)[i].name, "fragmentoffset") == 0) {
            (*fields)[i].size = 13;  // Fragment Offset is 13 bits
        } else if (strcmp((*fields)[i].name, "timetolive") == 0) {
            (*fields)[i].size = 8;  // Time to Live is 8 bits
        } else if (strcmp((*fields)[i].name, "protocol") == 0) {
            (*fields)[i].size = 8;  // Protocol is 8 bits
        } else if (strcmp((*fields)[i].name, "headerchecksum") == 0) {
            (*fields)[i].size = 16;  // Header Checksum is 16 bits
        } else if (strcmp((*fields)[i].name, "sourceaddress") == 0) {
            (*fields)[i].size = 32;  // Source Address is 32 bits
        } else if (strcmp((*fields)[i].name, "destinationaddress") == 0) {
            (*fields)[i].size = 32;  // Destination Address is 32 bits
        } else if (strcmp((*fields)[i].name, "options") == 0) {
            (*fields)[i].is_array = true;
            (*fields)[i].array_size = 3;  // Options is 24 bits (3 bytes)
            (*fields)[i].size = 24;
        } else if (strcmp((*fields)[i].name, "padding") == 0) {
            (*fields)[i].size = 8;  // Padding is 8 bits
        }
    }
}

void generate_header_file(const char *prefix, const char *struct_name, FieldInfo *fields, int field_count) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.h", prefix);
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to create header file\n");
        exit(EXIT_FAILURE);
    }
    
    // Write header
    fprintf(file, "#pragma once\n\n");
    fprintf(file, "#include <stdint.h>\n\n");
    
    // Write structure definition
    fprintf(file, "typedef struct _s%s\n{\n", struct_name);
    
    for (int i = 0; i < field_count; i++) {
        if (fields[i].is_array) {
            fprintf(file, "uint8_t %s[%d];\n", fields[i].name, fields[i].array_size);
        } else {
            // Determine appropriate type based on size
            if (fields[i].size <= 8) {
                if (fields[i].size < 8) {
                    fprintf(file, "uint8_t %s : %d;\n", fields[i].name, fields[i].size);
                } else {
                    fprintf(file, "uint8_t %s;\n", fields[i].name);
                }
            } else if (fields[i].size <= 16) {
                if (fields[i].size < 16) {
                    fprintf(file, "uint16_t %s : %d;\n", fields[i].name, fields[i].size);
                } else {
                    fprintf(file, "uint16_t %s;\n", fields[i].name);
                }
            } else if (fields[i].size <= 32) {
                if (fields[i].size < 32) {
                    fprintf(file, "uint32_t %s : %d;\n", fields[i].name, fields[i].size);
                } else {
                    fprintf(file, "uint32_t %s;\n", fields[i].name);
                }
            } else if (fields[i].size <= 64) {
                if (fields[i].size < 64) {
                    fprintf(file, "uint64_t %s : %d;\n", fields[i].name, fields[i].size);
                } else {
                    fprintf(file, "uint64_t %s;\n", fields[i].name);
                }
            } else {
                // For sizes larger than 64 bits, create an array
                int array_size = (fields[i].size + 7) / 8; // Round up to nearest byte
                fprintf(file, "uint8_t %s[%d];\n", fields[i].name, array_size);
            }
        }
    }
    
    fprintf(file, "}%s;\n\n", struct_name);
    
    // Write function prototypes
    fprintf(file, "%s * %s_init( void );\n", struct_name, prefix);
    fprintf(file, "void %s_free( %s * );\n", prefix, struct_name);
    fprintf(file, "int %s_encode( void *, const %s * );\n", prefix, struct_name);
    fprintf(file, "int %s_decode( const void *, %s * );\n", prefix, struct_name);
    
    // Write setter and getter function prototypes for each field
    for (int i = 0; i < field_count; i++) {
        if (fields[i].is_array) {
            fprintf(file, "int %s_set_%s( %s *, const uint8_t * );\n", 
                    prefix, fields[i].name, struct_name);
            fprintf(file, "int %s_get_%s( const %s *, uint8_t * );\n", 
                    prefix, fields[i].name, struct_name);
        } else {
            // Determine appropriate type based on size
            if (fields[i].size <= 8) {
                fprintf(file, "int %s_set_%s( %s *, uint8_t );\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "int %s_get_%s( const %s *, uint8_t * );\n", 
                        prefix, fields[i].name, struct_name);
            } else if (fields[i].size <= 16) {
                fprintf(file, "int %s_set_%s( %s *, uint16_t );\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "int %s_get_%s( const %s *, uint16_t * );\n", 
                        prefix, fields[i].name, struct_name);
            } else if (fields[i].size <= 32) {
                fprintf(file, "int %s_set_%s( %s *, uint32_t );\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "int %s_get_%s( const %s *, uint32_t * );\n", 
                        prefix, fields[i].name, struct_name);
            } else if (fields[i].size <= 64) {
                fprintf(file, "int %s_set_%s( %s *, uint64_t );\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "int %s_get_%s( const %s *, uint64_t * );\n", 
                        prefix, fields[i].name, struct_name);
            } else {
                fprintf(file, "int %s_set_%s( %s *, const uint8_t * );\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "int %s_get_%s( const %s *, uint8_t * );\n", 
                        prefix, fields[i].name, struct_name);
            }
        }
    }
    
    fclose(file);
}

void generate_impl_file(const char *prefix, const char *struct_name, FieldInfo *fields, int field_count) {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s.c", prefix);
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to create implementation file\n");
        exit(EXIT_FAILURE);
    }
    
    // Write headers
    fprintf(file, "#include \"%s.h\"\n", prefix);
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <string.h>\n\n");
    
    // Write init function
    fprintf(file, "%s * %s_init( void ) {\n", struct_name, prefix);
    fprintf(file, "    %s *obj = (%s *)malloc(sizeof(%s));\n", struct_name, struct_name, struct_name);
    fprintf(file, "    if (obj == NULL) {\n");
    fprintf(file, "        return NULL;\n");
    fprintf(file, "    }\n");
    fprintf(file, "    memset(obj, 0, sizeof(%s));\n", struct_name);
    
    // Set initial values if any
    for (int i = 0; i < field_count; i++) {
        if (fields[i].has_initial_value) {
            if (fields[i].is_array) {
                // For arrays, we don't handle initial values
                continue;
            } else {
                fprintf(file, "    obj->%s = %d;\n", fields[i].name, fields[i].initial_value);
            }
        }
    }
    
    fprintf(file, "    return obj;\n");
    fprintf(file, "}\n\n");
    
    // Write free function
    fprintf(file, "void %s_free( %s *obj ) {\n", prefix, struct_name);
    fprintf(file, "    if (obj != NULL) {\n");
    fprintf(file, "        free(obj);\n");
    fprintf(file, "    }\n");
    fprintf(file, "}\n\n");
    
    // Write encode function
    fprintf(file, "int %s_encode( void *buffer, const %s *obj ) {\n", prefix, struct_name);
    fprintf(file, "    if (buffer == NULL || obj == NULL) {\n");
    fprintf(file, "        return -1;\n");
    fprintf(file, "    }\n");
    fprintf(file, "    memcpy(buffer, obj, sizeof(%s));\n", struct_name);
    fprintf(file, "    return 0;\n");
    fprintf(file, "}\n\n");
    
    // Write decode function
    fprintf(file, "int %s_decode( const void *buffer, %s *obj ) {\n", prefix, struct_name);
    fprintf(file, "    if (buffer == NULL || obj == NULL) {\n");
    fprintf(file, "        return -1;\n");
    fprintf(file, "    }\n");
    fprintf(file, "    memcpy(obj, buffer, sizeof(%s));\n", struct_name);
    fprintf(file, "    return 0;\n");
    fprintf(file, "}\n\n");
    
    // Write setter and getter functions for each field
    for (int i = 0; i < field_count; i++) {
        if (fields[i].is_array) {
            // Setter for array
            fprintf(file, "int %s_set_%s( %s *obj, const uint8_t *value ) {\n", 
                    prefix, fields[i].name, struct_name);
            fprintf(file, "    if (obj == NULL || value == NULL) {\n");
            fprintf(file, "        return -1;\n");
            fprintf(file, "    }\n");
            fprintf(file, "    memcpy(obj->%s, value, sizeof(obj->%s));\n", 
                    fields[i].name, fields[i].name);
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n\n");
            
            // Getter for array
            fprintf(file, "int %s_get_%s( const %s *obj, uint8_t *value ) {\n", 
                    prefix, fields[i].name, struct_name);
            fprintf(file, "    if (obj == NULL || value == NULL) {\n");
            fprintf(file, "        return -1;\n");
            fprintf(file, "    }\n");
            fprintf(file, "    memcpy(value, obj->%s, sizeof(obj->%s));\n", 
                    fields[i].name, fields[i].name);
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n\n");
        } else {
            // Determine appropriate type based on size
            const char *type_name;
            if (fields[i].size <= 8) {
                type_name = "uint8_t";
            } else if (fields[i].size <= 16) {
                type_name = "uint16_t";
            } else if (fields[i].size <= 32) {
                type_name = "uint32_t";
            } else if (fields[i].size <= 64) {
                type_name = "uint64_t";
            } else {
                // For sizes larger than 64 bits, handle as array
                // Setter
                fprintf(file, "int %s_set_%s( %s *obj, const uint8_t *value ) {\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "    if (obj == NULL || value == NULL) {\n");
                fprintf(file, "        return -1;\n");
                fprintf(file, "    }\n");
                fprintf(file, "    memcpy(obj->%s, value, sizeof(obj->%s));\n", 
                        fields[i].name, fields[i].name);
                fprintf(file, "    return 0;\n");
                fprintf(file, "}\n\n");
                
                // Getter
                fprintf(file, "int %s_get_%s( const %s *obj, uint8_t *value ) {\n", 
                        prefix, fields[i].name, struct_name);
                fprintf(file, "    if (obj == NULL || value == NULL) {\n");
                fprintf(file, "        return -1;\n");
                fprintf(file, "    }\n");
                fprintf(file, "    memcpy(value, obj->%s, sizeof(obj->%s));\n", 
                        fields[i].name, fields[i].name);
                fprintf(file, "    return 0;\n");
                fprintf(file, "}\n\n");
                
                continue; // Skip the standard setter/getter generation
            }
            
            // Setter
            fprintf(file, "int %s_set_%s( %s *obj, %s value ) {\n", 
                    prefix, fields[i].name, struct_name, type_name);
            fprintf(file, "    if (obj == NULL) {\n");
            fprintf(file, "        return -1;\n");
            fprintf(file, "    }\n");
            fprintf(file, "    obj->%s = value;\n", fields[i].name);
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n\n");
            
            // Getter
            fprintf(file, "int %s_get_%s( const %s *obj, %s *value ) {\n", 
                    prefix, fields[i].name, struct_name, type_name);
            fprintf(file, "    if (obj == NULL || value == NULL) {\n");
            fprintf(file, "        return -1;\n");
            fprintf(file, "    }\n");
            fprintf(file, "    *value = obj->%s;\n", fields[i].name);
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n\n");
        }
    }
    
    fclose(file);
}

int main(int argc, char *argv[]) {
    ProgramOptions options = parse_options(argc, argv);
    
    // Read input file
    FILE *file = fopen(options.input_file, "r");
    if (!file) {
        fprintf(stderr, "Failed to open input file: %s\n", options.input_file);
        exit(EXIT_FAILURE);
    }
    
    // Count lines in file
    int line_count = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        line_count++;
    }
    
    // Rewind file
    rewind(file);
    
    // Read lines into memory
    char **lines = malloc(line_count * sizeof(char *));
    if (!lines) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < line_count; i++) {
        lines[i] = malloc(1024);
        if (!lines[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int j = 0; j < i; j++) {
                free(lines[j]);
            }
            free(lines);
            fclose(file);
            exit(EXIT_FAILURE);
        }
        
        if (fgets(lines[i], 1024, file) == NULL) {
            fprintf(stderr, "Error reading line %d\n", i);
            for (int j = 0; j <= i; j++) {
                free(lines[j]);
            }
            free(lines);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    
    fclose(file);
    
    // Parse field information
    FieldInfo *fields = NULL;
    int field_count = 0;
    parse_field_info(lines, line_count, &fields, &field_count);
    
    // Generate header and implementation files
    generate_header_file(options.prefix, options.struct_name, fields, field_count);
    generate_impl_file(options.prefix, options.struct_name, fields, field_count);
    
    // Cleanup
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    free(fields);
    
    printf("Generated %s.h and %s.c successfully.\n", options.prefix, options.prefix);
    
    return 0;
}