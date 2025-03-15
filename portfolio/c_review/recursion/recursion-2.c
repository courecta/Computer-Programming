#include <stdio.h>

// Recurrence relation: T(n) = T(n - 1) + n, for n > 1
// T(1) = 1
// Thus, it can be modeled with the close-form of T(n) = n(n + 1)/2

int printed = 0;

void fun1(int n);

void fun1(int n)
{
    int i = 0;
    if(n > 1)
        fun1(n - 1);
    for(i = 0; i < n; i++)
        printed += printf(" * ") - 2;
}

int main(void){
    int n = 10;
    fun1(n);
    printf("\n");
    printf("Stars printed %d\n", printed);
}