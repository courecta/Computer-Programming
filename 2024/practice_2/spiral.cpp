#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 101  // Max size for demonstration purposes; larger values are impractical

int main() {
    int sz, p;
    while (1) {
        scanf("%d %d", &sz, &p);
        if (sz == 0 && p == 0) break;

        // Guard against large sizes (this approach is limited by memory constraints)
        if (sz > MAX_SIZE) {
            printf("Size too large for array-based approach.\n");
            continue;
        }

        int grid[MAX_SIZE][MAX_SIZE] = {0}; // Initialize a 2D array with zeroes

        // Start filling from the center of the grid
        int x = sz / 2, y = sz / 2;
        int current_num = 1;  // Start counting from 1
        grid[x][y] = current_num;

        // Spiral directions (left, down, right, up)
        int dx[] = {0, 1, 0, -1};
        int dy[] = {-1, 0, 1, 0};

        int steps = 1;   // Initial number of steps in each direction
        int direction = 0; // Starting direction: left

        while (current_num < sz * sz) {
            for (int i = 0; i < 2; i++) { // Two turns for each step increase
                for (int j = 0; j < steps; j++) {
                    x += dx[direction];
                    y += dy[direction];
                    current_num++;
                    grid[x][y] = current_num;
                    if (current_num == p) {
                        // Output the result when we reach the desired position
                        printf("Line = %d, column = %d.\n", sz - x, y + 1);
                    }
                }
                direction = (direction + 1) % 4; // Change direction
            }
            steps++; // Increase step size after two turns
        }
    }
    return 0;
}
