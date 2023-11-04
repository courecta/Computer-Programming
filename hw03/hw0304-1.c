#include "hw0304.h"

void towerOfHanoi(int n, int rodOne, int rodThree, int rodTwo) {
    if (n >= 1) {
        towerOfHanoi(n - 1, rodOne, rodTwo, rodThree);
        printf("Move disk %d to rod %d\n", n, rodThree);
        towerOfHanoi(n - 1, rodTwo, rodThree, rodOne);
    }
    return;
}