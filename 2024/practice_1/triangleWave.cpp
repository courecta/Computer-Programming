#include <stdio.h>

int main(){

    int cases(0), amplitude(0), frequency(0), j(0), k(0), l(2);
    scanf("%d\n\n", &cases);

    while(cases--)
    {
        scanf("%d\n%d", &amplitude, &frequency);
        while(frequency--)
        {
            l = 2;
            for(int i = 1 ; i <= ((amplitude * 2) - 1) ; i++)
            {
                if(i <= amplitude)
                {
                    j = i;
                    while(j--) printf("%d", i);
                } else if(i > amplitude)
                {
                    j = i - l;
                    k = j;
                    while(j--) printf("%d", k);
                    l += 2;
                }
                printf("\n");
            }
            if(frequency != 0)printf("\n");
        }
        if(cases != 0)printf("\n");
    }

}