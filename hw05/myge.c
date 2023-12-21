#include "myge.h"

int32_t gaussian_elimination( int32_t n, int32_t *pA, int32_t *py, int32_t **px )
{

    if (n <= 0 || pA == NULL || py == NULL || px == NULL )
    {
        return -1;
    }

    int64_t matrix[n][n + 1];

    for (size_t i = 0 ; i < n ; ++i)
    {
        for (size_t j = 0 ; j < n + 1 ; ++j)
        {
            j == n ? matrix[i][j] = py[i] : pA[i * n + j];
        }
    }


    for (size_t j = 0 ; j <= n ; ++j)
    {
        for (size_t i = 1 ; i <= n + 1 ; ++i)
        {
            //matrix[i][j]
        }
        printf("\n");
    }

    *px = (int32_t *)calloc(n , sizeof(int32_t *));

    

}

int64_t lcm(int64_t a, int64_t b)
{
    return ((a / gcd(a , b)) * b);
}

int64_t gcd(int64_t a, int64_t b)
{
    return b == 0 ? a : gcd(b, a % b);
}