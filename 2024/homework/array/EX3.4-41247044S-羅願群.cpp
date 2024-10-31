#include <stdio.h>

int main(){

    int scores[10] = {0};
    int score = 0;
    printf("Please input a score (-1 to stop and display the histogram)...>");

    while(1)
    {
        scanf("%d", &score);
        if(score == -1) break;
        if(score % 10 == 0 && score % 100 != 0)
        {
            scores[(score /= 10) - 1] += 1;
        }else if(score == 100)
        {
            scores[9] += 1;
        }else scores[(score /= 10)] += 1;
    }

    printf("---------------------------------\n");

    int lower = 0, upper = 10, stars = 0;

    for(int i = 0 ; i < 10 ; i++)
    {
        printf("%2d~%3d ", lower, upper);
        lower += 11;
        if(i != 0) lower--;
        upper += 10;
        stars = scores[i];
        while(stars--) printf("*");
        printf("\n");
    }
    return 0;
}