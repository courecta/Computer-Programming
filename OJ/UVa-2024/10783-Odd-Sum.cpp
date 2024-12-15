#include <stdio.h>

int main()
{
    int cases = 0, a = 0, b = 0, sum = 0, i = 1;
    scanf("%d", &cases);
    int answer[cases + 1];
    while(cases--)
    {
        scanf("%d%d", &a, &b);
        sum = 0;
        for(int i = a ; i <= b ; i++)
        {
            if(i % 2 != 0)
                sum += i;
        }
        answer[i++] = sum;
    }
    for(int j = 1 ; j < i ; j++) printf("Case %d: %d\n", j, answer[j]);
}