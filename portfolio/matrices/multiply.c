#include "matrix.h"

void multiplication(int m, int n, int p, int a[m][n], int b[m][n])
{
    int result;
    for(int i = 0 ; i < m ; i++)
    {
        for(int j = 0 ; j < p ; j++)
        {
            result = 0;
            for(int k = 0 ; k < n ; k++)
            {
                result += a[i][k] * b[k][j];
            }
            printf("%d ", result);
        }
        printf("\n");
    }
}