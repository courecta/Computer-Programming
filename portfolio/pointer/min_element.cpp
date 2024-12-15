#include <stdio.h>

const int* min_element(const int arr[], int size);
void min_max_element(int** min, int** max, int* arr, int size);

int main()
{
    constexpr int DataSize = 5;
    int arr[DataSize] = {0};
 
    for (int i = 0; i < DataSize; i++)
        scanf("%d", &arr[i]);

    printf("min: %d\n", *min_element(arr, DataSize));
 
    int *min = nullptr, *max = nullptr;
    min_max_element(&min, &max, arr, DataSize);
    printf("min: %d, max: %d\n", *min, *max);
 
    *min = 0;
    *max = 100;
 
    for (int i = 0; i < DataSize; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

const int* min_element(const int arr[], int size)
{
    const int* min = arr;
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < *min) min = &arr[i];
    }
    return min;
}

void min_max_element(int** min, int** max, int* arr, int size)
{
    *min = *max = arr; // can be initialized seperately for clarity
    for (int i = 1; i < size; i++)
    {
        if (arr[i] < **min) *min = &arr[i]; // can also be (arr + i)
        if (arr[i] > **max) *max = &arr[i];
    }
}