#include <stdio.h>

int main(){
    int cases = 0, sum = 0, n = 0;
    scanf("%d", &cases);
    while(cases--)
    {
        scanf("%d", &n);
        sum = 0;
        for(int i = 1 ; i < n ; i++) if(n % i == 0) sum += i;
        if(sum > n) printf("abundant\n");
        else if(sum == n) printf("perfect\n");
        else printf("deficient\n");
    }
}