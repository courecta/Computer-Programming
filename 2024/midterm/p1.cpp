#include <stdio.h>

int main(){
    // initialize variables on seperate lines for clarity
    int n = 0;
    int j = 0;
    int k = 0;
    int spaces = 0;
    int temp = 0;

    //take user input
    scanf("%d", &n);
    
    // fast edge case handling
    if(n != 1){
    // print initial 1
    temp = (n - 1) / 2; 
    while(temp--)
    {
        printf(" ");
    }
    printf("1\n");
    
    // print loop
    for(int i = 3 ; i <= n ; i += 2)
    {
        for(int j = 1 ; j <= i ; j++)
        {
            if(j % 2 != 0)
            {
                k = j;
                spaces = ((n - 1) / 2) - (k - 1) / 2;
                while(spaces--)
                {
                    printf(" ");
                }
                while(k--)
                {
                    printf("%d", j);
                }
                printf("\n");
            }
        }
    }
    } else
    {
        printf("1\n");
    }
    return 0;
}