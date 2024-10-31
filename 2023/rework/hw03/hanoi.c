#include <stdio.h>
#include <stdint.h>
#include <time.h>

void recursive_hanoi(uint8_t n, int from_rod, int to_rod, int aux_rod);
void iter_hanoi(uint8_t n, int start, int end, int helper);

int main(){
    
    uint8_t n = 0;
    clock_t begin, end;
    printf("Please enter the disk number(2 - 20): ");
    scanf("%hu", &n);

    begin = clock();
    recursive_hanoi(n, 1, 3, 2);
    end = clock();
    double recursive_time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    printf("\n");

    begin = clock();
    iter_hanoi(n, 1, 3, 2);
    end = clock();
    double iterative_time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\n");

    printf("Recursive hanoi time spent: %lf\n", recursive_time_spent);
    printf("Iterative hanoi time spent: %lf\n", iterative_time_spent);

    return 0;
}

void recursive_hanoi(uint8_t n, int from_rod, int to_rod, int aux_rod)
{
    if(n == 0) return;

    recursive_hanoi(n - 1, from_rod, aux_rod, to_rod);
    printf("Move disk %u to from rod %c to rod %c\n", n, from_rod, to_rod);
    recursive_hanoi(n - 1, aux_rod, to_rod, from_rod);
}

void iter_hanoi(uint8_t n, int start, int end, int helper)
{
    // Create a converter for a pile index (0, 1 or 2) to the identifiers that are given as arguments
    int map[] = {start, end, helper};
    
    // Array to track the current position of each disk (1-based indexing for clarity)
    int diskPositions[n];
    for (int i = 1; i <= n; i++) diskPositions[i] = start; // Initially, all disks are on the start peg
    

    // Perform as many iterations as there are moves to perform:
    for (int last = 1 << n, i = 1; i < last; i++) {
        // Use bit pattern of i to determine the source peg. First remove and count the trailing 0 bits in i:
        int j = i;
        int zeroCount = 0;
        while (j % 2 == 0) {
            j >>= 1;
            zeroCount++;
        }

        // Derive the source pile from that zero-stripped number
        int source = (j >> 1) % 3;
        // Get next pile as destination
        int target = (source + 1) % 3;

        // Depending on parity, the source/target pile should be mirrored
        if ((n + zeroCount) % 2 == 0) {
            source = (3 - source) % 3;
            target = (3 - target) % 3;
        }

        // The disk being moved is determined by the number of trailing zero bits
        int disk = zeroCount + 1; // Disk number is based on the number of trailing zeros

        // Print the disk move
        printf("Move disk %d from rod %d to rod %d\n", disk, map[source], map[target]);

        // Update the position of the disk in the diskPositions array
        diskPositions[disk - 1] = map[target];
    }
}