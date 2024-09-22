#include <stdio.h>
#include <stdint.h>

void hanoi(uint8_t n);

int main(){
    
    uint8_t int n = 0;
    printf("Please enter the disk number(2 - 20): ");
    scanf("%hu");

    hanoi(n);

    return 0;
}

void hanoi(uint8_t n)
{



    return 0;
}

/*
n = 154,321
temp = n
while(temp >= 10)
{
    temp /= 10;
    digits++;
}


*/