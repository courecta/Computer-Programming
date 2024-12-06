#include <stdio.h>

int main(){

    int cases = 0, ordered = 0;
    int lumberjacks[10];
    scanf("%d", &cases);
    int array[cases];
    for(int k = 0 ; k < cases ; k++)
    {
        ordered = 1;
        for(int i = 0 ; i < 10 ; i++) scanf("%d", &lumberjacks[i]);
        
        if(lumberjacks[0] < lumberjacks[1])
        {
            for(int i = 1 ; i < 10 ; i++)
            {
                if(lumberjacks[i - 1] > lumberjacks[i]) ordered = 0;
            }
        }
        else if(lumberjacks[0] > lumberjacks[1])
        {
            for(int i = 1 ; i < 10 ; i++)
            {
                if(lumberjacks[i - 1] < lumberjacks[i]) ordered = 0;
            }
        }

        array[k] = (ordered) ? 1 : 0;
    }

    printf("Lumberjacks:\n");
    for(int i = 0 ; i < cases ; i++) (array[i]) ? printf("Ordered\n") : printf("Unordered\n");

    return 0;
}