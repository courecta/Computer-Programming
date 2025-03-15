#include <stdio.h>

int main(void){
    int a[] = {5, 16, 7, 89, 45, 32, 23, 10};
    int *p = &a[1];
    int *q = &a[5];

    printf("%d ", *(p + 3));    // p + 3 = 45
    printf("%d ", *(q - 3));    // q - 3 = 7
    printf("%d ", q - p);       // q - p = 5 - 1 = 4
    printf("%d ", p < q);       // p < q = 1 < 5 = true
    printf("%d ", *p < *q);     // *p < *q = 16 < 32 = true 
    printf("\n");
    return 0;
}