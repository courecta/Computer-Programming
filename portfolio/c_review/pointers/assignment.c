#include <stdio.h>

int main(void){
    int i = 1;
    int *p = &i, *q;
    q = p;
    *q = 5;
    printf("*p = %d\n*q = %d\ni = %d\n", *p, *q, i);
    return 0;
}