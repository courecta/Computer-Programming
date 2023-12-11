#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void swap (int32_t *num1, int32_t *num2);

int main() {
    int32_t x[] = {34, 89, 100, 7, 9, 1, 23, 233, 109, 70};
    int32_t size = sizeof(x)/sizeof(x[0]);
    for (size_t i = 0 ; i < size ; ++i ){
        for (size_t j = i + 1 ; j < size ; ++j ){
            if (x[i] > x[j]){
                swap(&x[i], &x[j]);
            }
        }
    }
    
    for (size_t i = 0 ; i < size ; i++){
    printf("%d ", x[i]);
    }

    return 0;
}
 
void swap (int32_t *num1, int32_t *num2) {
    int32_t temp;
    
    temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}