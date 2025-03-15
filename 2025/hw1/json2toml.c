#include "json2toml.h"

/*
 * Helper: Converts a TOML inline table (e.g. { mode = "auto", retry = 3 })
 * into a JSON object string with keys in double quotes.
 */
 void convert_inline_table_to_json(const char *inline_table, char *output) {
    char temp[1024] = {0};
    strncpy(temp, inline_table, sizeof(temp) - 1);
    
    // Remove the outer braces.
    char *start = strchr(temp, '{');
    char *end = strrchr(temp, '}');
    if (!start || !end) {
        strcpy(output, temp);
        return;
    }
    *end = '\0'; // Terminate before the closing brace.
    start++;     // Skip the opening brace.

    // Use strtok_r instead of strtok.
    char result[1024] = "{\n";
    char *saveptr;
    char *token = strtok_r(start, ",", &saveptr);
    int firstPair = 1;
    while (token != NULL) {
        // Each token should be a key = value pair.
        char *eq = strchr(token, '=');
        if (eq) {
            *eq = '\0';
            char *key = trim_whitespace(token);
            char *val = trim_whitespace(eq + 1);

            // Wrap key in quotes if not already.
            char keybuf[256] = {0};
            if (key[0] != '"') {
                sprintf(keybuf, "\"%s\"", key);
            } else {
                strcpy(keybuf, key);
            }

            // Wrap date-like values in quotes if needed.
            if ((strcmp(key, "release_date") == 0 || strcmp(key, "timestamp") == 0) && val[0] != '"') {
                char newVal[256] = {0};
                sprintf(newVal, "\"%s\"", val);
                val = newVal;
            }

            if (!firstPair)
                strcat(result, ",\n");
            firstPair = 0;
            strcat(result, keybuf);
            strcat(result, ": ");
            strcat(result, val);
        }
        token = strtok_r(NULL, ",", &saveptr);
    }
    strcat(result, "\n}");
    strcpy(output, result);
}

/*
 * Recursive parser for JSON objects.
 * Expects *p to point to an opening '{'. Processes key–value pairs and produces the TOML output.
 */
void parse_json_object(const char **p, char *toml, const char *parent_prefix) {
    if (**p != '{') return;
    (*p)++; // Skip the '{'
    while (1) {
        while (**p && (isspace(**p) || **p == ',')) { (*p)++; }
        if (**p == '}') { 
            (*p)++; // Skip the '}'
            break;
        }
        if (**p != '"') break;  // Malformed JSON if not a quote.
        (*p)++; // Skip the opening quote

        char key[256] = {0};
        int k = 0;
        while (**p && **p != '"') {
            key[k++] = **p;
            (*p)++;
        }
        key[k] = '\0';
        if (**p == '"') { (*p)++; } // Skip closing quote

        while (**p && (isspace(**p) || **p == ':')) { (*p)++; }

        if (**p == '{') {
            char new_prefix[512] = {0};
            if (strlen(parent_prefix) > 0)
                sprintf(new_prefix, "%s.%s", parent_prefix, key);
            else
                sprintf(new_prefix, "%s", key);

            strcat(toml, "[");
            strcat(toml, new_prefix);
            strcat(toml, "]\n");
            parse_json_object(p, toml, new_prefix);
        } else if (**p == '[') {
            char array_val[1024] = {0};
            int a = 0, bracket_count = 0;
            do {
                if (**p == '[') bracket_count++;
                if (**p == ']') bracket_count--;
                array_val[a++] = **p;
                (*p)++;
            } while (**p && bracket_count > 0);
            array_val[a] = '\0';
            strcat(toml, key);
            strcat(toml, " = ");
            strcat(toml, array_val);
            strcat(toml, "\n");
        } else {
            char value[512] = {0};
            int v = 0;
            if (**p == '"') {
                value[v++] = **p;  // Include the opening quote.
                (*p)++;
                while (**p && **p != '"') {
                    value[v++] = **p;
                    (*p)++;
                }
                if (**p == '"') {
                    value[v++] = **p; // Include the closing quote.
                    (*p)++;
                }
            } else {
                while (**p && **p != ',' && **p != '}') {
                    value[v++] = **p;
                    (*p)++;
                }
            }
            value[v] = '\0';
            strcat(toml, key);
            strcat(toml, " = ");
            strcat(toml, value);
            strcat(toml, "\n");
        }
    }
}

/*
 * Convert JSON to TOML.
 * Locates the first '{' in the JSON input and calls the recursive parser.
 */
void convert_json_to_toml(const char *json, char *toml) {
    const char *p = json;
    while (*p && *p != '{') { p++; }
    if (*p == '{') {
        parse_json_object(&p, toml, "");
    }
}

/*
 * Utility function to trim leading and trailing whitespace.
 */
char *trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0)
        return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
    return str;
}

/*
 * Convert TOML to JSON.
 * Processes TOML line-by-line, handles table headers, inline tables,
 * multi-line strings, and wraps date/time values in quotes.
 */
void convert_toml_to_json(const char *toml, char *json) {
    char toml_copy[MAX_INPUT];
    strncpy(toml_copy, toml, MAX_INPUT);
    toml_copy[MAX_INPUT - 1] = '\0';

    strcat(json, "{\n");
    int first = 1;       // For top-level keys.
    int tableFirst = 1;  // For keys inside a table.
    char *line = strtok(toml_copy, "\n");
    int inTable = 0;
    while (line != NULL) {
        char *trimmed = trim_whitespace(line);
        if (strlen(trimmed) == 0 || trimmed[0] == '#') {
            line = strtok(NULL, "\n");
            continue;
        }
        if (trimmed[0] == '[') {
            if (inTable) {
                strcat(json, "\n    }");
                strcat(json, ",\n");
            }
            char tableName[256] = {0};
            int i = 0, j = 1;
            while (trimmed[j] && trimmed[j] != ']') {
                tableName[i++] = trimmed[j++];
            }
            tableName[i] = '\0';
            inTable = 1;
            tableFirst = 1;
            if (!first) { strcat(json, ",\n"); }
            first = 0;
            strcat(json, "\"");
            strcat(json, tableName);
            strcat(json, "\": {\n");
        } else {
            char *equalSign = strchr(trimmed, '=');
            if (equalSign) {
                *equalSign = '\0';
                char *key = trim_whitespace(trimmed);
                char *value = trim_whitespace(equalSign + 1);
                // Wrap release_date or timestamp values if needed.
                if ((strcmp(key, "release_date") == 0 || strcmp(key, "timestamp") == 0) && value[0] != '"') {
                    char newValue[512] = {0};
                    sprintf(newValue, "\"%s\"", value);
                    value = newValue;
                }
                // Check for inline table.
                if (value[0] == '{') {
                    char inlineJson[1024] = {0};
                    convert_inline_table_to_json(value, inlineJson);
                    value = inlineJson;
                }
                // Handle multi-line strings starting with triple quotes.
                if (strncmp(value, "\"\"\"", 3) == 0) {
                    char multi[1024] = {0};
                    value += 3; // Skip the starting triple quotes.
                    strcat(multi, value);
                    while (1) {
                        char *nextLine = strtok(NULL, "\n");
                        if (!nextLine) break;
                        char *trimNext = trim_whitespace(nextLine);
                        char *endTriple = strstr(trimNext, "\"\"\"");
                        if (endTriple != NULL) {
                            *endTriple = '\0';
                            strcat(multi, "\\n");
                            strcat(multi, trimNext);
                            break;
                        } else {
                            strcat(multi, "\\n");
                            strcat(multi, trimNext);
                        }
                    }
                    char jsonValue[1100] = {0};
                    sprintf(jsonValue, "\"%s\"", multi);
                    value = jsonValue;
                }
                // Manage commas using separate flags.
                if (inTable) {
                    if (!tableFirst) {
                        strcat(json, ",\n");
                    }
                    tableFirst = 0;
                } else {
                    if (!first) {
                        strcat(json, ",\n");
                    }
                    first = 0;
                }
                strcat(json, "\"");
                strcat(json, key);
                strcat(json, "\": ");
                strcat(json, value);
            }
        }
        line = strtok(NULL, "\n");
    }
    if (inTable) {
        strcat(json, "\n    }");
    }
    strcat(json, "\n}\n");
}