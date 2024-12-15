#include <stdio.h>

int min(const int a, const int b);
int max(const int a, const int b);

int main(){
    int W = 0, H = 0, N = 0;
    int X1 = 0, Y1 = 0, X2 = 0, Y2 = 0;
    int free = 0;
    int min_x = 0, min_y = 0, max_x = 0, max_y = 0;
    
    while(1)
    {
        int array[500][500] = {0};
        scanf("%d%d%d", &W, &H, &N);
        if(W == 0 && H == 0 && N == 0) break;

        free = W * H;
        
        while(N--)
        {
            scanf("%d%d%d%d", &X1, &Y1, &X2, &Y2);
            min_x = min(X1, X2) - 1;
            min_y = min(Y1, Y2) - 1;
            max_x = max(X1, X2);
            max_y = max(Y1, Y2);

            for(int i = min_x ; i < max_x ; ++i)
            {
                for(int j = min_y ; j < max_y ; ++j)
                {
                    if(array[i][j] == 0)
                    {
                        free--;
                        array[i][j] = 1;
                    }
                }
            }
        }

        if(free > 1) printf("There are %d empty spots.\n", free);
        else if(free == 1) printf("There is one empty spot.\n");
        else printf("There is no empty spots.\n");
    }

    return 0;
}

int min(const int a, const int b)
{
    return (a < b) ? a : b;
}

int max(const int a, const int b)
{
    return (a > b) ? a : b;
}