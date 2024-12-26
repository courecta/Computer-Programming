#include <stdio.h>

int isNum(char c);

int main(){

    int n = 0;
    scanf("%d", &n);        
    char map[11][11] = {'.'};
    for(int i = 0 ; i < n ; i++)
    {
        scanf("%s", map[i]);
    }

    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
            if(isNum(map[i][j]))
            {
                int t = map[i][j] - '0';
                int up_ok = 1, down_ok = 1, left_ok = 1, right_ok = 1;
                for(int k = 1 ; k <= t ; k++)
                {
                    if(j + k < n && map[i][j + k] != 'X' && right_ok) map[i][j + k] = 'O';
                    else if(j + k >= n || map[i][j + k] == 'X') right_ok = 0;
                    
                    if(j - k >= 0 && map[i][j - k] != 'X' && left_ok) map[i][j - k] = 'O';
                    else if(j - k < 0 || map[i][j - k] == 'X') left_ok = 0;
                    
                    if(j + k < n && map[i + k][j] != 'X' && down_ok) map[i + k][j] = 'O';
                    else if(j + k >= n || map[i + k][j] == 'X') down_ok = 0;
                    
                    if(j - k >= 0 && map[i - k][j] != 'X' && up_ok) map[i - k][j] = 'O';
                    else if(j - k < 0 || map[i - k][j] == 'X') up_ok = 0;
                }
            }
    }

    for(int i = 0 ; i < n ; i++)
    {
        for(int j = 0 ; j < n ; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
}

int isNum(char c)
{
     return (c >= '0' && c <= '9');
}