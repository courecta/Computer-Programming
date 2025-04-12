#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Define a Color structure.
typedef struct {
    int r, g, b;
} Color;

// Define a Canvas structure.
typedef struct {
    int width, height;
    Color *pixels;  // Pixels stored in row-major order (top-left is (0,0))
} Canvas;

// Set a pixel in the canvas (only if within bounds).
void setPixel(Canvas *canvas, int x, int y, Color c) {
    if (x < 0 || x >= canvas->width || y < 0 || y >= canvas->height)
        return;
    canvas->pixels[y * canvas->width + x] = c;
}

// Create a new canvas and fill it with a background color.
Canvas *createCanvas(int width, int height, Color bg) {
    Canvas *canvas = (Canvas *)malloc(sizeof(Canvas));
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = (Color *)malloc(width * height * sizeof(Color));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            canvas->pixels[y * width + x] = bg;
        }
    }
    return canvas;
}

// Draw a filled circle on the canvas.
void drawCircle(Canvas *canvas, int cx, int cy, int r, Color c) {
    int xStart = cx - r, xEnd = cx + r;
    int yStart = cy - r, yEnd = cy + r;
    for (int y = yStart; y <= yEnd; y++) {
        for (int x = xStart; x <= xEnd; x++) {
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r) {
                setPixel(canvas, x, y, c);
            }
        }
    }
}

// Draw a filled rectangle on the canvas.
void drawRectangle(Canvas *canvas, int x, int y, int w, int h, Color c) {
    for (int j = y; j < y + h; j++) {
        for (int i = x; i < x + w; i++) {
            setPixel(canvas, i, j, c);
        }
    }
}

// Helper function for barycentric coordinates.
double edgeFunction(double ax, double ay, double bx, double by, double cx, double cy) {
    return (cx - ax) * (by - ay) - (cy - ay) * (bx - ax);
}

// Draw a filled triangle using barycentric coordinates.
void drawTriangle(Canvas *canvas, int x1, int y1, int x2, int y2, int x3, int y3, Color c) {
    // Compute triangle bounding box.
    int minX = (x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3));
    int maxX = (x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3));
    int minY = (y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3));
    int maxY = (y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3));

    // Compute the area of the triangle.
    double area = edgeFunction(x1, y1, x2, y2, x3, y3);
    if (area == 0)
        return; // Degenerate triangle

    // Iterate over the bounding box and fill in pixels that are inside the triangle.
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            // Compute barycentric coordinates.
            double w0 = edgeFunction(x2, y2, x3, y3, x, y);
            double w1 = edgeFunction(x3, y3, x1, y1, x, y);
            double w2 = edgeFunction(x1, y1, x2, y2, x, y);

            // If all have the same sign as the area, the pixel is inside.
            if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                setPixel(canvas, x, y, c);
            }
        }
    }
}

// Write the canvas to a 24-bit BMP file.
void writeBMP(const char *filename, Canvas *canvas) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: cannot create output file %s.\n", filename);
        return;
    }

    int width = canvas->width;
    int height = canvas->height;
    // Each pixel is 3 bytes and each row must be padded to a multiple of 4 bytes.
    int rowPad = (4 - (width * 3) % 4) % 4;
    int rowSize = width * 3 + rowPad;
    int dataSize = rowSize * height;
    int fileSize = 54 + dataSize;  // 54 bytes header

    // Construct BMP file header (14 bytes).
    unsigned char fileHeader[14] = {0};
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    fileHeader[2] = fileSize & 0xFF;
    fileHeader[3] = (fileSize >> 8) & 0xFF;
    fileHeader[4] = (fileSize >> 16) & 0xFF;
    fileHeader[5] = (fileSize >> 24) & 0xFF;
    // Reserved bytes remain 0.
    fileHeader[10] = 54;  // Pixel data offset

    // Construct BMP info header (40 bytes).
    unsigned char infoHeader[40] = {0};
    infoHeader[0] = 40;  // Header size
    // Image width.
    infoHeader[4] = width & 0xFF;
    infoHeader[5] = (width >> 8) & 0xFF;
    infoHeader[6] = (width >> 16) & 0xFF;
    infoHeader[7] = (width >> 24) & 0xFF;
    // Use negative height to indicate a top-down bitmap.
    int negHeight = -height;
    infoHeader[8] = negHeight & 0xFF;
    infoHeader[9] = (negHeight >> 8) & 0xFF;
    infoHeader[10] = (negHeight >> 16) & 0xFF;
    infoHeader[11] = (negHeight >> 24) & 0xFF;
    infoHeader[12] = 1;    // Planes
    infoHeader[14] = 24;   // Bits per pixel
    // Compression (0 = none)
    infoHeader[20] = dataSize & 0xFF;
    infoHeader[21] = (dataSize >> 8) & 0xFF;
    infoHeader[22] = (dataSize >> 16) & 0xFF;
    infoHeader[23] = (dataSize >> 24) & 0xFF;

    // Write headers.
    fwrite(fileHeader, sizeof(fileHeader), 1, fp);
    fwrite(infoHeader, sizeof(infoHeader), 1, fp);

    // Write pixel data.
    // Use a temporary row buffer that includes padding.
    unsigned char *row = (unsigned char *)calloc(rowSize, sizeof(unsigned char));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color pixel = canvas->pixels[y * width + x];
            // BMP uses BGR order.
            row[x * 3 + 0] = (unsigned char)pixel.b;
            row[x * 3 + 1] = (unsigned char)pixel.g;
            row[x * 3 + 2] = (unsigned char)pixel.r;
        }
        // Write the row (including any padding).
        fwrite(row, sizeof(unsigned char), rowSize, fp);
    }
    free(row);
    fclose(fp);
}

//
// Main function
//
int main(void) {
    char outputFilename[256];
    char inputFilename[256];

    // Prompt for canvas (BMP) file name.
    printf("canvas name: ");
    if (scanf("%255s", outputFilename) != 1) {
        fprintf(stderr, "Error reading canvas name.\n");
        return 1;
    }

    // Prompt for input commands file.
    printf("canvas input: ");
    if (scanf("%255s", inputFilename) != 1) {
        fprintf(stderr, "Error reading canvas input file.\n");
        return 1;
    }

    // Open the input file.
    FILE *inFile = fopen(inputFilename, "r");
    if (!inFile) {
        fprintf(stderr, "Error: cannot open input file %s.\n", inputFilename);
        return 1;
    }

    char line[512];
    int width = 0, height = 0;
    Canvas *canvas = NULL;
    Color bg;

    // Process commands line-by-line.
    while (fgets(line, sizeof(line), inFile)) {
        char command[32];
        if (sscanf(line, "%31s", command) != 1)
            continue;  // skip empty lines

        if (strcmp(command, "SIZE") == 0) {
            // Format example: SIZE 800x600
            if (sscanf(line, "SIZE %dx%d", &width, &height) != 2) {
                fprintf(stderr, "Invalid SIZE command.\n");
                fclose(inFile);
                return 1;
            }
        } else if (strcmp(command, "BACKGROUND") == 0) {
            // Format example: BACKGROUND 255,255,255
            int r, g, b;
            if (sscanf(line, "BACKGROUND %d,%d,%d", &r, &g, &b) != 3) {
                fprintf(stderr, "Invalid BACKGROUND command.\n");
                fclose(inFile);
                return 1;
            }
            bg.r = r; bg.g = g; bg.b = b;
            // Now we create the canvas.
            canvas = createCanvas(width, height, bg);
        } else if (strcmp(command, "CIRCLE") == 0) {
            // Format example: CIRCLE (350,150) ,50,255,0,0
            int cx, cy, r, R, G, B;
            if (sscanf(line, "CIRCLE (%d,%d) ,%d,%d,%d,%d", &cx, &cy, &r, &R, &G, &B) != 6) {
                fprintf(stderr, "Invalid CIRCLE command.\n");
                continue;
            }
            Color c = {R, G, B};
            drawCircle(canvas, cx, cy, r, c);
        } else if (strcmp(command, "RECTANGLE") == 0) {
            // Format example: RECTANGLE (300,150) ,200,50,255,255,255
            int x, y, w, h, R, G, B;
            if (sscanf(line, "RECTANGLE (%d,%d) ,%d,%d,%d,%d,%d",
                       &x, &y, &w, &h, &R, &G, &B) != 7) {
                fprintf(stderr, "Invalid RECTANGLE command.\n");
                continue;
            }
            Color c = {R, G, B};
            drawRectangle(canvas, x, y, w, h, c);
        } else if (strcmp(command, "TRIANGLE") == 0) {
            // Format example: TRIANGLE (300,150) ,(500,150) ,(400,250) ,255,0,0
            int x1, y1, x2, y2, x3, y3, R, G, B;
            if (sscanf(line, "TRIANGLE (%d,%d) ,(%d,%d) ,(%d,%d) ,%d,%d,%d",
                       &x1, &y1, &x2, &y2, &x3, &y3, &R, &G, &B) != 9) {
                fprintf(stderr, "Invalid TRIANGLE command.\n");
                continue;
            }
            Color c = {R, G, B};
            drawTriangle(canvas, x1, y1, x2, y2, x3, y3, c);
        }
    }
    fclose(inFile);

    // Write the canvas to a BMP file.
    writeBMP(outputFilename, canvas);

    // Free allocated memory.
    free(canvas->pixels);
    free(canvas);

    return 0;
}