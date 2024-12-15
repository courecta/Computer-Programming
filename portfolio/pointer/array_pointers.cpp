#include <stdio.h>

int main(){
    int num = 0, arr1[5] = {0}, arr2[3][4] = {0}, *ptr1 = nullptr, *aop[4] = {0};

    ptr1 = &num;
    aop[0] = arr1;
    *aop[0] = 60;

    aop[1] = &arr1[2];
    *aop[1] = 70;

    aop[2] = arr1[1];
    aop[2][1] = 80;

    aop[3] = arr2[1];
    aop[3][1] = 90;
}

/*
    int arr[5] = {1, 2, 3, 4, 5};
    
    int num = 66;
    int* array_of_pointers[3] = {&num, arr + 2};

    array_of_pointers[2] = &arr[4];

    for(int i = 0 ; i < 3 ; i++)
        printf("%d ", *array_of_pointers[i]);
    printf("\n");
*/