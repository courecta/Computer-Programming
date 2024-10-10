#include <stdio.h>

int main(){

    int cases(0), walls(0), current_wall(0), next_wall(0), high(0), low(0), num(1);

    scanf("%d", &cases);

    while(cases--)
    {
        walls = 0, current_wall = 0, next_wall = 0, high = 0, low = 0;
        scanf("%d", &walls);
        if(walls != 1)
        {
            scanf("%d", &current_wall);
            for(int i = 1 ; i < walls ; i++)
            {
                scanf("%d", &next_wall);
                if(next_wall > current_wall) high++;
                if(next_wall < current_wall) low++;
                current_wall = next_wall;
            }
        } else scanf("%d", &current_wall);

        printf("Case %d: %d %d\n", num, high, low);
        num++;
    }

}