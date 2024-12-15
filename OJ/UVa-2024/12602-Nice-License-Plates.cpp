#include <stdio.h>

int main()
{
    int n = 0, num = 0, sum = 0;
    scanf("%d", &n);
    while(n--)
    {
        char str[10];
        sum = num = 0;
        scanf("%s", str);
        for(int i = 0 ; i < 3 ; i++) sum = (sum * 26) + (str[i] - 'A');
        for(int i = 4 ; i < 8 ; i++) num = (num * 10) + (str[i] - '0');
        sum = (sum > num) ? sum - num : num - sum;
        if(sum <= 100)
            printf("nice\n");
        else
            printf("not nice\n");
    }
}