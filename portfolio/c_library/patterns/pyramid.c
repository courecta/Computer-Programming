#include<stdio.h>
#include<stdint.h>
#include <time.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

// Update this to use a typedef struct to construct a tuple that returns RGB and on a function call, returns new values for this RGB tuple

int main(){
    srand(getpid());
    // uint32_t h = (rand() % 20) + 1;
    while(1)
    {
        #ifndef h
        uint32_t h = 7;
        #endif
        int32_t R = rand() % 255;
        int32_t G = rand() % 255;
        int32_t B = rand() % 255;
        for(int32_t i = 0 ; i < h ; ++i)
        {
            for(int32_t j = 0 ; j < (2 * h) - 1 ; ++j)
            {
                if(j >= h - (i - 1) && j <= h + (i - 1))
                {
                    // sleep(1/10);
                    printf("\x1b[38;2;%d;%d;%dm*", R, G, B);
                } else printf(" ");
            }
            printf("\x1b[0m\n");
        }
        printf("\x1b[0m");

        #ifdef _WIN32
        Sleep(100);
        #else
        // sleep(0.90);  /* sleep for 100 milliSeconds */
        #endif
        system("clear");
    }
    return 0;
}