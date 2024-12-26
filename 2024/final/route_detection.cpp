#include <stdio.h>

char GetDirection(int x,int y, int nx, int ny);
char GetTurn(int odir, int ndir);

int main(){

    int n = 0;
    int x = 0, y = 0, nx = 0, ny = 0;
    int l = 0, r = 0;
    char odir = 'E';
    scanf("%d", &n);
    while(n--)
    {
        scanf("%d%d", &nx, &ny);

        char ndir = GetDirection(x, y, nx, ny);
        char turn = GetTurn(odir, ndir);

        if(turn == 'L')
        {
            l++;
        }
        else if(turn == 'R')
        {
            r++;
        }
        
        x = nx;
        y = ny;
        odir = ndir;
    }
    printf("%d %d\n", l, r);
}

char GetDirection(int x, int y, int nx, int ny)
{
    if(ny > y)
    {
        return 'N';
    } else if(ny < y)
    {
        return 'S';
    } else if(nx > x)
    {
        return 'E';
    } else if(nx < x)
    {
        return 'W';
    }
}

char GetTurn(int odir, int ndir)
{
    char dirs[4] = {'N', 'W', 'S', 'E'};
    for(int i = 0; i < 4; i++)
    {
        if(dirs[i] == odir)
        {
            if(dirs[(i + 1) % 4] == ndir)
            {
                return 'L';
            } else if(dirs[(i - 1 + 4) % 4] == ndir)
            {
                return 'R';
            }
        }
    }
    return '-';
}