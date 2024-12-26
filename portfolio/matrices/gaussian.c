#include <stdio.h>
#include <stdint.h>

// I promise that A is an nxn matrix (2d array).
// However , you need to check if the inputs are valid.
// You should malloc for x.
// Return -1 if the inputs are invalid. *px should be NULL.
// Return 0 if there is no solution. *px should be NULL.
// Return 1 if there is only one solution. x should be the solution
// Return 2 if there are more than one solutions. x should be one possible solution.

int32_t gaussian_elimination(int32_t n, int32_t *pA, int32_t *py, int32_t **px);

int main(){

    int32_t n = 3, result = 0;
    int32_t A[3][3] = {{1, 1, 1}, {1, 2, 4}, {1, 3, 9}};
    int32_t y[3] = {6, 17, 34};
    int32_t *x = NULL;
    result = gaussian_elimination(n, (int32_t *)A, y, &x);
    return 0;
}