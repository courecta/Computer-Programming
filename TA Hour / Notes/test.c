#include <stdio.h>

void towerOfHanoi(int num_disks, int from_rod, int to_rod, int aux_rod) {
    if (num_disks == 1) {
        printf("Move disk %d to rod %d\n", num_disks, to_rod);
        return;
    }
    towerOfHanoi(num_disks - 1, from_rod, aux_rod, to_rod);
    printf("Move disk %d to rod %d\n", num_disks, to_rod);
    towerOfHanoi(num_disks - 1, aux_rod, to_rod, from_rod);
}

int main() {
    int num_disks;
    printf("Enter the disk number (2-20): ");
    scanf("%d", &num_disks);
    towerOfHanoi(num_disks, 1, 3, 2); // Assuming 3 rods (1, 2, 3)
    return 0;
}
