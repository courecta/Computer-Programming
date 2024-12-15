#include <stdio.h>

int main()
{
    int n = 0, num = 0, sum = 0;
    while(scanf("%d", &n) != 0 && n != 0)
    {
        while(n >= 10)
        {
            sum = 0;
            while(n > 0)
            {
                sum += n % 10;
                n /= 10;
            }
            n = sum;
        }
        printf("%d\n", n);
    }
}