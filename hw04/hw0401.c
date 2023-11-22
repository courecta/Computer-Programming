#include "mysort.h"

int main (){

    int32_t array[] = {946, 723, 192, 62, 389, 891, 108, 609, 562, 638};
    int32_t size = sizeof(array)/sizeof(array[0]);
    //even descending, odd ascending
    // output: 8, 6, 4, 2, 1, 3, 5, 7, 9
    mysort(array, size);
    myprint(array, size);

    return 0;
}