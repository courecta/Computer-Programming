#include <stdio.h>

int main(void){
    int height = 0, row = 0, spaces = 0, blocks = 0;

    while(1)
    {
        printf("Height: ");
        scanf("%d", &height);
        if(height > 0 && height < 9) break;
    }
    row = (2 * height) + 1;
    for(int i = 0; i < height; i++)
    {
        spaces = height - (i + 1);
        while(spaces--) printf(" ");
        blocks = i + 1;
        for(int j = 0; j < blocks; j++) printf("#");
        printf(" ");
        for(int j = 0; j < blocks; j++) printf("#");
        printf("\n");
    }

    return 0;
}