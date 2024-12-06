#include <stdio.h>

void foo(int val, int* addr);

int main(){

    int x = 2024;
    foo(x, &x);
    printf("x = %d, &x = %p\n", x, &x);

    return 0;
}

void foo(int val, int* addr)
{
    
    printf("x = %d, &x = %p\n", val, addr);
}