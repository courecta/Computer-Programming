#include <stdio.h>
#include <stdarg.h>

int gcd(int n, ...);
int euclidean(int a, int b);

int main(void){
    printf("GCD of (24, 36, 48) = %d\n", gcd(3, 24, 36, 48));  // Output: 12
    printf("GCD of (7, 14, 21) = %d\n", gcd(3, 7, 14, 21));    // Output: 7
    printf("GCD of (5, 10, 15, 20) = %d\n", gcd(4, 5, 10, 15, 20));  // Output: 5
}

int gcd(int n, ...)
{
    va_list args;
    va_start(args, n);

    int result = va_arg(args, int); // first element of VLA list
    for(int i = 1; i < n; i++)
    {
        int next = va_arg(args, int);
        result = euclidean(result, next);
        if(result == 1) break;
    }
    va_end(args);
    return result;
}

int euclidean(int a, int b)
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