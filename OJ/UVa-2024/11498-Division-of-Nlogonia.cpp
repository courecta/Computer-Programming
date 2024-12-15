#include <stdio.h>

int main()
{
    int cases = 0;
    int div_x = 0, div_y = 0, x = 0, y = 0, diff_x = 0, diff_y = 0;
    while(1)
    {
        scanf("%d", &cases);
        if(cases == 0)
            break;
        scanf("%d%d", &div_x, &div_y);
        while(cases--)
        {
            scanf("%d%d", &x, &y);
            diff_x = x - div_x;
            diff_y = y - div_y;
            if(diff_x > 0 && diff_y > 0)
                printf("NE\n");
            else if(diff_x < 0 && diff_y > 0)
                printf("NO\n");
            else if(diff_x < 0 && diff_y < 0)
                printf("SO\n");
            else if(diff_x > 0 && diff_y < 0)
                printf("SE\n");
            else
                printf("divisa\n");
        }
    }
}