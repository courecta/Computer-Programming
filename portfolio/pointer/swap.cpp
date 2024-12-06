#include <stdio.h>

void swap(int* a, int* b);

int main(){

    int a(0), b(0);
    scanf("%d%d", &a, &b);
    printf("a = %d, b = %d\n", a, b);
    swap(&a, &b);
    printf("a = %d, b = %d\n", a, b);

    return 0;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}