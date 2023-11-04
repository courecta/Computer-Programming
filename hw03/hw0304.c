#include "hw0304.h"

int main(){

    int32_t n = 0; // n is the number of disks
    int32_t rodOne = 1, rodTwo = 2, rodThree = 3; // initializes the three rods as integers
    printf("Please enter the disk number (2-20): ");
    scanf("%d", &n);
    if (n < 2 || n > 20){
        printf("Error! Please enter a valid disk amount between 2 to 20!\n");
        return 1;
    }
    
    towerOfHanoi ( n, rodOne, rodThree, rodTwo);

    return 0;
}