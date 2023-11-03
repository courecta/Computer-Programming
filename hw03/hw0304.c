#include "hw0304.h"

int main(){

    int16_t n = 0; // n is the number of disks
    printf("Please enter the disk number (2-20): ");
    scanf("%d", &n);
    if (n < 2 || n > 20){
        printf("Error! Please enter a valid disk amount between 2 to 20!\n");
        return 1;
    }
    
    towerOfHanoi (int32_t n);

    return 0;
}