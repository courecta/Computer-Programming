#include "numeric.h"

int sum(const int array[], const int size)
{
    if(size == 1) return array[0];
    int sum = 0;
    for(int i = 0; i < size; i++) sum += array[i];
    return sum;
}

double average(const int array[], const int size)
{
    return sum(array, size) / size;
}

void print(const int array[], const int size)
{
    for(int i = 0; i < size; i++) printf("%d ", array[i]);
    printf("\n");
}

void reset(int array[], const int size)
{
    for(int i = 0; i < size; i++) array[i] = 0;
}

int max(const int array[], const int size)
{
    int max = array[0];
    for(int i = 1; i < size; i++) if(array[i] > max) max = array[i];
    return max;
}

int max_pos(const int array[], const int size)
{
    int max_val = max(array, size);
    for(int i = 0; i < size; i++) if(array[i] == max_val) return i;
    return -1;
}