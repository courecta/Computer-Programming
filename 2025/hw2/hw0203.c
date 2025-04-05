#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/*
 * Returns whether the pixel (x,y) is inside the blue (rounded) shape.
 *
 * For r <= l/2, the shape is the usual rounded square:
 *   - A pixel is inside if it lies in the central rectangle (x or y between r and l-r)
 *     or in one of the four quarter-circles (centered at (r, r), (l-r, r), etc.).
 *
 * For r > l/2, we “cut off” the corners by using the unique circle (with radius r)
 * that passes through the midpoints of the two adjacent edges.
 *   - For the top-left corner (x < l/2 and y < l/2) the circle’s center is (a,a) where
 *     a = (l + sqrt(8*r*r - l*l)) / 4.
 *   - Similarly for the other three corners.
 */
static bool isInsideShape(int x, int y, int l, int r) {
    if (r > l/2) {
        // For large r, the straight sections shrink: the cut-off points are at l/2.
        // Compute the circle center for the top-left corner.
        double a = (l + sqrt(8.0*r*r - (double)l*l)) / 4.0;
        // For each corner, determine which quadrant the pixel belongs to.
        if (x < l/2 && y < l/2) {
            // Top-left corner
            double dx = x - a;
            double dy = y - a;
            return (dx*dx + dy*dy) <= (double)r*r;
        } else if (x >= l/2 && y < l/2) {
            // Top-right corner
            double dx = x - (l - a);
            double dy = y - a;
            return (dx*dx + dy*dy) <= (double)r*r;
        } else if (x < l/2 && y >= l/2) {
            // Bottom-left corner
            double dx = x - a;
            double dy = y - (l - a);
            return (dx*dx + dy*dy) <= (double)r*r;
        } else { // x >= l/2 && y >= l/2
            // Bottom-right corner
            double dx = x - (l - a);
            double dy = y - (l - a);
            return (dx*dx + dy*dy) <= (double)r*r;
        }
    } else {
        // r <= l/2: use the usual rounded square definition.
        if (r <= 0) {
            return (x >= 0 && x < l && y >= 0 && y < l);
        }
        // Check the central (straight) regions:
        if (x >= r && x < (l - r))
            return true;
        if (y >= r && y < (l - r))
            return true;
        // Check the four quarter-circles:
        int cornerCenters[4][2] = {
            { r,      r      },  // top-left
            { l - r,  r      },  // top-right
            { r,      l - r  },  // bottom-left
            { l - r,  l - r  }   // bottom-right
        };
        for (int i = 0; i < 4; i++) {
            int cx = cornerCenters[i][0];
            int cy = cornerCenters[i][1];
            int dx = x - cx;
            int dy = y - cy;
            if ((dx * (long)dx + dy * (long)dy) <= (long)r * r)
                return true;
        }
        return false;
    }
}

int main(void) {
    int l, r;
    char filename[256];

    // Get user input.
    printf("Please enter the length of a side in pixel: ");
    if (scanf("%d", &l) != 1) {
        fprintf(stderr, "Invalid input for length.\n");
        return 1;
    }
    if (l <= 0) {
        fprintf(stderr, "Error: length must be positive.\n");
        return 1;
    }

    printf("Please enter the radius of the circle in pixel: ");
    if (scanf("%d", &r) != 1) {
        fprintf(stderr, "Invalid input for radius.\n");
        return 1;
    }
    if (r < 0) {
        fprintf(stderr, "Error: radius cannot be negative.\n");
        return 1;
    }

    printf("Please enter the output file name: ");
    if (scanf("%s", filename) != 1) {
        fprintf(stderr, "Invalid file name.\n");
        return 1;
    }

    // BMP Header sizes.
    const int BMP_HEADER_SIZE = 54;

    // For a 24-bit BMP, each pixel is 3 bytes (B, G, R).
    // Each row must be padded to a multiple of 4 bytes.
    int bytesPerPixel = 3;
    int rowSize = (bytesPerPixel * l + 3) & ~3;
    int imageSize = rowSize * l;
    int fileSize  = BMP_HEADER_SIZE + imageSize;

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return 1;
    }

    // --- Write BMP Header ---
    // BITMAPFILEHEADER
    unsigned char bfType[2] = { 'B', 'M' };
    fwrite(bfType, 1, 2, fp);

    uint32_t bfSize = fileSize;
    fwrite(&bfSize, 4, 1, fp);

    uint32_t bfReserved = 0;
    fwrite(&bfReserved, 4, 1, fp);

    uint32_t bfOffBits = BMP_HEADER_SIZE;
    fwrite(&bfOffBits, 4, 1, fp);

    // BITMAPINFOHEADER
    uint32_t biSize = 40;
    fwrite(&biSize, 4, 1, fp);

    int32_t biWidth = l;
    int32_t biHeight = l;
    fwrite(&biWidth, 4, 1, fp);
    fwrite(&biHeight, 4, 1, fp);

    uint16_t biPlanes = 1;
    fwrite(&biPlanes, 2, 1, fp);

    uint16_t biBitCount = 24;
    fwrite(&biBitCount, 2, 1, fp);

    uint32_t biCompression = 0;
    fwrite(&biCompression, 4, 1, fp);

    uint32_t biSizeImage = imageSize;
    fwrite(&biSizeImage, 4, 1, fp);

    uint32_t biXPelsPerMeter = 0;
    uint32_t biYPelsPerMeter = 0;
    fwrite(&biXPelsPerMeter, 4, 1, fp);
    fwrite(&biYPelsPerMeter, 4, 1, fp);

    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
    fwrite(&biClrUsed, 4, 1, fp);
    fwrite(&biClrImportant, 4, 1, fp);

    // --- Write Pixel Data ---
    unsigned char *rowBuffer = (unsigned char *)calloc(rowSize, 1);
    if (!rowBuffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        fclose(fp);
        return 1;
    }

    // Colors: Blue for the shape (B, G, R = 255, 0, 0) and White for the background (255, 255, 255).
    for (int y = l - 1; y >= 0; y--) {
        // Clear row buffer (including padding).
        for (int i = 0; i < rowSize; i++) {
            rowBuffer[i] = 0;
        }
        int offset = 0;
        for (int x = 0; x < l; x++) {
            bool inside = isInsideShape(x, y, l, r);
            if (inside) {
                rowBuffer[offset + 0] = 255; // Blue
                rowBuffer[offset + 1] = 0;   // Green
                rowBuffer[offset + 2] = 0;   // Red
            } else {
                rowBuffer[offset + 0] = 255; // Blue
                rowBuffer[offset + 1] = 255; // Green
                rowBuffer[offset + 2] = 255; // Red
            }
            offset += 3;
        }
        fwrite(rowBuffer, 1, rowSize, fp);
    }

    free(rowBuffer);
    fclose(fp);

    printf("Rounded square BMP image created successfully: %s\n", filename);
    return 0;
}