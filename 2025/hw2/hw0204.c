#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Define the BITMAPFILEHEADER and BITMAPINFOHEADER structures
// We use uint16_t and uint32_t from stdint.h for fixed-width integers.

#pragma pack(push, 1) // ensure no padding in these structures
typedef struct {
    uint16_t bfType;      // File type, must be 'BM'
    uint32_t bfSize;      // Size of the file in bytes
    uint16_t bfReserved1; // Reserved, must be zero
    uint16_t bfReserved2; // Reserved, must be zero
    uint32_t bfOffBits;   // Offset to start of pixel data
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;          // Size of the header (40 bytes)
    int32_t  biWidth;         // Width of the image in pixels
    int32_t  biHeight;        // Height of the image in pixels
    uint16_t biPlanes;        // Number of color planes, must be 1
    uint16_t biBitCount;      // Number of bits per pixel (we assume 24)
    uint32_t biCompression;   // Compression type (0 = uncompressed)
    uint32_t biSizeImage;     // Image size (may be 0 for uncompressed)
    int32_t  biXPelsPerMeter; // Preferred resolution in pixels per meter
    int32_t  biYPelsPerMeter; // Preferred resolution in pixels per meter
    uint32_t biClrUsed;       // Number of color map entries
    uint32_t biClrImportant;  // Number of significant colors
} BITMAPINFOHEADER;
#pragma pack(pop)

// Define an RGB pixel structure (BMP pixels are stored as BGR)
typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} RGB;

// Define a structure to hold a circular focus region
typedef struct {
    int centerX;
    int centerY;
    int radius;
} Circle;

// Maximum number of circles allowed (arbitrary, can be increased)
#define MAX_CIRCLES 100

// Function prototypes
int isPointInCircle(int x, int y, const Circle *circle);
uint8_t computeGray(uint8_t r, uint8_t g, uint8_t b);

int main() {
    char inputFileName[256];
    char outputFileName[256];
    
    // Prompt for input image name
    printf("Please enter the input image name: ");
    if (scanf("%255s", inputFileName) != 1) {
        fprintf(stderr, "Error reading input file name.\n");
        return 1;
    }
    
    // Open input file in binary mode
    FILE *inFile = fopen(inputFileName, "rb");
    if (inFile == NULL) {
        fprintf(stderr, "Cannot open input file.\n");
        return 1;
    }
    
    // Read the first two bytes to check BMP signature
    char signature[2];
    if (fread(signature, 1, 2, inFile) != 2) {
        fprintf(stderr, "Error reading file signature.\n");
        fclose(inFile);
        return 1;
    }
    if (signature[0] != 'B' || signature[1] != 'M') {
        fprintf(stderr, "Warning: This file is not a BMP file.\n");
        fclose(inFile);
        return 1;
    }
    
    // Since we've read the first two bytes, rewind to beginning for complete header read
    rewind(inFile);
    
    // Read the BITMAPFILEHEADER
    BITMAPFILEHEADER fileHeader;
    if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, inFile) != 1) {
        fprintf(stderr, "Error reading BMP file header.\n");
        fclose(inFile);
        return 1;
    }
    
    // Read the BITMAPINFOHEADER
    BITMAPINFOHEADER infoHeader;
    if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, inFile) != 1) {
        fprintf(stderr, "Error reading BMP info header.\n");
        fclose(inFile);
        return 1;
    }
    
    // Check that we are dealing with a 24-bit BMP image
    if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0) {
        fprintf(stderr, "Only uncompressed 24-bit BMP files are supported.\n");
        fclose(inFile);
        return 1;
    }
    
    // Allocate memory for pixel data
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight); // abs in case height is negative
    // Each row is padded to a multiple of 4 bytes.
    int rowPadding = (4 - (width * 3) % 4) % 4;
    int rowSize = width * 3 + rowPadding;
    
    // Allocate memory for the entire pixel array
    uint8_t *pixelData = (uint8_t *)malloc(rowSize * height);
    if (pixelData == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(inFile);
        return 1;
    }
    
    // Move file pointer to the beginning of pixel data
    fseek(inFile, fileHeader.bfOffBits, SEEK_SET);
    
    // Read pixel data row by row
    for (int i = 0; i < height; i++) {
        if (fread(pixelData + i * rowSize, 1, rowSize, inFile) != rowSize) {
            fprintf(stderr, "Error reading pixel data.\n");
            free(pixelData);
            fclose(inFile);
            return 1;
        }
    }
    fclose(inFile);
    
    // Prompt for output image name
    printf("Please enter the output image name: ");
    if (scanf("%255s", outputFileName) != 1) {
        fprintf(stderr, "Error reading output file name.\n");
        free(pixelData);
        return 1;
    }
    
    // Array to store circles
    Circle circles[MAX_CIRCLES];
    int circleCount = 0;
    
    // Process user input for circular focuses
    char choice;
    while (1) {
        printf("Want to add a new focus [Y/N]? ");
        scanf(" %c", &choice);
        choice = toupper(choice);
        if (choice == 'N') {
            break;
        } else if (choice == 'Y') {
            if (circleCount >= MAX_CIRCLES) {
                fprintf(stderr, "Maximum number of circles reached.\n");
                break;
            }
            // Expect input exactly like: (200,480)
            printf("Please enter the center: ");
            // Read the input string. We'll assume the input is exactly in the given format.
            char centerStr[50];
            scanf("%49s", centerStr);
            // Now parse the string. Expect it to be in the format "(x,y)"
            int cx, cy;
            if (sscanf(centerStr, "(%d,%d)", &cx, &cy) != 2) {
                fprintf(stderr, "Invalid center format. Expected format: (x,y)\n");
                continue;
            }
            // Read radius
            int r;
            printf("Please enter the radius: ");
            if (scanf("%d", &r) != 1) {
                fprintf(stderr, "Invalid radius input.\n");
                continue;
            }
            // Add circle to array
            circles[circleCount].centerX = cx;
            circles[circleCount].centerY = cy;
            circles[circleCount].radius = r;
            circleCount++;
        } else {
            fprintf(stderr, "Invalid choice. Please enter Y or N.\n");
        }
    }
    
    // Process each pixel in the image.
    // Note: Our coordinate system is assumed with (0,0) at the top-left.
    // However, BMP files are typically stored bottom-up, so the first row in our pixelData
    // corresponds to the bottom of the image. We need to map accordingly:
    // y_coordinate = height - row_index - 1
    for (int row = 0; row < height; row++) {
        // Calculate the y coordinate in our coordinate system
        int y = row;  // If we assume image is stored top-down. 
                      // Otherwise, if stored bottom-up, use: int y = height - row - 1;
                      // Since the problem specifies the coordinate system, adjust if needed.
                      // Here, we assume the BMP is stored in a top-down order for simplicity.
        for (int col = 0; col < width; col++) {
            // Compute pixel offset in the row
            int offset = row * rowSize + col * 3;
            // Get the pixel
            RGB *pixel = (RGB *)&pixelData[offset];
            
            // Determine if the pixel is inside any focus circle.
            // The circle definition is based on the coordinate system where (0,0) is top-left.
            int inFocus = 0;
            for (int i = 0; i < circleCount; i++) {
                if (isPointInCircle(col, y, &circles[i])) {
                    inFocus = 1;
                    break;
                }
            }
            // If not in any focus, convert the pixel to grayscale.
            if (!inFocus) {
                uint8_t gray = computeGray(pixel->red, pixel->green, pixel->blue);
                pixel->red = gray;
                pixel->green = gray;
                pixel->blue = gray;
            }
        }
    }
    
    // Write the modified pixel data into the output BMP file.
    FILE *outFile = fopen(outputFileName, "wb");
    if (outFile == NULL) {
        fprintf(stderr, "Cannot open output file.\n");
        free(pixelData);
        return 1;
    }
    
    // Write the headers back.
    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, outFile);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, outFile);
    
    // Write pixel data row by row
    for (int i = 0; i < height; i++) {
        fwrite(pixelData + i * rowSize, 1, rowSize, outFile);
    }
    
    fclose(outFile);
    free(pixelData);
    
    printf("Output image created successfully as %s\n", outputFileName);
    return 0;
}

// Function to determine if a point (x, y) is within a given circle.
// Returns 1 if inside, 0 otherwise.
// Points with negative coordinates are considered out-of-bound, but if a circle covers some valid region, those valid pixels are processed.
int isPointInCircle(int x, int y, const Circle *circle) {
    int dx = x - circle->centerX;
    int dy = y - circle->centerY;
    // Compare squared distance to squared radius.
    return (dx * dx + dy * dy <= circle->radius * circle->radius);
}

// Function to compute the grayscale value using the formula:
// Gray = 0.299 * R + 0.587 * G + 0.114 * B
// The computed value is rounded to the nearest integer.
uint8_t computeGray(uint8_t r, uint8_t g, uint8_t b) {
    return (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);
}