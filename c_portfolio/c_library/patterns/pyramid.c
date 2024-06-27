#include<stdio.h>
#include<stdint.h>
#include <time.h>
#include <stdlib.h>

// Update this to use a typedef struct to construct a tuple that returns RGB and on a function call, returns new values for this RGB tuple

int main(){
    srand(time(NULL));
    while(1)
    {
        uint32_t h = (rand() % 20) + 1;
        int32_t R = rand() % 255;
        int32_t G = rand() % 255;
        int32_t B = rand() % 255;
        for(int32_t i = 0 ; i < h ; ++i)
        {
            for(int32_t j = 0 ; j < (2 * h) - 1 ; ++j)
            {
                if(j >= h - (i - 1) && j <= h + (i - 1))
                {
                    printf("\x1b[38;2;%d;%d;%dm*", R, G, B);
                } else printf(" ");
            }
            printf("\x1b[0m\n");
        }
        printf("\x1b[0m");
        system("clear");
    }
    return 0;
}