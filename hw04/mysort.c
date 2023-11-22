#include "mysort.h"

void mysort( int32_t array[], int32_t size ){
    if (size <= 1){
        return;
    }
    int32_t even[size], odd[size], e = 0, o = 0;
    
    for(int32_t i = 0 ; i < size ; i++){
        if (array[i] % 2 == 0){
            even[e] = array[i];
            e++;
        }
        else if (array[i] % 2 == 1){
            odd[o] = array[i];
            o++;
        }
    }

    for(int32_t i = 0 ; i < e ; i++){
        for(int32_t j = i + 1 ; j < e ; j++){
            if (even[i] < even[j]){
                int32_t temp = even[i];
                even[i] = even[j];
                even[j] = temp;
            }
        }    
    }
    
    for(int32_t i = 0 ; i < e ; i++){
        array[i] = even[i];
    }

    for(int32_t i = 0 ; i < o ; i++){
        for(int32_t j = i + 1 ; j < o ; j++){
            if (odd[i] > odd[j]){
                int32_t temp = odd[i];
                odd[i] = odd[j];
                odd[j] = temp;
            }
        }    
    }
    int32_t count = 0;
    for(int32_t i = e ; i < size ; i++){
        array[i] = odd[count];
        count++;
    }

}

void myprint( int32_t array[], int32_t size ){

    for(int32_t i = 0 ; i < size ; i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}