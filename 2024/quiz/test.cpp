#include <stdio.h>

int main(){

    int cases(0), val(0), sum(0), total(0);
    scanf("%d", &cases);

    while(cases--)
    {
        sum = 0;
        for(int i = 0 ; i < 3 ; i++)
        {
            scanf("%d", &val);
            sum += val;
        }
        if(sum >= 2) total++;
    }

    printf("%d\n", total);

    return 0;
}