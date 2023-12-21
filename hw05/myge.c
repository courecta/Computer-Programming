#include "myge.h"

int32_t gaussian_elimination( int32_t n, int32_t *pA, int32_t *py, int32_t **px )
{

    if (n <= 0 || pA == NULL || py == NULL || px == NULL )
    {
        return -1;
    }

    *px = (int32_t *)calloc(n , sizeof(int32_t));

    int64_t matrix[n][n + 1];

    for (size_t i = 0 ; i < n ; ++i)
    {
        for (size_t j = 0 ; j < n + 1 ; ++j)
        {
            if (j == n)
            {
            matrix[i][j] = py[i];
            }
            else matrix[i][j] = pA[i * n + j];
    }


    for (size_t i = 0 ; i < n ; ++i)
    {
        if (matrix[i][i] == 0)
        {
            int32_t exchange = -2;

            for (size_t j = i + 1 ; j < n ; ++j)
            {
                if (matrix[i][j] != 0)
                {
                    exchange = j;
                    break;
                }
            }

            if (exchange == -2)
            {
            free(*px);
            return 0;
            }

                for (size_t j = 0; j <= n; ++j)
                {
                int32_t temp = matrix[i][j];
                matrix[i][j] = matrix[exchange][j];
                matrix[exchange][j] = temp;
                }
        }

        for (size_t j = i + 1; j < n; ++j)
        {
            int64_t factorPrime = lcm(matrix[i][i], matrix[j][i]) / matrix[i][i];
            int64_t factorSecond = lcm(matrix[i][i], matrix[j][i]) / matrix[j][i];

            for (size_t k = i; k <= n; ++k)
            {
                matrix[j][k] *= factorSecond;
                matrix[j][k] -= matrix[j][k] * factorPrime;
            }

            int64_t simplify = matrix[i][j];
            for (size_t m = i + 1; m <= n; ++m)
            {
                simplify = gcd(simplify, matrix[i][m]);
            }
            
            if (simplify != 0)
            {
                for (size_t a = i; a <= n; ++a)
                {
                    matrix[i][a] /= simplify;
                }
            }

        }
    }
}
    
    for (size_t i = n - 1; i >= 0; --i)
    {
        (*px)[i] = matrix[i][n];
        for (size_t j = i + 1; j < n; ++j)
        {
            (*px)[i] -= matrix[i][j] * (*px)[j];
        }
        if (matrix[i][i] == 0)
        {
            if ((*px)[i] == 0)
            {
                free(*px);
                return 2;
            } else {
                free(*px);
                return 0;
            }
        }
        (*px)[i] /= matrix[i][i];
    }
    
    return 1;

}

int64_t lcm(int64_t a, int64_t b)
{
    return (a / gcd(a , b) * b);
}

int64_t gcd(int64_t a, int64_t b)
{
    return b == 0 ? a : gcd(b, a % b);
}