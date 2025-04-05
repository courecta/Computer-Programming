#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256
#define MAX_INCLUDES 10
#define MAX_REQUIRES 10
#define MAX_TESTS 100
#define MAX_FILENAME_LEN 100
#define MAX_PATH_LEN 256 // Increased buffer size for full paths
#define MAX_TOKEN_LEN 200 // Max length for expected value or expression

// Structure to hold test case information
typedef struct {
    char expected[MAX_TOKEN_LEN];
    char expression[MAX_TOKEN_LEN];
} TestCase;

// --- Helper Functions ---

// Trim leading and trailing whitespace from a string
char *trim(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

// Extract content within parentheses
int extract_content(const char *line, const char *prefix, char *output, size_t output_size) {
    const char *start = strstr(line, prefix);
    if (!start) return 0;
    start += strlen(prefix);
    while (isspace((unsigned char)*start)) start++;
    if (*start != '(') return 0;
    start++;
    const char *end = strrchr(start, ')');
    if (!end) return 0;
    size_t len = end - start;
    if (len >= output_size) len = output_size - 1;
    strncpy(output, start, len);
    output[len] = '\0';
    strcpy(output, trim(output));
    return 1;
}

// Extract expected value and expression from EXPECT_EQ line
int extract_expect_eq(const char *line, char *expected, size_t expected_size, char *expression, size_t expression_size) {
    const char *prefix = "EXPECT_EQ";
    const char *start = strstr(line, prefix);
    if (!start) return 0;
    start += strlen(prefix);
    while (isspace((unsigned char)*start)) start++;
    if (*start != '(') return 0;
    start++;
    const char *comma = strchr(start, ',');
    if (!comma) return 0;
    const char *end = strchr(comma, ')');
    if (!end) return 0;

    size_t expected_len = comma - start;
    if (expected_len >= expected_size) expected_len = expected_size - 1;
    strncpy(expected, start, expected_len);
    expected[expected_len] = '\0';
    strcpy(expected, trim(expected));
    if (strcmp(expected, "NULL") == 0) {
        strncpy(expected, "0", expected_size - 1);
        expected[expected_size - 1] = '\0';
    }

    const char *expr_start = comma + 1;
    size_t expr_len = end - expr_start + 1;
    if (expr_len >= expression_size) expr_len = expression_size - 1;
    strncpy(expression, expr_start, expr_len);
    expression[expr_len] = '\0';
    strcpy(expression, trim(expression));
    if (strlen(expression) == 0) {
        fprintf(stderr, "Warning: Parsed empty expression for line: %s\n", line);
        return 0;
    }
    return 1;
}

// *** NEW (v6): Helper to get basename from a path ***
const char* get_basename(const char *path) {
    // Find the last path separator '/'
    const char *base = strrchr(path, '/');
    // TODO: Could also check for '\' on Windows if needed
    return base ? base + 1 : path; // If separator found, return char after it, else return original path
}


// --- Main Logic ---

int main() {
    char script_filename[MAX_PATH_LEN]; // Use MAX_PATH_LEN
    char output_dir[MAX_PATH_LEN] = ""; // Directory to output files
    char output_c_path[MAX_PATH_LEN];
    char output_makefile_path[MAX_PATH_LEN];
    char line[MAX_LINE_LEN];
    FILE *script_file, *c_file, *makefile;

    char includes[MAX_INCLUDES][MAX_FILENAME_LEN]; // Store include paths as given
    char requires[MAX_REQUIRES][MAX_FILENAME_LEN]; // Store require paths as given
    TestCase tests[MAX_TESTS];
    int include_count = 0;
    int require_count = 0;
    int test_count = 0;
    int failed_count = 0;

    // 1. Get script filename
    printf("Please enter the test script: "); // Modified prompt
    // Read the whole line including potential spaces in path
    if (fgets(script_filename, sizeof(script_filename), stdin) == NULL) {
         fprintf(stderr, "Error reading script path.\n");
         return 1;
    }
    // Remove trailing newline from fgets
    script_filename[strcspn(script_filename, "\n")] = 0;


    // *** NEW (v6): Extract output directory from script path ***
    const char *last_slash = strrchr(script_filename, '/');
    // TODO: Could also check for '\' on Windows if needed
    if (last_slash != NULL) {
        // Found a directory separator
        size_t dir_len = last_slash - script_filename + 1; // Include the slash
        if (dir_len < sizeof(output_dir)) {
            strncpy(output_dir, script_filename, dir_len);
            output_dir[dir_len] = '\0'; // Null-terminate
        } else {
            fprintf(stderr, "Error: Script path directory too long.\n");
            return 1;
        }
    } else {
        // No slash found, script is in current directory.
        // output_dir remains empty or could be set to "./"
        // strcpy(output_dir, "./"); // Optional: explicit current dir
    }

    // *** NEW (v6): Construct full output paths ***
    int written_c = snprintf(output_c_path, sizeof(output_c_path), "%sa.c", output_dir);
    int written_mf = snprintf(output_makefile_path, sizeof(output_makefile_path), "%sMakefile", output_dir);

    if (written_c < 0 || (size_t)written_c >= sizeof(output_c_path) ||
        written_mf < 0 || (size_t)written_mf >= sizeof(output_makefile_path)) {
        fprintf(stderr, "Error: Could not construct output file paths (path too long?).\n");
        return 1;
    }


    // 2. Open script file (using original script_filename)
    script_file = fopen(script_filename, "r");
    if (!script_file) {
        perror("Error opening script file");
        fprintf(stderr, "Filename: %s\n", script_filename);
        return 1;
    }

    // 3. Parse script file (store include/require paths as they are)
    while (fgets(line, sizeof(line), script_file)) {
        char *comment_start = strchr(line, '#');
        if (comment_start != NULL) *comment_start = '\0';
        char *trimmed_line = trim(line);
        if (strlen(trimmed_line) == 0) continue;

        char content[MAX_TOKEN_LEN];

        if (strncmp(trimmed_line, "INCLUDE", 7) == 0) {
            if (include_count < MAX_INCLUDES && extract_content(trimmed_line, "INCLUDE", content, sizeof(content))) {
                strncpy(includes[include_count++], content, MAX_FILENAME_LEN - 1);
                includes[include_count - 1][MAX_FILENAME_LEN - 1] = '\0';
            } else { /* Handle errors/warnings */ }
        } else if (strncmp(trimmed_line, "REQUIRE", 7) == 0) {
            if (require_count < MAX_REQUIRES && extract_content(trimmed_line, "REQUIRE", content, sizeof(content))) {
                strncpy(requires[require_count++], content, MAX_FILENAME_LEN - 1);
                 requires[require_count - 1][MAX_FILENAME_LEN - 1] = '\0';
            } else { /* Handle errors/warnings */ }
        } else if (strncmp(trimmed_line, "EXPECT_EQ", 9) == 0) {
            if (test_count < MAX_TESTS) {
                 if (!extract_expect_eq(trimmed_line, tests[test_count].expected, sizeof(tests[test_count].expected), tests[test_count].expression, sizeof(tests[test_count].expression))) {
                     fprintf(stderr, "Warning: Could not parse EXPECT_EQ directive: %s\n", trimmed_line);
                     continue;
                 }
                 test_count++;
            } else { /* Handle errors/warnings */ }
        } else { /* Handle errors/warnings */ }
    }
    fclose(script_file);

    // 4. Generate a.c (using the constructed path)
    c_file = fopen(output_c_path, "w");
    if (!c_file) {
        perror("Error opening a.c for writing");
        fprintf(stderr, "Path: %s\n", output_c_path);
        return 1;
    }

    fprintf(c_file, "// Generated by unittest tool (v6 - Path Aware)\n");
    fprintf(c_file, "#include <stdio.h>\n");
    fprintf(c_file, "#include <stdlib.h>\n\n");

    // Add includes from script - paths should be relative to the script/Makefile location
    for (int i = 0; i < include_count; ++i) {
        fprintf(c_file, "#include \"%s\"\n", includes[i]);
    }
    fprintf(c_file, "\n");

    // --- Test Runner Main Function ---
    fprintf(c_file, "int main() {\n");
    fprintf(c_file, "    int test_num = 0;\n");
    fprintf(c_file, "    int failed_count = 0;\n");
    fprintf(c_file, "    int total_tests = %d;\n\n", test_count);
    fprintf(c_file, "    printf(\"Running %%d test cases:\\n\", total_tests);\n\n");

    for (int i = 0; i < test_count; ++i) {
        if (strlen(tests[i].expression) == 0 || strlen(tests[i].expected) == 0) {
             fprintf(stderr, "Internal Error: Skipping test %d due to empty expression/expected value.\n", i + 1);
             continue;
        }
        fprintf(c_file, "    // Test case %d: %s\n", i + 1, tests[i].expression);
        fprintf(c_file, "    test_num++;\n");
        fprintf(c_file, "    {\n");
        fprintf(c_file, "        int actual_result = %s;\n", tests[i].expression);
        fprintf(c_file, "        int expected_result = %s;\n", tests[i].expected);
        fprintf(c_file, "        printf(\"%%d) \", test_num);\n");
        fprintf(c_file, "        printf(\"%%s: \", \"%s\");\n", tests[i].expression); // Corrected in v5
        fprintf(c_file, "        if (actual_result == expected_result) {\n");
        fprintf(c_file, "            printf(\"PASS\\n\");\n");
        fprintf(c_file, "        } else {\n");
        fprintf(c_file, "            printf(\"FAIL - Expected: %%d, Got: %%d\\n\", expected_result, actual_result);\n");
        fprintf(c_file, "            failed_count++;\n");
        fprintf(c_file, "        }\n");
        fprintf(c_file, "    }\n\n");
    }

    fprintf(c_file, "    return (failed_count > 0) ? 1 : 0;\n");
    fprintf(c_file, "}\n");
    fclose(c_file);

    // 5. Generate Makefile (using the constructed path)
    makefile = fopen(output_makefile_path, "w");
    if (!makefile) {
        perror("Error opening Makefile for writing");
        fprintf(stderr, "Path: %s\n", output_makefile_path);
        remove(output_c_path); // Attempt cleanup of a.c
        return 1;
    }

    fprintf(makefile, "# Generated by unittest tool\n");
    fprintf(makefile, "CC = gcc\n");
    fprintf(makefile, "CFLAGS = -Wall -Wextra -std=c99 -g\n");
    fprintf(makefile, "TARGET = a.out\n");
    fprintf(makefile, "TEST_RUNNER_SRC = a.c\n"); // a.c is local to Makefile

    // *** MODIFIED (v6): Generate OBJS list using basenames ***
    fprintf(makefile, "OBJS = $(TEST_RUNNER_SRC:.c=.o)"); // Start with a.o
    char obj_basenames[MAX_REQUIRES][MAX_FILENAME_LEN]; // Store just the base.o names
    int obj_count = 0;
    for (int i = 0; i < require_count; ++i) {
        const char *base_c = get_basename(requires[i]);
        char base_o[MAX_FILENAME_LEN];
        strncpy(base_o, base_c, sizeof(base_o) - 1);
        base_o[sizeof(base_o) - 1] = '\0'; // Ensure null term

        // Change .c to .o (or append .o if no extension)
        char *dot = strrchr(base_o, '.');
        if (dot && (strcmp(dot, ".c") == 0 || strcmp(dot, ".C") == 0)) {
             strcpy(dot, ".o");
        } else {
             // Append .o safely
             strncat(base_o, ".o", sizeof(base_o) - strlen(base_o) - 1);
        }
        fprintf(makefile, " %s", base_o); // Add to OBJS list
        strncpy(obj_basenames[obj_count++], base_o, MAX_FILENAME_LEN - 1); // Store for clean rule
        obj_basenames[obj_count - 1][MAX_FILENAME_LEN - 1] = '\0';
    }
    fprintf(makefile, "\n\n");

    // --- Makefile Rules ---
    fprintf(makefile, "all: $(TARGET)\n\n");

    // Link rule (uses OBJS list)
    fprintf(makefile, "$(TARGET): $(OBJS)\n");
    fprintf(makefile, "\t$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)\n\n");

    // Rule for a.o (local source a.c)
    fprintf(makefile, "$(TEST_RUNNER_SRC:.c=.o): $(TEST_RUNNER_SRC)\n");
	fprintf(makefile, "\t$(CC) $(CFLAGS) -c $< -o $@\n\n");

    // *** MODIFIED (v6): Generate explicit rules for required source objects ***
    for (int i = 0; i < require_count; ++i) {
        const char *source_path = requires[i]; // Full path as given in script
        const char *obj_file = obj_basenames[i]; // Basename.o

        // Add dependency on corresponding header if found in includes? (More complex)
        // For simplicity, just depend on the source file.
        fprintf(makefile, "%s: %s\n", obj_file, source_path);
        fprintf(makefile, "\t$(CC) $(CFLAGS) -c %s -o %s\n\n", source_path, obj_file);
    }


    // Clean rule
    fprintf(makefile, ".PHONY: clean\n");
    fprintf(makefile, "clean:\n");
    // Remove target, local a.c, Makefile, and all generated object files by basename
    fprintf(makefile, "\trm -f $(TARGET) $(TEST_RUNNER_SRC) Makefile $(OBJS)\n");

    fclose(makefile);

    // 6. Print confirmation
    printf("%s and %s are generated.\n", output_c_path, output_makefile_path);
    //printf("a.c and Makefile are generated.\n");
    return 0;
}