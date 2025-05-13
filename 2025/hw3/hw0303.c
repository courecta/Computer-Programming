/*
 * golden_ratio_corrected.c
 *
 * Generate a BMP of the Fibonacci spiral composed of quarter-circle arcs,
 * with a smooth RGB gradient from start-color to end-color.
 * Corrected for connectivity, spiral direction, and bounding box.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdint.h>
 #include <math.h>
 
 // M_PI might not be defined in strict C standards, 
 // but usually is with <math.h> and _USE_MATH_DEFINES on Windows
 #ifndef M_PI
 #define M_PI 3.14159265358979323846
 #endif
 
 #define MAX_ITER 1000
 
 /* Color and BMP structures */
 typedef struct { uint8_t r, g, b; } Color;
 typedef struct { int width, height, padding; uint8_t *data; } BMPImage;
 
 /* Structure to store parameters for each arc */
 typedef struct {
     double cx, cy;          // Center of the arc
     double radius;
     double start_angle, end_angle;
 } ArcGeometry;
 
 /* Prototypes */
 void print_help(const char *prog);
 int parse_color(const char *hex, Color *c);
 void compute_fib(int n_terms, long fib[]);
 BMPImage *create_canvas(int w, int h, Color bg);
 void write_bmp(const char *filename, const BMPImage *img);
 void draw_line(BMPImage *img, int x0, int y0, int x1, int y1, Color c);
 void draw_quarter_arc(BMPImage *img, double cx_world, double cy_world, double radius,
                       double start_ang, double end_ang,
                       Color c0, Color c1,
                       double *accumulated_dist, double total_dist,
                       double global_min_x, double global_min_y);
 void set_pixel(BMPImage *img, int px, int py, Color c);
 
 int main(int argc, char **argv) {
     int base_radius = 0;
     int num_arcs = 5;
     Color bg_color = {255, 255, 255};
     Color start_color = {0, 0, 0};
     Color end_color = {255, 255, 255};
     char *outfile_name = strdup("output.bmp");
 
     /* Parse arguments */
     for (int i = 1; i < argc; i++) {
         if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
             print_help(argv[0]); free(outfile_name); return 0;
         } else if ((!strcmp(argv[i],"-r")||!strcmp(argv[i],"--radius")) && i+1<argc) {
             base_radius = atoi(argv[++i]);
         } else if ((!strcmp(argv[i],"-n")||!strcmp(argv[i],"--number")) && i+1<argc) {
             num_arcs = atoi(argv[++i]);
         } else if (!strcmp(argv[i],"--background-color") && i+1<argc) {
             if (parse_color(argv[++i], &bg_color) != 0) { free(outfile_name); return 1;}
         } else if (!strcmp(argv[i],"--start-color") && i+1<argc) {
             if (parse_color(argv[++i], &start_color) != 0) { free(outfile_name); return 1;}
         } else if (!strcmp(argv[i],"--end-color") && i+1<argc) {
             if (parse_color(argv[++i], &end_color) != 0) { free(outfile_name); return 1;}
         } else if ((!strcmp(argv[i],"-o")||!strcmp(argv[i],"--output")) && i+1<argc) {
             free(outfile_name); outfile_name = strdup(argv[++i]);
         } else {
             fprintf(stderr, "Unknown option: %s\n", argv[i]);
             print_help(argv[0]);
             free(outfile_name); return 1;
         }
     }
 
     if (base_radius <= 0) {
         fprintf(stderr, "Error: Radius (-r, --radius) must be a positive integer.\n");
         free(outfile_name); return 1;
     }
     if (num_arcs <= 0 || num_arcs > MAX_ITER) {
         fprintf(stderr, "Error: Number of arcs (-n, --number) must be positive and <= %d.\n", MAX_ITER);
         free(outfile_name); return 1;
     }
 
     long fib[MAX_ITER + 1];
     compute_fib(num_arcs, fib); // Compute Fibonacci numbers F_1 to F_num_arcs
 
     ArcGeometry computed_arcs[MAX_ITER];
     double pen_x = 0.0, pen_y = 0.0; // Current drawing point, starts at (0,0) of the spiral's local coords
     double min_coord_x = 0.0, max_coord_x = 0.0; // Bounding box of the spiral
     double min_coord_y = 0.0, max_coord_y = 0.0;
     double total_spiral_length = 0.0;
 
     // --- First pass: Calculate arc geometries and overall bounding box ---
     for (int k = 0; k < num_arcs; k++) {
         double radius = (double)fib[k + 1] * base_radius; // fib[1] is the first Fibonacci number for the first arc
         if (radius < 0) radius = 0; // Should not happen with standard Fibonacci
 
         int dir = k % 4; // Determines direction of the current arc
         double cx, cy, sa, ea; // Center, start angle, end angle
         
         // arc_start_x and arc_start_y are where the current arc begins (i.e., current pen position)
         double arc_start_x = pen_x;
         double arc_start_y = pen_y;
 
         // Determine arc parameters for a counter-clockwise spiral starting UP-RIGHT
         if (dir == 0) { // Arc 1: From (0,0) going Up-Right
             cx = arc_start_x;
             cy = arc_start_y + radius;
             sa = 1.5 * M_PI; // 270 degrees (pointing down from center)
             ea = 2.0 * M_PI; // 360 degrees (pointing right from center)
             pen_x = cx + radius; // End point: (arc_start_x + radius, arc_start_y + radius)
             pen_y = cy;          // Center is (arc_start_x, arc_start_y + radius)
         } else if (dir == 1) { // Arc 2: From previous end, going Up-Left
             cx = arc_start_x - radius;
             cy = arc_start_y;
             sa = 0.0 * M_PI; // 0 degrees (pointing right from center)
             ea = 0.5 * M_PI; // 90 degrees (pointing up from center)
             pen_x = cx;          // End point: (arc_start_x - radius, arc_start_y + radius)
             pen_y = cy + radius; // Center is (arc_start_x - radius, arc_start_y)
         } else if (dir == 2) { // Arc 3: From previous end, going Down-Left
             cx = arc_start_x;
             cy = arc_start_y - radius;
             sa = 0.5 * M_PI; // 90 degrees (pointing up from center)
             ea = 1.0 * M_PI; // 180 degrees (pointing left from center)
             pen_x = cx - radius; // End point: (arc_start_x - radius, arc_start_y - radius)
             pen_y = cy;          // Center is (arc_start_x, arc_start_y - radius)
         } else { // dir == 3, Arc 4: From previous end, going Down-Right
             cx = arc_start_x + radius;
             cy = arc_start_y;
             sa = 1.0 * M_PI; // 180 degrees (pointing left from center)
             ea = 1.5 * M_PI; // 270 degrees (pointing down from center)
             pen_x = cx;          // End point: (arc_start_x + radius, arc_start_y - radius)
             pen_y = cy - radius; // Center is (arc_start_x + radius, arc_start_y)
         }
 
         computed_arcs[k].cx = cx;
         computed_arcs[k].cy = cy;
         computed_arcs[k].radius = radius;
         computed_arcs[k].start_angle = sa;
         computed_arcs[k].end_angle = ea;
         total_spiral_length += (M_PI / 2.0) * radius;
 
         // Update overall bounding box based on this arc's extents
         double current_arc_min_x, current_arc_max_x, current_arc_min_y, current_arc_max_y;
         // The bounding box of each quarter arc segment is defined by its center and radius
         if (dir == 0) { // Arc from (cx, cy-R) to (cx+R, cy)
             current_arc_min_x = cx; current_arc_max_x = cx + radius;
             current_arc_min_y = cy - radius; current_arc_max_y = cy;
         } else if (dir == 1) { // Arc from (cx+R, cy) to (cx, cy+R)
             current_arc_min_x = cx; current_arc_max_x = cx + radius;
             current_arc_min_y = cy; current_arc_max_y = cy + radius;
         } else if (dir == 2) { // Arc from (cx, cy+R) to (cx-R, cy)
             current_arc_min_x = cx - radius; current_arc_max_x = cx;
             current_arc_min_y = cy; current_arc_max_y = cy + radius;
         } else { // dir == 3, Arc from (cx-R, cy) to (cx, cy-R)
             current_arc_min_x = cx - radius; current_arc_max_x = cx;
             current_arc_min_y = cy - radius; current_arc_max_y = cy;
         }
         
         if (k == 0) { // Initialize global bounds with the first arc's bounds
             min_coord_x = current_arc_min_x; max_coord_x = current_arc_max_x;
             min_coord_y = current_arc_min_y; max_coord_y = current_arc_max_y;
         } else { // Expand global bounds
             if (current_arc_min_x < min_coord_x) min_coord_x = current_arc_min_x;
             if (current_arc_max_x > max_coord_x) max_coord_x = current_arc_max_x;
             if (current_arc_min_y < min_coord_y) min_coord_y = current_arc_min_y;
             if (current_arc_max_y > max_coord_y) max_coord_y = current_arc_max_y;
         }
     }
     
     // Handle case of num_arcs = 0 or very small spiral (e.g. radius 0)
     if (num_arcs == 0) {
          min_coord_x = 0; max_coord_x = 0; min_coord_y = 0; max_coord_y = 0;
     }
 
     // Canvas dimensions based on the calculated bounding box
     // Add 1 to ensure the pixels at max_coord_x/y are included
     int canvas_width = (int)round(max_coord_x - min_coord_x) + 1;
     int canvas_height = (int)round(max_coord_y - min_coord_y) + 1;
     
     if (canvas_width <= 0) canvas_width = 1;
     if (canvas_height <= 0) canvas_height = 1;
 
     BMPImage *img = create_canvas(canvas_width, canvas_height, bg_color);
     if (!img) {
         fprintf(stderr, "Failed to create canvas.\n");
         free(outfile_name);
         return 1;
     }
 
     // --- Second pass: Draw the arcs onto the canvas ---
     // Arcs are drawn offset by min_coord_x and min_coord_y to fit into the canvas
     double accumulated_length = 0.0;
     for (int k = 0; k < num_arcs; k++) {
         ArcGeometry arc = computed_arcs[k];
         draw_quarter_arc(img, arc.cx, arc.cy, arc.radius,
                            arc.start_angle, arc.end_angle,
                            start_color, end_color,
                            &accumulated_length, total_spiral_length,
                            min_coord_x, min_coord_y); // Pass offset
     }
 
     write_bmp(outfile_name, img);
     printf("Wrote %s (%dx%d)\n", outfile_name, canvas_width, canvas_height);
 
     free(img->data);
     free(img);
     free(outfile_name);
     return 0;
 }
 
 void print_help(const char *prog) {
     printf("Usage: %s [options]\n", prog); // Added Usage line back
     printf("Options:\n");
     printf("\t-r, --radius <int> Mandatory. Set the radius (pixel) of the first quarter circle.\n");
     printf("\t--background-color <RRGGBB> Set the background color with RGB. The default value is FFFFFF.\n");
     printf("\t--start-color <RRGGBB> Set the first quarter circle color with RGB. The default value is 000000.\n");
     printf("\t--end-color <RRGGBB> Set the last quarter circle color with RGB. The default value is FFFFFF.\n");
     printf("\t-n, --number <int> Set the iteration number. The default value is 5.\n");
     printf("\t-o, --output <File> Set the output file name. The default value is output.bmp.\n");
     printf("\t-h, --help Display this description.\n");
 }
 
 int parse_color(const char *hex, Color *c) {
     if (strlen(hex) != 6) {
         fprintf(stderr, "Error: Color hex string '%s' must be 6 characters long (RRGGBB).\n", hex);
         return -1; 
     }
     char buf[3] = {0}; // Temporary buffer for 2 hex chars + null terminator
     sscanf(hex, "%2hhx%2hhx%2hhx", &c->r, &c->g, &c->b); // More direct parsing
     return 0; 
 }
 
 void compute_fib(int n_terms, long fib[]) {
     if (n_terms <= 0) return;
     fib[0] = 0; // F0, usually not used for spiral radii
     if (n_terms >= 1) fib[1] = 1; // F1
     if (n_terms >= 2) fib[2] = 1; // F2
     for (int i = 3; i <= n_terms; i++) {
         if (i > MAX_ITER) break; // Safety, though num_arcs is already checked
         fib[i] = fib[i - 1] + fib[i - 2];
     }
 }
 
 BMPImage *create_canvas(int w, int h, Color bg) {
     if (w <= 0 || h <= 0) {
         fprintf(stderr, "Error: Canvas width and height must be positive (%d x %d requested).\n", w, h);
         return NULL;
     }
     BMPImage *img = malloc(sizeof(BMPImage));
     if (!img) { perror("Failed to allocate BMPImage struct"); return NULL; }
 
     img->width = w;
     img->height = h;
     img->padding = (4 - (3 * w) % 4) % 4;
     size_t row_size = (size_t)3 * w + img->padding;
     // Check for potential overflow if w and h are huge, though MAX_ITER limits this somewhat
     if (h > 0 && row_size > SIZE_MAX / h) { // Check for overflow before multiplication
         fprintf(stderr, "Error: Requested image dimensions too large, would overflow size_t.\n");
         free(img);
         return NULL;
     }
     img->data = malloc((size_t)h * row_size); 
     if (!img->data) { 
         perror("Failed to allocate BMP pixel data");
         free(img); 
         return NULL; 
     }
 
     // Fill background
     for (int y = 0; y < h; y++) {
         for (int x = 0; x < w; x++) {
             size_t pixel_offset = (size_t)y * row_size + (size_t)3 * x;
             img->data[pixel_offset + 0] = bg.b; // Blue
             img->data[pixel_offset + 1] = bg.g; // Green
             img->data[pixel_offset + 2] = bg.r; // Red
         }
     }
     return img;
 }
 
 void write_bmp(const char *filename, const BMPImage *img) {
     FILE *f = fopen(filename, "wb");
     if (!f) {
         perror("Error opening file for BMP writing");
         return;
     }
 
     int w = img->width;
     int h = img->height;
     int padding = img->padding;
     size_t row_size = (size_t)3 * w + padding;
     // Ensure h is not negative or excessively large before multiplication
     if (h < 0) { fprintf(stderr, "Error: BMP height is negative.\n"); fclose(f); return; }
     uint32_t data_size = (uint32_t)((size_t)h * row_size); // Cast to size_t first for multiplication
     uint32_t file_size = 54 + data_size;
 
 
     uint8_t bmp_header[54] = {0};
     bmp_header[0] = 'B'; bmp_header[1] = 'M';
     memcpy(&bmp_header[2], &file_size, 4);
     uint32_t data_offset = 54;
     memcpy(&bmp_header[10], &data_offset, 4);
 
     uint32_t dib_header_size = 40;
     memcpy(&bmp_header[14], &dib_header_size, 4);
     memcpy(&bmp_header[18], &w, 4);
     memcpy(&bmp_header[22], &h, 4);
     uint16_t color_planes = 1;
     memcpy(&bmp_header[26], &color_planes, 2);
     uint16_t bits_per_pixel = 24;
     memcpy(&bmp_header[28], &bits_per_pixel, 2);
 
     if (fwrite(bmp_header, 1, 54, f) != 54) {
         perror("Error writing BMP header");
         fclose(f);
         return;
     }
     
     for (int y = 0; y < h; y++) {
         if (fwrite(img->data + (size_t)(h - 1 - y) * row_size, 1, row_size, f) != row_size) {
             perror("Error writing BMP pixel data row");
             fclose(f);
             return;
         }
     }
     fclose(f);
 }
 
 void set_pixel(BMPImage *img, int px, int py, Color c) {
     if (px < 0 || px >= img->width || py < 0 || py >= img->height) {
         return; 
     }
     size_t row_size = (size_t)3 * img->width + img->padding;
     // py is 0 for bottom row, img->height-1 for top row in this canvas representation
     uint8_t *p = img->data + (size_t)py * row_size + (size_t)3 * px; 
     p[0] = c.b; 
     p[1] = c.g; 
     p[2] = c.r; 
 }
 
 void draw_line(BMPImage *img, int x0, int y0, int x1, int y1, Color c) {
     int dx = abs(x1 - x0);
     int sx = (x0 < x1) ? 1 : -1;
     int dy = -abs(y1 - y0); 
     int sy = (y0 < y1) ? 1 : -1;
     int err = dx + dy;
     int e2;
 
     for (;;) {
         set_pixel(img, x0, y0, c);
         if (x0 == x1 && y0 == y1) break;
         e2 = 2 * err;
         if (e2 >= dy) { 
             err += dy;
             x0 += sx;
         }
         if (e2 <= dx) { 
             err += dx;
             y0 += sy;
         }
     }
 }
 
 void draw_quarter_arc(BMPImage *img, double cx_world, double cy_world, double radius,
                       double start_ang, double end_ang,
                       Color c0, Color c1,
                       double *accumulated_dist, double total_dist,
                       double global_min_x, double global_min_y) {
 
     if (radius < 0.5 && total_dist > 0) { // If radius is tiny, draw a single pixel for the arc or skip
          // To avoid excessive d_theta or division by zero.
          // A single point might be approximated by its start or center.
          // For now, we'll let it try, but a very small radius might not draw well.
          // If radius is 0, it should ideally just update accumulated_dist if needed.
     }
     if (radius <= 0.001) { // Essentially a point, don't draw segments.
         // The total length calculation would have added M_PI/2 * radius.
         // To ensure gradient progresses if this arc has length:
         // *accumulated_dist += (M_PI/2.0) * radius; // (This is already done in main)
         return;
     }
 
 
     // Adjust d_theta based on radius for smoother curves on larger arcs without excessive points on small ones
     // A smaller d_theta means more segments.
     double d_theta = (radius > 1.0) ? (0.005 / radius) : 0.005; 
     if (d_theta > M_PI/4.0) d_theta = M_PI/4.0; // Cap d_theta to avoid too few segments
 
     double theta = start_ang;
     
     double prev_x_world = cx_world + cos(theta) * radius;
     double prev_y_world = cy_world + sin(theta) * radius;
 
     // Ensure end_ang logic handles the sweep correctly.
     // For our specific quarter turns, end_ang is always start_ang + M_PI/2.
     double loop_end_angle = end_ang;
 
     // Loop until theta effectively reaches end_ang
     while (theta < loop_end_angle - d_theta * 0.5) { // Stop just before or at end_angle
         theta += d_theta;
         // Ensure the last segment ends exactly at end_ang
         if (theta > loop_end_angle) {
             theta = loop_end_angle;
         }
 
         double current_x_world = cx_world + cos(theta) * radius;
         double current_y_world = cy_world + sin(theta) * radius;
 
         double segment_length = hypot(current_x_world - prev_x_world, current_y_world - prev_y_world);
         *accumulated_dist += segment_length;
 
         double t = 0.0;
         if (total_dist > 1e-6) { // Avoid division by zero if total_dist is effectively zero
             t = *accumulated_dist / total_dist;
         }
         if (t < 0.0) t = 0.0;
         if (t > 1.0) t = 1.0;
         
         Color current_color;
         current_color.r = (uint8_t)round(c0.r + t * (c1.r - c0.r));
         current_color.g = (uint8_t)round(c0.g + t * (c1.g - c0.g));
         current_color.b = (uint8_t)round(c0.b + t * (c1.b - c0.b));
 
        // ... inside draw_quarter_arc ...
        // Transform world coordinates to canvas coordinates
        // global_min_x is the leftmost point of the spiral in world coordinates.
        // img->width - 1 is the maximum x-index on the canvas.
        
        double translated_prev_x = prev_x_world - global_min_x;
        double translated_current_x = current_x_world - global_min_x;

        // Apply horizontal flip for the X-coordinate
        int ix0_canvas = (img->width - 1) - (int)round(translated_prev_x);
        int iy0_canvas = (int)round(prev_y_world - global_min_y); // Y-coordinate remains the same

        int ix1_canvas = (img->width - 1) - (int)round(translated_current_x);
        int iy1_canvas = (int)round(current_y_world - global_min_y); // Y-coordinate remains the same
        
        //draw_line(img, ix0_canvas, iy0_canvas, ix1_canvas, iy1_canvas, current_color);

         // Only draw if the line segment has distinct start/end points on canvas
         // or if it's the very first point of a zero-length arc to mark its color.
         if (ix0_canvas != ix1_canvas || iy0_canvas != iy1_canvas || segment_length < 1e-6 ) {
              draw_line(img, ix0_canvas, iy0_canvas, ix1_canvas, iy1_canvas, current_color);
         }
 
 
         prev_x_world = current_x_world;
         prev_y_world = current_y_world;
         
         if (theta >= loop_end_angle - 1e-9) break; // Exit if we've effectively reached end_angle
     }
 }