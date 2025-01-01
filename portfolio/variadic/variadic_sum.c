#include <stdio.h>
#include <stdarg.h>

int sum(int n, ...);

int main(void){
    printf("Sum of %d + %d + %d = %d\n", 1, 2, 3, sum(3, 1, 2, 3));
    printf("Sum of %d + %d + %d + %d = %d\n", 4, 5, 6, 7, sum(4, 4, 5, 6, 7));
}

int sum(int n, ...)
{
    va_list args;
    va_start(args, n);

    int total = 0;
    for(int i = 0; i < n; i++)
    {
        total += va_arg(args, int);
    }

    va_end(args);
    return total;
}