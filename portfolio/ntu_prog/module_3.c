#include <stdio.h>

void find(int target, int n, int m, int bingo[10][256][256]);
int won(int n, int m, int bingo[10][256][256], int winners[10]);

int main(void){
    int n = 0, m = 0, answer = 0, found_winner = 0;
    scanf("%d%d", &n, &m);
    int bingo[10][256][256] = {0};
    int winners[10] = {0};

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            for(int k = 0; k < m; k++)
            {
                scanf("%d", &bingo[i][j][k]);
            }
        }
    }

    for(int i = 0; i < m * m; i++)
    {
        scanf("%d", &answer);
        find(answer, n, m, bingo);
        for(int j = 0; j < n; j++)
        {
            if(won(j, m, bingo, winners))
            {
                found_winner = 1;
            }
        }
        if(found_winner) break;
    }
    printf("%d ", answer);
    for(int i = 0; i < n; i++)
    {
        if(winners[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");
    return 0;
}

void find(int target, int n, int m, int bingo[10][256][256])
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0 ; j < m ; j++)
        {
            for(int k = 0; k < m; k++)
            {
                if(bingo[i][j][k] == target)
                {
                    bingo[i][j][k] = -1;
                }
            }
        }
    }
}

int won(int n, int m, int bingo[10][256][256], int winners[10])
{
    int match = 0;
    // horizontal bingo search & tag
    for(int i = 0; i < m; i++)
    {
        if(bingo[n][i][0] != -1) continue;
        match = 1;
        for(int j = 1; j < m; j++)
        {
            if(!match) break;
            if(bingo[n][i][j] != -1) match = 0;
        }
        if(match)
        {
            winners[n] = 1;
            return 1;
        }
    }

    // vertical bingo search & tag
    for(int i = 0; i < m; i++)
    {
        if(bingo[n][0][i] != -1) continue;
        match = 1;
        for(int j = 1; j < m; j++)
        {
            if(!match) break;
            if(bingo[n][j][i] != -1) match = 0;
        }
        if(match)
        {
            winners[n] = 1;
            return 1;
        }
    }

    // diagonal bingo search & tag
    if(bingo[n][0][0] == -1)
    {
        match = 1;
        for(int i = 1; i < m; i++)
        {
            if(!match) break;
            if(bingo[n][i][i] != -1) match = 0;
        }
        if(match)
        {
            winners[n] = 1;
            return 1;
        }
    }
    if(bingo[n][0][m - 1] == -1)
    {
        match = 1;
        for(int i = 1; i < m; i++)
        {
            if(!match) break;
            if(bingo[n][i][(m - 1) - i] != -1) match = 0;
        }
        if(match)
        {
            winners[n] = 1;
            return 1;
        }
    }
    
    return 0;
}