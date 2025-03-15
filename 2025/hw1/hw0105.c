#include "json2toml.h"

int main() {
    char lang[16];
    char lineCountStr[16];
    char inputData[MAX_INPUT] = {0};
    char outputData[MAX_OUTPUT] = {0};

    printf("Which language is this: ");
    fgets(lang, sizeof(lang), stdin);
    lang[strcspn(lang, "\n")] = '\0';

    printf("How many lines: ");
    fgets(lineCountStr, sizeof(lineCountStr), stdin);
    int lineCount = atoi(lineCountStr);

    printf("Enter your data:\n");
    for (int i = 0; i < lineCount; i++) {
        char buffer[1024];
        fgets(buffer, sizeof(buffer), stdin);
        strcat(inputData, buffer);
    }

    if (strstr(lang, "JSON") != NULL) {
        convert_json_to_toml(inputData, outputData);
        printf("Output:\n%s\n", outputData);
    } else if (strstr(lang, "TOML") != NULL) {
        convert_toml_to_json(inputData, outputData);
        printf("Output:\n%s\n", outputData);
    } else {
        printf("Unknown language format.\n");
    }
    return 0;
}