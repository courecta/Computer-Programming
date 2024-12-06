#include <stdio.h>
#include <string.h>

// Minesweeper

int main(){
    constexpr int MaxSize = 101;
    int n = 0, m = 0, t = 1;
    while (scanf("%d%d", &n, &m) == 2 && n && m)
    {
        char board[MaxSize][MaxSize] = {' '};
        for(int i = 0; i < n; i++)
        {
            scanf("%s", &board[i][0]);
        }
        if(t > 1) printf("\n");
        printf("Field #%d:\n", t++);
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                if(board[i][j] == '*') printf("*");
                else
                {
                    int count = 0;
                    for(int x = -1; x <= 1; x++)
                    {
                        for(int y = -1; y <= 1; y++)
                        {
                            if(i + x >= 0 && i + x < n && j + y >= 0 && j + y < m && board[i + x][j + y] == '*') count++;
                        }
                    }
                    printf("%d", count);
                }
            }
            printf("\n");
        }
    }
}