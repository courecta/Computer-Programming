#include "numeric.h"

#include <time.h>
#include <stdlib.h>

int main(){
    // Initializing array and variables
    constexpr int size = 10;
    srand(time(NULL));
    int array[size] = {0};

    // Populate array with random numbers and printing it out
    for(int i = 0; i < size; i++) array[i] = rand() % 100;
    printf("Current Aray: ");
    print(array, size);

    // testing out functions
    printf("Sum: %d\n", sum(array, size));
    printf("Average: %.0f\n", average(array, size));
    int max_val = max(array, size);
    int max_pos_val = max_pos(array, size);
    printf("Max: %d\n", max_val);
    printf("Max Position: %d\n", max_pos_val);

    // Cleaning the array except for the MAX value
    reset(array, size);
    printf("Reset Array except for MAX value: ");
    array[max_pos_val] = max_val;
    print(array, size);

    return 0;
}