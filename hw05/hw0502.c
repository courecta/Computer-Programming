#include "myge.h"

int main() {
    int32_t n = 3;

    // Flatten the matrix into a single array
    int32_t A[] = {
        1, 1, 1,
        1, 2, 4,
        1, 3, 9
    };
    int32_t y[] = {6, 17, 34};
    int32_t *x = NULL;
    int32_t x_answer[] = {1, 2, 3};

    int32_t result = gaussian_elimination(n, A, y, &x);

    if (result == 1) {
        printf("The solution is:\n");
        for (int32_t i = 0; i < n; ++i) {
            printf("x[%d] = %d\n", i, x[i]);
        }
        printf("example solution:\n");
        for (int32_t i = 0; i < n; ++i) {
            printf("x[%d] = %d\n", i, x_answer[i]);
        }
    } else {
        printf("Wrong Answer. Result should be 1.\n");
    }

    if (x != NULL) free(x);

    return 0;
}