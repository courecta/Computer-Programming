#include <stdio.h>

int main(){

    int x = 0, y = 0, dx = 0, dy = 0;
    scanf("%d %d", &dx, &dy);
    while(dx || dy)
    {
        int new_x = x + dx;
        int new_y = y + dy;
        int k = 1, matched = 0;
        while (k * k <= new_x || k * k <= new_y)
        {
            if(new_x == k * k)
            {
                matched++;
            }
            if(new_y == k * k)
            {
                matched++;
            }
            k++;
        }
        if(matched != 2)
        {
            x = new_x;
            y = new_y;
        }
        scanf("%d %d", &dx, &dy);
    }
    printf("%d %d\n", x, y);
}