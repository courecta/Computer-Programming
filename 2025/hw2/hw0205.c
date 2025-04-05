#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

#define EPSILON 1e-9

// --- Vector utilities ---
typedef struct {
    double x, y, z;
} Vector3;

Vector3 vec_add(Vector3 a, Vector3 b) { return (Vector3){a.x+b.x, a.y+b.y, a.z+b.z}; }
Vector3 vec_sub(Vector3 a, Vector3 b) { return (Vector3){a.x-b.x, a.y-b.y, a.z-b.z}; }
Vector3 vec_scale(Vector3 v, double s) { return (Vector3){v.x*s, v.y*s, v.z*s}; }
double vec_dot(Vector3 a, Vector3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
Vector3 vec_cross(Vector3 a, Vector3 b) {
    return (Vector3){ a.y*b.z - a.z*b.y,
                      a.z*b.x - a.x*b.z,
                      a.x*b.y - a.y*b.x };
}
double vec_length(Vector3 v) { return sqrt(vec_dot(v, v)); }
Vector3 vec_normalize(Vector3 v) {
    double len = vec_length(v);
    if (len < EPSILON) return v;
    return vec_scale(v, 1.0/len);
}

// --- Triangle and Scene Structures ---
typedef struct {
    Vector3 color;
    Vector3 v1, v2, v3;
    Vector3 normal; // flat, normalized
} Triangle;

// Structure for a bounding box.
typedef struct {
    Vector3 min;
    Vector3 max;
} AABB;

// --- Uniform Grid Acceleration Structure ---
// We subdivide the scene's bounding box into a 3D grid of cells.
// Each cell will contain an array of triangle indices.
typedef struct {
    int *triIndices;      // dynamic array of indices
    int count;            // number of triangles in cell
    int capacity;         // capacity of the triIndices array
} GridCell;

typedef struct {
    AABB bounds;
    int nx, ny, nz;       // number of cells in each dimension
    GridCell *cells;      // cells in a flattened array: index = ix + nx*(iy + ny*iz)
} UniformGrid;

// Helper to compute cell index.
int cell_index(UniformGrid *grid, int ix, int iy, int iz) {
    return ix + grid->nx * (iy + grid->ny * iz);
}

// Add a triangle index to a cell.
void gridcell_add(GridCell *cell, int triIndex) {
    if (cell->count >= cell->capacity) {
        cell->capacity = cell->capacity ? cell->capacity * 2 : 4;
        cell->triIndices = realloc(cell->triIndices, cell->capacity * sizeof(int));
    }
    cell->triIndices[cell->count++] = triIndex;
}

// --- BMP Structures (same as before) ---
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

// Clamp function to [0,1]
double clamp(double x, double min, double max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

// --- Global Scene Data ---
Triangle *triangles = NULL;
int numTriangles;

// --- Input Parsing (similar to before) ---
void parse_input(char *fileName, int *screenWidth, int *screenHeight,
                 Vector3 *Ka, Vector3 *Kd, Vector3 *Ks,
                 Vector3 *illum, int *specExp, Vector3 *camera,
                 Vector3 *canvas_ul, Vector3 *canvas_ur, Vector3 *canvas_dl) {
    // Read from stdin
    fscanf(stdin, "%s", fileName);
    fscanf(stdin, "%d", screenWidth);
    fscanf(stdin, "%d", screenHeight);
    fscanf(stdin, "%lf %lf %lf", &Ka->x, &Ka->y, &Ka->z);
    fscanf(stdin, "%lf %lf %lf", &Kd->x, &Kd->y, &Kd->z);
    fscanf(stdin, "%lf %lf %lf", &Ks->x, &Ks->y, &Ks->z);
    fscanf(stdin, "%lf %lf %lf", &illum->x, &illum->y, &illum->z);
    fscanf(stdin, "%d", specExp);
    fscanf(stdin, "%lf %lf %lf", &camera->x, &camera->y, &camera->z);
    fscanf(stdin, "%lf %lf %lf", &canvas_ul->x, &canvas_ul->y, &canvas_ul->z);
    fscanf(stdin, "%lf %lf %lf", &canvas_ur->x, &canvas_ur->y, &canvas_ur->z);
    fscanf(stdin, "%lf %lf %lf", &canvas_dl->x, &canvas_dl->y, &canvas_dl->z);
    fscanf(stdin, "%d", &numTriangles);
}

void parse_triangles() {
    triangles = malloc(numTriangles * sizeof(Triangle));
    for (int i = 0; i < numTriangles; i++) {
        fscanf(stdin, "%lf %lf %lf", &triangles[i].color.x, &triangles[i].color.y, &triangles[i].color.z);
        fscanf(stdin, "%lf %lf %lf", &triangles[i].v1.x, &triangles[i].v1.y, &triangles[i].v1.z);
        fscanf(stdin, "%lf %lf %lf", &triangles[i].v2.x, &triangles[i].v2.y, &triangles[i].v2.z);
        fscanf(stdin, "%lf %lf %lf", &triangles[i].v3.x, &triangles[i].v3.y, &triangles[i].v3.z);
        fscanf(stdin, "%lf %lf %lf", &triangles[i].normal.x, &triangles[i].normal.y, &triangles[i].normal.z);
        triangles[i].normal = vec_normalize(triangles[i].normal);
    }
}

// --- Build Scene Bounding Box ---
AABB compute_scene_bounds() {
    AABB bounds;
    bounds.min = (Vector3){DBL_MAX, DBL_MAX, DBL_MAX};
    bounds.max = (Vector3){-DBL_MAX, -DBL_MAX, -DBL_MAX};
    for (int i = 0; i < numTriangles; i++) {
        // For each vertex of each triangle
        Vector3 verts[3] = { triangles[i].v1, triangles[i].v2, triangles[i].v3 };
        for (int j = 0; j < 3; j++) {
            if (verts[j].x < bounds.min.x) bounds.min.x = verts[j].x;
            if (verts[j].y < bounds.min.y) bounds.min.y = verts[j].y;
            if (verts[j].z < bounds.min.z) bounds.min.z = verts[j].z;
            if (verts[j].x > bounds.max.x) bounds.max.x = verts[j].x;
            if (verts[j].y > bounds.max.y) bounds.max.y = verts[j].y;
            if (verts[j].z > bounds.max.z) bounds.max.z = verts[j].z;
        }
    }
    return bounds;
}

// --- Build Uniform Grid ---
UniformGrid build_uniform_grid(int gridResolution) {
    UniformGrid grid;
    grid.bounds = compute_scene_bounds();
    // Expand bounds slightly to avoid edge issues.
    double pad = 0.001;
    grid.bounds.min = vec_sub(grid.bounds.min, (Vector3){pad, pad, pad});
    grid.bounds.max = vec_add(grid.bounds.max, (Vector3){pad, pad, pad});
    grid.nx = grid.ny = grid.nz = gridResolution;
    int totalCells = grid.nx * grid.ny * grid.nz;
    grid.cells = calloc(totalCells, sizeof(GridCell));
    // For each triangle, compute its bounding box and then determine which cells it overlaps.
    for (int i = 0; i < numTriangles; i++) {
        AABB triBox;
        triBox.min = (Vector3){ fmin(triangles[i].v1.x, fmin(triangles[i].v2.x, triangles[i].v3.x)),
                                fmin(triangles[i].v1.y, fmin(triangles[i].v2.y, triangles[i].v3.y)),
                                fmin(triangles[i].v1.z, fmin(triangles[i].v2.z, triangles[i].v3.z)) };
        triBox.max = (Vector3){ fmax(triangles[i].v1.x, fmax(triangles[i].v2.x, triangles[i].v3.x)),
                                fmax(triangles[i].v1.y, fmax(triangles[i].v2.y, triangles[i].v3.y)),
                                fmax(triangles[i].v1.z, fmax(triangles[i].v2.z, triangles[i].v3.z)) };
        // Map triangle's AABB to grid cell indices.
        int ix0 = (int)(grid.nx * ((triBox.min.x - grid.bounds.min.x) / (grid.bounds.max.x - grid.bounds.min.x)));
        int iy0 = (int)(grid.ny * ((triBox.min.y - grid.bounds.min.y) / (grid.bounds.max.y - grid.bounds.min.y)));
        int iz0 = (int)(grid.nz * ((triBox.min.z - grid.bounds.min.z) / (grid.bounds.max.z - grid.bounds.min.z)));
        int ix1 = (int)(grid.nx * ((triBox.max.x - grid.bounds.min.x) / (grid.bounds.max.x - grid.bounds.min.x)));
        int iy1 = (int)(grid.ny * ((triBox.max.y - grid.bounds.min.y) / (grid.bounds.max.y - grid.bounds.min.y)));
        int iz1 = (int)(grid.nz * ((triBox.max.z - grid.bounds.min.z) / (grid.bounds.max.z - grid.bounds.min.z)));
        if (ix1 >= grid.nx) ix1 = grid.nx - 1;
        if (iy1 >= grid.ny) iy1 = grid.ny - 1;
        if (iz1 >= grid.nz) iz1 = grid.nz - 1;
        for (int iz = iz0; iz <= iz1; iz++) {
            for (int iy = iy0; iy <= iy1; iy++) {
                for (int ix = ix0; ix <= ix1; ix++) {
                    int index = cell_index(&grid, ix, iy, iz);
                    gridcell_add(&grid.cells[index], i);
                }
            }
        }
    }
    return grid;
}

// Modified Ray-Triangle Intersection with Relaxed Barycentrics
int ray_triangle_intersect(Vector3 rayOrig, Vector3 rayDir, Triangle *tri, double *tOut) {
    // Backface culling: if the ray hits the back face, skip it.
    if (vec_dot(tri->normal, rayDir) > EPSILON)
        return 0;
    
    double denom = vec_dot(tri->normal, rayDir);
    if (fabs(denom) < EPSILON)
        return 0;
    
    Vector3 EV1 = vec_sub(tri->v1, rayOrig);
    double t = vec_dot(tri->normal, EV1) / denom;
    if (t < EPSILON)
        return 0;
    
    Vector3 P = vec_add(rayOrig, vec_scale(rayDir, t));
    
    // Compute barycentric coordinates.
    Vector3 a = tri->v1, b = tri->v2, c = tri->v3;
    Vector3 v0 = vec_sub(b, a);
    Vector3 v1 = vec_sub(c, a);
    Vector3 v2 = vec_sub(P, a);
    double d00 = vec_dot(v0, v0);
    double d01 = vec_dot(v0, v1);
    double d11 = vec_dot(v1, v1);
    double d20 = vec_dot(v2, v0);
    double d21 = vec_dot(v2, v1);
    double denomBary = d00 * d11 - d01 * d01;
    if (fabs(denomBary) < EPSILON)
        return 0;
    
    double v_param = (d11 * d20 - d01 * d21) / denomBary;
    double w_param = (d00 * d21 - d01 * d20) / denomBary;
    double u_param = 1.0 - v_param - w_param;
    
    // Use a relaxed tolerance to allow near-edge hits.
    double tol = 1e-4;
    if (u_param < -tol || v_param < -tol || w_param < -tol ||
        u_param > 1+tol || v_param > 1+tol || w_param > 1+tol)
        return 0;
    
    *tOut = t;
    return 1;
}

// --- Modified Ray Traversal through Uniform Grid using 3D DDA ---
int traverse_grid(UniformGrid *grid, Vector3 rayOrig, Vector3 rayDir, double t_min_global, int *hitTri, double *hitT) {
    // Compute entry and exit t for overall grid bounds.
    double tmin = -DBL_MAX, tmax = DBL_MAX;
    for (int i = 0; i < 3; i++) {
        double origin, dir, bmin, bmax;
        if (i==0) { origin = rayOrig.x; dir = rayDir.x; bmin = grid->bounds.min.x; bmax = grid->bounds.max.x; }
        else if (i==1) { origin = rayOrig.y; dir = rayDir.y; bmin = grid->bounds.min.y; bmax = grid->bounds.max.y; }
        else { origin = rayOrig.z; dir = rayDir.z; bmin = grid->bounds.min.z; bmax = grid->bounds.max.z; }
        double t1 = (bmin - origin) / (fabs(dir) < EPSILON ? 1 : dir);
        double t2 = (bmax - origin) / (fabs(dir) < EPSILON ? 1 : dir);
        if (t1 > t2) { double tmp = t1; t1 = t2; t2 = tmp; }
        if (t1 > tmin) tmin = t1;
        if (t2 < tmax) tmax = t2;
    }
    if (tmax < tmin || tmax < 0)
        return 0; // Ray misses grid.
    
    // Start at t = max(tmin, 0)
    double t = tmin < 0 ? 0 : tmin;
    Vector3 pos = vec_add(rayOrig, vec_scale(rayDir, t));
    // Compute initial cell indices.
    int ix = (int)(grid->nx * ((pos.x - grid->bounds.min.x) / (grid->bounds.max.x - grid->bounds.min.x)));
    int iy = (int)(grid->ny * ((pos.y - grid->bounds.min.y) / (grid->bounds.max.y - grid->bounds.min.y)));
    int iz = (int)(grid->nz * ((pos.z - grid->bounds.min.z) / (grid->bounds.max.z - grid->bounds.min.z)));
    if (ix < 0) ix = 0; if (ix >= grid->nx) ix = grid->nx-1;
    if (iy < 0) iy = 0; if (iy >= grid->ny) iy = grid->ny-1;
    if (iz < 0) iz = 0; if (iz >= grid->nz) iz = grid->nz-1;
    
    // Determine step and tDelta for DDA.
    int stepX = (rayDir.x >= 0) ? 1 : -1;
    int stepY = (rayDir.y >= 0) ? 1 : -1;
    int stepZ = (rayDir.z >= 0) ? 1 : -1;
    
    double cellSizeX = (grid->bounds.max.x - grid->bounds.min.x) / grid->nx;
    double cellSizeY = (grid->bounds.max.y - grid->bounds.min.y) / grid->ny;
    double cellSizeZ = (grid->bounds.max.z - grid->bounds.min.z) / grid->nz;
    
    double nextX = grid->bounds.min.x + (ix + (stepX > 0 ? 1 : 0)) * cellSizeX;
    double nextY = grid->bounds.min.y + (iy + (stepY > 0 ? 1 : 0)) * cellSizeY;
    double nextZ = grid->bounds.min.z + (iz + (stepZ > 0 ? 1 : 0)) * cellSizeZ;
    
    double tMaxX = (fabs(rayDir.x) < EPSILON) ? DBL_MAX : (nextX - rayOrig.x) / rayDir.x;
    double tMaxY = (fabs(rayDir.y) < EPSILON) ? DBL_MAX : (nextY - rayOrig.y) / rayDir.y;
    double tMaxZ = (fabs(rayDir.z) < EPSILON) ? DBL_MAX : (nextZ - rayOrig.z) / rayDir.z;
    
    double tDeltaX = (fabs(rayDir.x) < EPSILON) ? DBL_MAX : cellSizeX / fabs(rayDir.x);
    double tDeltaY = (fabs(rayDir.y) < EPSILON) ? DBL_MAX : cellSizeY / fabs(rayDir.y);
    double tDeltaZ = (fabs(rayDir.z) < EPSILON) ? DBL_MAX : cellSizeZ / fabs(rayDir.z);
    
    // Initialize closest hit.
    *hitT = DBL_MAX;
    *hitTri = -1;
    
    // Traverse the grid cells until t exceeds tmax.
    while (t <= tmax) {
        int index = cell_index(grid, ix, iy, iz);
        // Test ray against triangles in this cell.
        for (int i = 0; i < grid->cells[index].count; i++) {
            int triIndex = grid->cells[index].triIndices[i];
            double t_hit;
            if (ray_triangle_intersect(rayOrig, rayDir, &triangles[triIndex], &t_hit)) {
                if (t_hit < *hitT && t_hit >= t_min_global) {
                    *hitT = t_hit;
                    *hitTri = triIndex;
                }
            }
        }
        
        // Advance to next cell.
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                ix += stepX;
                t = tMaxX;
                tMaxX += tDeltaX;
            } else {
                iz += stepZ;
                t = tMaxZ;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                iy += stepY;
                t = tMaxY;
                tMaxY += tDeltaY;
            } else {
                iz += stepZ;
                t = tMaxZ;
                tMaxZ += tDeltaZ;
            }
        }
        // Check if we're outside the grid.
        if (ix < 0 || ix >= grid->nx || iy < 0 || iy >= grid->ny || iz < 0 || iz >= grid->nz)
            break;
    }
    return (*hitTri != -1);
}

// --- Main Rendering Function ---
int main() {
    char fileName[256];
    int screenWidth, screenHeight;
    Vector3 Ka, Kd, Ks;
    Vector3 illum;
    int specExp;
    Vector3 camera;
    Vector3 canvas_ul, canvas_ur, canvas_dl;
    
    // Parse global input parameters.
    parse_input(fileName, &screenWidth, &screenHeight, &Ka, &Kd, &Ks, &illum, &specExp, &camera, &canvas_ul, &canvas_ur, &canvas_dl);
    
    // Parse triangle surfaces.
    parse_triangles();
    
    // Build uniform grid with a chosen resolution (tune this parameter as needed).
    int gridResolution = 50; // e.g., 50x50x50 cells.
    UniformGrid grid = build_uniform_grid(gridResolution);
    
    // Allocate image buffer.
    unsigned char* image = malloc(screenWidth * screenHeight * 3);
    memset(image, 0, screenWidth * screenHeight * 3);
    
    // Compute canvas basis vectors.
    Vector3 canvas_u = vec_sub(canvas_ur, canvas_ul);
    Vector3 canvas_v = vec_sub(canvas_dl, canvas_ul);
    
    // For each pixel, cast a ray and traverse the grid.
    for (int j = 0; j < screenHeight; j++) {
        for (int i = 0; i < screenWidth; i++) {
            double u = (i + 0.5) / screenWidth;
            double v = (j + 0.5) / screenHeight;
            Vector3 pixelPos = vec_add(canvas_ul, vec_add(vec_scale(canvas_u, u), vec_scale(canvas_v, v)));
            Vector3 rayDir = vec_normalize(vec_sub(pixelPos, camera));
            
            double t_hit;
            int hitTri;
            // Use the grid traversal to get an intersection.
            int hit = traverse_grid(&grid, camera, rayDir, 0, &hitTri, &t_hit);
            
            double r_final = 0.5, g_final = 0.5, b_final = 0.5;
            if (hit && hitTri != -1) {
                Triangle *tri = &triangles[hitTri];
                Vector3 P = vec_add(camera, vec_scale(rayDir, t_hit));
                double bias = 1e-4;
                Vector3 P_bias = vec_add(P, vec_scale(tri->normal, bias));
    
                // Ambient component (always added)
                double Ia_r = Ka.x * tri->color.x;
                double Ia_g = Ka.y * tri->color.y;
                double Ia_b = Ka.z * tri->color.z;
    
                // --- Shadow Check Needed Here ---
                int in_shadow = 0;
                Vector3 L = vec_normalize(vec_sub(illum, P_bias)); // Direction to light
                double light_dist = vec_length(vec_sub(illum, P_bias)); // Distance to light
    
                // Cast shadow ray from P_bias towards light
                int shadowHitTri;
                double shadowT;
                // Use t_min_global = 0 (or a tiny epsilon > 0) to avoid hitting itself immediately
                // Use traverse_grid to check for intersections along the shadow ray
                if (traverse_grid(&grid, P_bias, L, EPSILON, &shadowHitTri, &shadowT)) {
                    // Check if the hit occurs *before* the light source
                    if (shadowT < light_dist) {
                        in_shadow = 1; // Point is occluded
                    }
                }
                // --- End Shadow Check ---
    
                double Id_r = 0, Id_g = 0, Id_b = 0;
                double Is_r = 0, Is_g = 0, Is_b = 0;
    
                // Only calculate diffuse and specular if NOT in shadow
                if (!in_shadow) {
                    // Diffuse component
                    double NdotL = vec_dot(tri->normal, L);
                    if (NdotL < 0) NdotL = 0; // Light doesn't hit from behind
                    Id_r = Kd.x * NdotL * tri->color.x;
                    Id_g = Kd.y * NdotL * tri->color.y;
                    Id_b = Kd.z * NdotL * tri->color.z;
    
                    // Specular component
                    Vector3 R = vec_normalize(vec_sub(vec_scale(tri->normal, 2 * vec_dot(tri->normal, L)), L));
                    Vector3 V = vec_normalize(vec_sub(camera, P_bias)); // Use P_bias here too
                    double RdotV = vec_dot(R, V);
                    if (RdotV < 0) RdotV = 0;
                    double specFactor = pow(RdotV, specExp);
                    Is_r = Ks.x * specFactor * tri->color.x; // Specular reflects light color, not object color usually, but depends on model
                    Is_g = Ks.y * specFactor * tri->color.y; // Ks components often multiply light intensity, not object color
                    Is_b = Ks.z * specFactor * tri->color.z; // Consider changing Ks * specFactor * (light color, e.g., {1,1,1}) if needed
                }
    
                // Sum components
                r_final = clamp(Ia_r + Id_r + Is_r, 0, 1);
                g_final = clamp(Ia_g + Id_g + Is_g, 0, 1);
                b_final = clamp(Ia_b + Id_b + Is_b, 0, 1);
            }
            int index = (j * screenWidth + i) * 3;
            // BMP uses BGR order.
            image[index+2] = (unsigned char)(r_final * 255);
            image[index+1] = (unsigned char)(g_final * 255);
            image[index+0] = (unsigned char)(b_final * 255);
        }
    }   
    // Write BMP file.
    FILE* fp = fopen(fileName, "wb");
    if (!fp) { fprintf(stderr, "Error opening file for output\n"); return 1; }
    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    int rowSize = (screenWidth * 3 + 3) & (~3);
    int imageSize = rowSize * screenHeight;
    fileHeader.bfType = 0x4D42;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    fileHeader.bfSize = fileHeader.bfOffBits + imageSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = screenWidth;
    infoHeader.biHeight = screenHeight;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = imageSize;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;
    fwrite(&fileHeader, sizeof(fileHeader), 1, fp);
    fwrite(&infoHeader, sizeof(infoHeader), 1, fp);
    unsigned char *row = malloc(rowSize);
    for (int j = screenHeight-1; j >= 0; j--) {
        memset(row, 0, rowSize);
        memcpy(row, &image[j*screenWidth*3], screenWidth*3);
        fwrite(row, 1, rowSize, fp);
    }
    free(row);
    fclose(fp);
    
    // Cleanup: free grid cells.
    int totalCells = grid.nx * grid.ny * grid.nz;
    for (int i = 0; i < totalCells; i++) {
        free(grid.cells[i].triIndices);
    }
    free(grid.cells);
    free(image);
    free(triangles);
    
    printf("Image successfully written to %s\n", fileName);
    return 0;
}