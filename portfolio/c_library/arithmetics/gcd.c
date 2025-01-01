#include <stdio.h>

int gcd(int a, int b);

int main(void){
    printf("GCD of (24, 36, 48) = %d\n", gcd(gcd(24, 36), 48));  // Output: 12
    printf("GCD of (7, 14, 21) = %d\n", gcd(7, gcd(14, 21)));    // Output: 7
    printf("GCD of (5, 10, 15, 20) = %d\n", gcd(gcd(10, 15), 20));  // Output: 5
    return 0;
}

int gcd(int a, int b)
{
     int temp = 0;
    while(b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}