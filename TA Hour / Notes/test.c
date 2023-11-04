#include <stdio.h>

//Back-end complete function Template for C

// Function to find moves 
void toh(int N, int rodOne, int rodThree, int rodTwo) {
    if (N >= 1) {
        toh(N - 1, rodOne, rodTwo, rodThree);
        printf("Move disk %d to rod %d\n", N, rodThree);
        toh(N - 1, rodTwo, rodThree, rodOne);
    }
    return;
}

int main(){
    
    toh(2, 1, 2, 3);

    return 0;
}
