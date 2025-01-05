#include "tlv.h"

size_t take(uint8_t *bytes) {
    size_t size = 0;
    while(1)
    {
        int byte = 0;

        // Skip non-hexadecimal characters like '[', ']', and ','
        if (scanf(" %*[^0-9a-fA-F]") == EOF) break;  // Stop if end-of-input is reached

        // Parse a hexadecimal value
        if (scanf("%x", &byte) != 1) break;  // Stop if no more valid hex values

        // Terminate if 0xFF is encountered
        if (byte == 0xFF)
        {
          bytes[size++] = (uint8_t)byte;
          break;
        }

        bytes[size++] = (uint8_t)byte;
    }
    return size;
}

int main(void) {
    uint8_t bytes[1024];
    size_t size = take(bytes);
    int32_t result = run(bytes, size);
    if (result == -1) {
        printf("\n======\nERROR DETECTED\n");
    }
    return EXIT_SUCCESS;
}