#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <curl/curl.h>
#include <json-c/json.h>

// BMP file header structures
#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t size_image;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
} BMPInfoHeader;
#pragma pack(pop)

// RGB color structure
typedef struct {
    uint8_t b, g, r;
} RGB;

// YouBike station structure
typedef struct {
    double lat, lng;
    int available_rent_bikes;
    int available_return_bikes;
    int total;
} Station;

// Response structure for curl
struct APIResponse {
    char *data;
    size_t size;
};

// Map boundaries (from the problem description)
#define LAT_MIN 24.9605
#define LAT_MAX 25.2102
#define LNG_MIN 121.4571
#define LNG_MAX 121.6659

// Color definitions
RGB COLOR_RED = {0, 0, 255};
RGB COLOR_ORANGE = {0, 165, 255};
RGB COLOR_YELLOW = {0, 255, 255};
RGB COLOR_GREEN = {0, 255, 0};

// Global variables
int ratio = 1;
char output_file[256] = "output.bmp";
int mode = 0; // 0: available, 1: vacancy, 2: total

// Function prototypes
void print_help(void);
int parse_arguments(int argc, char *argv[]);
size_t write_callback(void *contents, size_t size, size_t nmemb, struct APIResponse *response);
char *fetch_youbike_data(void);
int parse_youbike_json(const char *json_data, Station **stations);
int load_bmp(const char *filename, RGB **image, int *width, int *height);
int save_bmp(const char *filename, RGB *image, int width, int height);
void draw_circle(RGB *image, int img_width, int img_height, int x, int y, int radius, RGB color);
RGB get_color_by_value(int value, int mode);
void process_stations(RGB *image, int img_width, int img_height, Station *stations, int station_count);

void print_help(void) {
    printf("Usage : fin02 [ options ]\n");
    printf("-r, --ratio=[1-5]\n");
    printf("Setup the output image ratio (integer).\n");
    printf("If the ratio is 2, the width and the height of the output image should be\n");
    printf("twice than the original image.\n");
    printf("Default : 1.\n");
    printf("-o, --output=<file name>\n");
    printf("Setup the output file name.\n");
    printf("Default : output.bmp.\n");
    printf("-a, --available\n");
    printf("Draw the available number (available_rent_bikes).\n");
    printf("This is the default option.\n");
    printf("-v, --vacancy\n");
    printf("Draw the available return number (available_return_bikes).\n");
    printf("-t, --total\n");
    printf("Draw the total number (total).\n");
    printf("-h, --help\n");
    printf("Display this information and exit.\n");
}

int parse_arguments(int argc, char *argv[]) {
    int opt;
    static struct option long_options[] = {
        {"ratio", required_argument, 0, 'r'},
        {"output", required_argument, 0, 'o'},
        {"available", no_argument, 0, 'a'},
        {"vacancy", no_argument, 0, 'v'},
        {"total", no_argument, 0, 't'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "r:o:avth", long_options, NULL)) != -1) {
        switch (opt) {
            case 'r':
                ratio = atoi(optarg);
                if (ratio < 1 || ratio > 5) {
                    fprintf(stderr, "Error: ratio must be between 1 and 5\n");
                    return -1;
                }
                break;
            case 'o':
                strncpy(output_file, optarg, sizeof(output_file) - 1);
                output_file[sizeof(output_file) - 1] = '\0';
                break;
            case 'a':
                mode = 0;
                break;
            case 'v':
                mode = 1;
                break;
            case 't':
                mode = 2;
                break;
            case 'h':
                print_help();
                exit(0);
            default:
                fprintf(stderr, "Use -h or --help for usage information\n");
                return -1;
        }
    }
    return 0;
}

size_t write_callback(void *contents, size_t size, size_t nmemb, struct APIResponse *response) {
    size_t real_size = size * nmemb;
    char *ptr = realloc(response->data, response->size + real_size + 1);
    
    if (!ptr) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    
    response->data = ptr;
    memcpy(&(response->data[response->size]), contents, real_size);
    response->size += real_size;
    response->data[response->size] = 0;
    
    return real_size;
}

char *fetch_youbike_data(void) {
    CURL *curl;
    CURLcode res;
    struct APIResponse response = {0};
    
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return NULL;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "https://tcgbusfs.blob.core.windows.net/dotapp/youbike/v2/youbike_immediate.json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        if (response.data) free(response.data);
        return NULL;
    }
    
    return response.data;
}

int parse_youbike_json(const char *json_data, Station **stations) {
    json_object *root = json_tokener_parse(json_data);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON\n");
        return -1;
    }
    
    if (!json_object_is_type(root, json_type_array)) {
        fprintf(stderr, "JSON root is not an array\n");
        json_object_put(root);
        return -1;
    }
    
    int station_count = json_object_array_length(root);
    *stations = malloc(station_count * sizeof(Station));
    if (!*stations) {
        fprintf(stderr, "Memory allocation failed\n");
        json_object_put(root);
        return -1;
    }
    
    int valid_stations = 0;
    for (int i = 0; i < station_count; i++) {
        json_object *station_obj = json_object_array_get_idx(root, i);
        if (!station_obj) continue;
        
        json_object *lat_obj, *lng_obj, *available_obj, *vacancy_obj, *total_obj;
        
        if (json_object_object_get_ex(station_obj, "latitude", &lat_obj) &&
            json_object_object_get_ex(station_obj, "longitude", &lng_obj) &&
            json_object_object_get_ex(station_obj, "available_rent_bikes", &available_obj) &&
            json_object_object_get_ex(station_obj, "available_return_bikes", &vacancy_obj) &&
            json_object_object_get_ex(station_obj, "total", &total_obj)) {
            
            Station *s = &(*stations)[valid_stations];
            s->lat = json_object_get_double(lat_obj);
            s->lng = json_object_get_double(lng_obj);
            s->available_rent_bikes = json_object_get_int(available_obj);
            s->available_return_bikes = json_object_get_int(vacancy_obj);
            s->total = json_object_get_int(total_obj);
            
            // Filter stations within Taipei boundaries
            if (s->lat >= LAT_MIN && s->lat <= LAT_MAX && 
                s->lng >= LNG_MIN && s->lng <= LNG_MAX) {
                valid_stations++;
            }
        }
    }
    
    json_object_put(root);
    return valid_stations;
}

int load_bmp(const char *filename, RGB **image, int *width, int *height) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return -1;
    }
    
    BMPHeader header;
    BMPInfoHeader info_header;
    
    fread(&header, sizeof(BMPHeader), 1, file);
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);
    
    if (header.type != 0x4D42) {
        fprintf(stderr, "Not a valid BMP file\n");
        fclose(file);
        return -1;
    }
    
    *width = info_header.width;
    *height = abs(info_header.height);
    
    fseek(file, header.offset, SEEK_SET);
    
    *image = malloc(*width * *height * sizeof(RGB));
    if (!*image) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return -1;
    }
    
    int padding = (4 - (*width * 3) % 4) % 4;
    
    for (int y = *height - 1; y >= 0; y--) {
        fread(&(*image)[y * *width], sizeof(RGB), *width, file);
        fseek(file, padding, SEEK_CUR);
    }
    
    fclose(file);
    return 0;
}

int save_bmp(const char *filename, RGB *image, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Cannot create file: %s\n", filename);
        return -1;
    }
    
    int padding = (4 - (width * 3) % 4) % 4;
    int data_size = (width * 3 + padding) * height;
    
    BMPHeader header = {
        .type = 0x4D42,
        .size = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + data_size,
        .reserved1 = 0,
        .reserved2 = 0,
        .offset = sizeof(BMPHeader) + sizeof(BMPInfoHeader)
    };
    
    BMPInfoHeader info_header = {
        .size = sizeof(BMPInfoHeader),
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 24,
        .compression = 0,
        .size_image = data_size,
        .x_pixels_per_meter = 0,
        .y_pixels_per_meter = 0,
        .colors_used = 0,
        .colors_important = 0
    };
    
    fwrite(&header, sizeof(BMPHeader), 1, file);
    fwrite(&info_header, sizeof(BMPInfoHeader), 1, file);
    
    uint8_t pad_bytes[3] = {0, 0, 0};
    for (int y = height - 1; y >= 0; y--) {
        fwrite(&image[y * width], sizeof(RGB), width, file);
        fwrite(pad_bytes, 1, padding, file);
    }
    
    fclose(file);
    return 0;
}

void draw_circle(RGB *image, int img_width, int img_height, int cx, int cy, int radius, RGB color) {
    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            if (x >= 0 && x < img_width && y >= 0 && y < img_height) {
                int dx = x - cx;
                int dy = y - cy;
                if (dx * dx + dy * dy <= radius * radius) {
                    image[y * img_width + x] = color;
                }
            }
        }
    }
}

RGB get_color_by_value(int value, int mode) {
    if (mode == 2) { // total mode
        if (value == 0) return COLOR_RED;
        else if (value <= 10) return COLOR_ORANGE;
        else if (value <= 20) return COLOR_YELLOW;
        else return COLOR_GREEN;
    } else { // available or vacancy mode
        if (value == 0) return COLOR_RED;
        else if (value <= 5) return COLOR_ORANGE;
        else if (value <= 10) return COLOR_YELLOW;
        else return COLOR_GREEN;
    }
}

void process_stations(RGB *image, int img_width, int img_height, Station *stations, int station_count) {
    // Draw circles in color order: red, orange, yellow, green (red on top)
    RGB colors[] = {COLOR_GREEN, COLOR_YELLOW, COLOR_ORANGE, COLOR_RED};
    
    int total_circles_drawn = 0;
    
    for (int color_idx = 0; color_idx < 4; color_idx++) {
        int circles_this_color = 0;
        
        for (int i = 0; i < station_count; i++) {
            Station *s = &stations[i];
            
            // Check if station is within boundaries first
            if (s->lat < LAT_MIN || s->lat > LAT_MAX || s->lng < LNG_MIN || s->lng > LNG_MAX) {
                continue;
            }
            
            int value;
            switch (mode) {
                case 0: value = s->available_rent_bikes; break;
                case 1: value = s->available_return_bikes; break;
                case 2: value = s->total; break;
                default: value = s->available_rent_bikes; break;
            }
            
            RGB station_color = get_color_by_value(value, mode);
            
            // Only draw if this station's color matches current color being drawn
            if (station_color.r == colors[color_idx].r && 
                station_color.g == colors[color_idx].g && 
                station_color.b == colors[color_idx].b) {
                
                // Convert lat/lng to pixel coordinates
                double x_ratio = (s->lng - LNG_MIN) / (LNG_MAX - LNG_MIN);
                double y_ratio = (LAT_MAX - s->lat) / (LAT_MAX - LAT_MIN);
                
                int x = (int)(x_ratio * img_width);
                int y = (int)(y_ratio * img_height);
                
                draw_circle(image, img_width, img_height, x, y, 5 * ratio, station_color);
                circles_this_color++;
                total_circles_drawn++;
            }
        }
    }
    
    printf("Drew %d station markers\n", total_circles_drawn);
}

// Add this debug function after your existing functions

void debug_stations(Station *stations, int count) {
    printf("=== DEBUG: Station Information ===\n");
    printf("Total stations found: %d\n", count);
    
    if (count == 0) {
        printf("No stations found! Check API response or filtering.\n");
        return;
    }
    
    printf("First 5 stations:\n");
    for (int i = 0; i < (count < 5 ? count : 5); i++) {
        printf("Station %d:\n", i+1);
        printf("  Lat: %.6f, Lng: %.6f\n", stations[i].lat, stations[i].lng);
        printf("  Available bikes: %d\n", stations[i].available_rent_bikes);
        printf("  Available spots: %d\n", stations[i].available_return_bikes);
        printf("  Total: %d\n", stations[i].total);
        
        // Check if within boundaries
        if (stations[i].lat >= LAT_MIN && stations[i].lat <= LAT_MAX &&
            stations[i].lng >= LNG_MIN && stations[i].lng <= LNG_MAX) {
            printf("  ✓ Within map boundaries\n");
        } else {
            printf("  ✗ Outside map boundaries\n");
        }
        printf("\n");
    }
}

// Add this coordinate mapping debug function
void debug_coordinates(Station *stations, int count, int img_width, int img_height) {
    printf("=== DEBUG: Coordinate Mapping ===\n");
    printf("Image size: %dx%d\n", img_width, img_height);
    printf("Map boundaries: Lat[%.3f-%.3f], Lng[%.3f-%.3f]\n", 
           LAT_MIN, LAT_MAX, LNG_MIN, LNG_MAX);
    
    for (int i = 0; i < (count < 3 ? count : 3); i++) {
        if (stations[i].lat >= LAT_MIN && stations[i].lat <= LAT_MAX &&
            stations[i].lng >= LNG_MIN && stations[i].lng <= LNG_MAX) {
            
            int x = (int)((stations[i].lng - LNG_MIN) / (LNG_MAX - LNG_MIN) * img_width);
            int y = (int)((LAT_MAX - stations[i].lat) / (LAT_MAX - LAT_MIN) * img_height);
            
            printf("Station %d: (%.6f, %.6f) -> pixel (%d, %d)\n", 
                   i+1, stations[i].lat, stations[i].lng, x, y);
        }
    }
}

int main(int argc, char *argv[]) {
    if (parse_arguments(argc, argv) != 0) {
        return 1;
    }
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Load the base map
    RGB *image;
    int orig_width, orig_height;
    if (load_bmp("taipei_map_white_bg.bmp", &image, &orig_width, &orig_height) != 0) {
        curl_global_cleanup();
        return 1;
    }
    printf("Loaded base map: %dx%d pixels\n", orig_width, orig_height);
    
    // Scale image if ratio > 1
    int new_width = orig_width * ratio;
    int new_height = orig_height * ratio;
    
    if (ratio > 1) {
        printf("Scaling image to %dx%d (ratio: %d)\n", new_width, new_height, ratio);
        RGB *scaled_image = malloc(new_width * new_height * sizeof(RGB));
        if (!scaled_image) {
            fprintf(stderr, "Memory allocation failed for scaled image\n");
            free(image);
            curl_global_cleanup();
            return 1;
        }
        
        // Simple nearest neighbor scaling
        for (int y = 0; y < new_height; y++) {
            for (int x = 0; x < new_width; x++) {
                int orig_x = x / ratio;
                int orig_y = y / ratio;
                scaled_image[y * new_width + x] = image[orig_y * orig_width + orig_x];
            }
        }
        
        free(image);
        image = scaled_image;
    }
    
    // Fetch YouBike data
    printf("Fetching YouBike data...\n");
    char *json_data = fetch_youbike_data();
    if (!json_data) {
        fprintf(stderr, "Failed to fetch YouBike data\n");
        free(image);
        curl_global_cleanup();
        return 1;
    }
    
    // Parse JSON and get stations
    Station *stations;
    int station_count = parse_youbike_json(json_data, &stations);
    free(json_data);
    
    if (station_count < 0) {
        fprintf(stderr, "Failed to parse JSON data\n");
        free(image);
        curl_global_cleanup();
        return 1;
    }
    
    printf("Successfully parsed %d YouBike stations\n", station_count);
    
    if (station_count == 0) {
        printf("Warning: No stations found within map boundaries\n");
    }
    
    // Process stations and draw circles
    printf("Drawing station markers...\n");
    process_stations(image, new_width, new_height, stations, station_count);
    
    // Save output
    if (save_bmp(output_file, image, new_width, new_height) != 0) {
        fprintf(stderr, "Failed to save output file\n");
        free(image);
        free(stations);
        curl_global_cleanup();
        return 1;
    }
    
    printf("Output saved to %s\n", output_file);
    
    // Cleanup
    free(image);
    free(stations);
    curl_global_cleanup();
    
    return 0;
}