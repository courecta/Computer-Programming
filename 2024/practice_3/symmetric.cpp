#include <stdio.h>

int main(){
    long long matrix[100][100] = {0};
    int cases, symmetric, n;
    char c;
    scanf("%d", &cases);
    for(int k = 1; k <= cases; k++)
    {
        symmetric = 1;
        scanf(" %c %c %d", &c, &c, &n);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                scanf("%lld", &matrix[i][j]);
                if(matrix[i][j] < 0) symmetric = 0;
            }
        }
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(matrix[i][j] != matrix[n - i - 1][n - j - 1]) symmetric = 0;
            }
        }
        if(symmetric) printf("Test #%d: Symmetric.\n", k);
        else printf("Test #%d: Non-symmetric.\n", k);
    }
}