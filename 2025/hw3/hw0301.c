#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 2048
#define MAX_TARGET 256

void to_lower_str(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

char *strcasestr_local(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    size_t needle_len = strlen(needle);
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            if (strncasecmp(haystack, needle, needle_len) == 0) {
                return (char *)haystack;
            }
        }
    }
    return NULL;
}

int main(void) {
    char target[MAX_TARGET];
    char line[MAX_LINE];
    char target_lower[MAX_TARGET];
    int count = 0, idx = 1;
    FILE *fp = fopen("bible.txt", "r");
    if (!fp) {
        fprintf(stderr, "Failed to open bible.txt\n");
        return 1;
    }

    printf("Please enter the search target: ");
    if (!fgets(target, sizeof(target), stdin)) {
        fclose(fp);
        return 1;
    }
    // Remove trailing newline
    target[strcspn(target, "\n")] = '\0';
    strncpy(target_lower, target, sizeof(target_lower));
    target_lower[sizeof(target_lower)-1] = '\0';
    to_lower_str(target_lower);

    // First pass: count matches
    while (fgets(line, sizeof(line), fp)) {
        char *text_ptr = strstr(line, "\"text\":");
        if (!text_ptr) continue;
        char line_lower[MAX_LINE];
        strncpy(line_lower, line, sizeof(line_lower));
        line_lower[sizeof(line_lower)-1] = '\0';
        to_lower_str(line_lower);
        if (strcasestr_local(line_lower, target_lower)) {
            count++;
        }
    }
    printf("Found %d time(s)\n", count);
    if (count == 0) {
        fclose(fp);
        return 0;
    }
    rewind(fp);
    idx = 1;
    // Second pass: print matches
    while (fgets(line, sizeof(line), fp)) {
        char *text_ptr = strstr(line, "\"text\":");
        if (!text_ptr) continue;
        char line_lower[MAX_LINE];
        strncpy(line_lower, line, sizeof(line_lower));
        line_lower[sizeof(line_lower)-1] = '\0';
        to_lower_str(line_lower);
        if (strcasestr_local(line_lower, target_lower)) {
            // Extract book_id, chapter, verse, text
            char book_id[16], book_name[32], text[1024];
            int chapter, verse;
            // Parse JSON line (simple, not robust)
            char *p = strstr(line, "\"book_id\":");
            if (p) {
                sscanf(p, "\"book_id\":\"%15[^\"]", book_id);
            } else {
                strcpy(book_id, "?");
            }
            p = strstr(line, "\"chapter\":");
            if (p) {
                sscanf(p, "\"chapter\":%d", &chapter);
            } else {
                chapter = 0;
            }
            p = strstr(line, "\"verse\":");
            if (p) {
                sscanf(p, "\"verse\":%d", &verse);
            } else {
                verse = 0;
            }
            p = strstr(line, "\"text\":\"");
            if (p) {
                sscanf(p, "\"text\":\"%1023[^\"]", text);
            } else {
                strcpy(text, "");
            }
            printf("%d. %s %d:%d %s\n", idx++, book_id, chapter, verse, text);
        }
    }
    fclose(fp);
    return 0;
}