#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

void minMax(int32_t array[], int32_t size, int32_t *min, int32_t *max);
int32_t *findMid(int32_t array[], int32_t size);
int32_t sumArray(int32_t array[], int32_t size);

int main(void){
    srand(time(NULL));

    const int32_t size = (rand() % 10) + 1; // instead of [0, N - 1], here it is [1, N]

    int32_t *arr = (int*)calloc(size, sizeof(int));

    for(int32_t i = 0; i < size; i++)
        arr[i] = rand() % 1000;

    printf("Generated array: ");
    for(int32_t i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");

    int32_t min, max;

    minMax(arr, size, &min, &max);

    printf("Minimum value of array is %d\nMaximum value of array is %d\n", min, max);

    int32_t *mid = findMid(arr, size);

    printf("Value of the middle of the array: %d\n", *mid);

    int32_t sum = sumArray(arr, size);

    printf("Sum of elements in array: %d\n", sum);

    int *ptr;
    printf("Reversed array: ");
    for(ptr = arr + size - 1; ptr >= arr; --ptr)
        printf("%d ", *ptr);
    printf("\n");

    free(arr);
    return 0;
}

void minMax(int32_t array[], int32_t size, int32_t *min, int32_t *max)
{
    *min = *max = array[0];
    int32_t i;
    for(int32_t i = 1; i < size; ++i)
    {
        if(array[i] > *max)
            *max = array[i];
        if(array[i] < *min)
            *min = array[i];
    }
}

int32_t *findMid(int32_t array[], int32_t size)
{
    return &(array[size / 2]);
}

int32_t sumArray(int32_t array[], int32_t size)
{
    int sum = 0, *p;
    for(p = array; p <= array + size - 1; ++p)
        sum += *p;
    return sum;
}