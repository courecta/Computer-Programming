#include <stdio.h>

bool won(int bingo[5][5]);
bool find(int target, int bingo[5][5]);

int main(){

    int cases = 0, current, win = 0, first_hit = 0;
    int bingo[5][5] = {0};
    scanf("%d", &cases);
    
    while(cases--)
    {
        bingo[2][2] = -1;
        for(int i = 0 ; i < 5 ; i++)
        {
            for(int j = 0 ; j < 5 ; j++)
            {
                if(i == 2 && j == 2) continue;
                scanf("%d", &(bingo[i][j]));
            }
        }

        int k = 1;
        first_hit = 0;
        win = -1;
        for( ; first_hit < 3 ; ++k)
        {
            scanf("%d", &current);
            if(find(current, bingo)) ++first_hit;
        }

        for( ; k <= 75 ; ++k)
        {
            scanf("%d", &current);
            if(win != -1) continue;
            if(win == -1 && find(current, bingo) && won(bingo)) win = k;
        }
        printf("BINGO after %d numbers announced\n", win);
    }

    return 0;
}

bool won(int bingo[5][5])
{
    // diagonal bingo hit
    if(bingo[0][0] == bingo[1][1] && bingo[3][3] == bingo[4][4]) return true;
    if(bingo[4][0] == bingo[3][1] && bingo[0][4] == bingo[1][3]) return true;
    
    // horizontal and vertical bingo hits
    for (int i = 0; i < 5; ++i)
    {
        if(bingo[i][0] == bingo[i][1] && bingo[i][2] == bingo[i][3] && bingo[i][4] == -1) return true;
        if(bingo[0][i] == bingo[1][i] && bingo[2][i] == bingo[3][i] && bingo[4][i] == -1) return true;
    }
    
    return false;
}

bool find(int target, int bingo[5][5])
{
    for(int i = 0 ; i < 5 ; i++)
    {
        for(int j = 0 ; j < 5 ; j++)
        {
            if(bingo[i][j] == target)
            {
                bingo[i][j] = -1;
                return true;
            }
        }
    }
    return false;
}