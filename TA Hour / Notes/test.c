#include <stdio.h>

//Back-end complete function Template for C

// Function to find moves 
void toh(int N, int rodOne, int rodThree, int rodTwo) {
    int total_moves = (1 << N) - 1;  // Total number of moves required

    for (int i = 1; i <= total_moves; i++) {
        int n = i & (((i - 1) % 3) + 1);
        
        if (i % 3 == 1) {
            printf("Move disk %d to rod %d\n", n, rodThree);
        } else if (i % 3 == 2) {
            printf("Move disk %d to rod %d\n", n, rodTwo);
        } else if (i % 3 == 0) {
            printf("Move disk %d to rod %d\n", n, rodThree);
        }
    }

    return;
}

int main(){
    
    toh(2, 1, 2, 3);

    return 0;
}
