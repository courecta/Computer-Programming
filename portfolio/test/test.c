#include <stdio.h>
#include <stdint.h>

void catan(int64_t length, int64_t number_layer);

int main(){ 
    int64_t length = 0, number_layer = 0;

    printf("Please input the length: ");
    scanf("%ld", &length);
    printf("Please input the number of layer: ");
    scanf("%ld", &number_layer);

    int64_t width_map;

    if(length < 3 || number_layer < 1 || length > INT32_MAX || number_layer > INT32_MAX)
    {
        printf("Error!\n");
        return 1;
    }

    catan(length,number_layer);    
}

void catan(int64_t length, int64_t number_layer){
        


    return;
}