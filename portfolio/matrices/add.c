#include "matrix.h"

void addition(int m, int n, int a[m][n], int b[m][n])
{
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++) printf("%d ", a[i][j] + b[i][j]);
        printf("\n");
    }
}