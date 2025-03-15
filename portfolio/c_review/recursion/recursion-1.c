#include <stdio.h>

// Recurrence relation: T(n) = 1 + T(n - 1) + T(n - 3), for n >= 1
// F(0 || negative integer) = 1

void get(int n);

int called = 1;

void get(int n)
{
    if(n < 1) return;
    get(n - 1);
    get(n - 3);
    called += printf("%d ", n);
    printf("Called #%d\n", called);
}

int main(void){
    get(6);
    printf("Total called: %d\n", called);
}