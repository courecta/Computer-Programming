#include <stdio.h>
#include <stdlib.h>

#define MAPPING_SIZE 128

int main(void) {
    char origin_filename[256];
    char encrypted_filename[256];
    char key_filename[256];
    FILE *origin_fp, *encrypted_fp, *key_fp;
    int mapping[MAPPING_SIZE];
    int i, orig, enc, ch;

    // Initialize mapping to identity mapping.
    for (i = 0; i < MAPPING_SIZE; i++) {
        mapping[i] = i;
    }

    // Prompt the user for the file names.
    printf("origin file: ");
    if (scanf("%255s", origin_filename) != 1) {
        fprintf(stderr, "Error reading origin filename.\n");
        return 1;
    }

    printf("encrypted file: ");
    if (scanf("%255s", encrypted_filename) != 1) {
        fprintf(stderr, "Error reading encrypted filename.\n");
        return 1;
    }

    printf("key file: ");
    if (scanf("%255s", key_filename) != 1) {
        fprintf(stderr, "Error reading key filename.\n");
        return 1;
    }

    // Open the key file.
    key_fp = fopen(key_filename, "r");
    if (key_fp == NULL) {
        fprintf(stderr, "Error: Cannot open key file '%s'.\n", key_filename);
        return 1;
    }

    // Read the key file to update the mapping.
    // Each line is expected to contain two integers.
    while (fscanf(key_fp, "%d %d", &orig, &enc) == 2) {
        if (orig >= 0 && orig < MAPPING_SIZE) {
            mapping[orig] = enc;
        }
    }
    fclose(key_fp);

    // Open the origin file.
    origin_fp = fopen(origin_filename, "r");
    if (origin_fp == NULL) {
        fprintf(stderr, "Error: Cannot open origin file '%s'.\n", origin_filename);
        return 1;
    }

    // Open the encrypted file for writing.
    encrypted_fp = fopen(encrypted_filename, "w");
    if (encrypted_fp == NULL) {
        fprintf(stderr, "Error: Cannot open encrypted file '%s' for writing.\n", encrypted_filename);
        fclose(origin_fp);
        return 1;
    }

    // Process the origin file character by character.
    while ((ch = fgetc(origin_fp)) != EOF) {
        fputc(mapping[ch], encrypted_fp);
    }

    fclose(origin_fp);
    fclose(encrypted_fp);

    return 0;
}