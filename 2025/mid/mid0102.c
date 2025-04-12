#include <stdio.h>
#include <stdlib.h>

#define MIN_PRINTABLE 32
#define MAX_PRINTABLE 126
#define RANGE (MAX_PRINTABLE - MIN_PRINTABLE + 1)

int main(void) {
    char cipher_filename[256];
    char decrypted_filename[256];
    char key_filename[256];
    FILE *cipher_fp, *decrypted_fp, *key_fp;
    int mapping[128];     // Encryption mapping for ASCII values.
    int inv_mapping[128]; // Inverse mapping for decryption.
    int i, orig, enc, ch;

    // Initialize mapping to identity mapping.
    for (i = 0; i < 128; i++) {
        mapping[i] = i;
    }

    // Prompt the user for the file names.
    printf("cipher file: ");
    if (scanf("%255s", cipher_filename) != 1) {
        fprintf(stderr, "Error reading cipher filename.\n");
        return 1;
    }

    printf("decrypted file: ");
    if (scanf("%255s", decrypted_filename) != 1) {
        fprintf(stderr, "Error reading decrypted filename.\n");
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

    // Read key mappings from key file.
    // The file is assumed to provide exactly 95 lines for printable characters.
    while (fscanf(key_fp, "%d %d", &orig, &enc) == 2) {
        if (orig >= 0 && orig < 128) {
            mapping[orig] = enc;
        }
    }
    fclose(key_fp);

    // Build the inverse mapping only for characters in the printable range.
    // This avoids overwriting the intended mapping with default identity values.
    for (i = MIN_PRINTABLE; i <= MAX_PRINTABLE; i++) {
        inv_mapping[mapping[i]] = i;
    }

    // Open the cipher file.
    cipher_fp = fopen(cipher_filename, "r");
    if (cipher_fp == NULL) {
        fprintf(stderr, "Error: Cannot open cipher file '%s'.\n", cipher_filename);
        return 1;
    }

    // Open the decrypted file for writing.
    decrypted_fp = fopen(decrypted_filename, "w");
    if (decrypted_fp == NULL) {
        fprintf(stderr, "Error: Cannot open decrypted file '%s' for writing.\n", decrypted_filename);
        fclose(cipher_fp);
        return 1;
    }

    // Process the cipher file character by character.
    while ((ch = fgetc(cipher_fp)) != EOF) {
        // If the character is printable and has a mapping in the printable range,
        // decrypt it; otherwise, leave it unchanged.
        if (ch >= MIN_PRINTABLE && ch <= MAX_PRINTABLE)
            fputc(inv_mapping[ch], decrypted_fp);
        else
            fputc(ch, decrypted_fp);
    }

    fclose(cipher_fp);
    fclose(decrypted_fp);

    return 0;
}