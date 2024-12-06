#include <stdio.h>
#include <string.h>

int main(){

    int n = 0, r = 0, current = 0, r_new = 0;
    int array[10000] = {0};

    while(scanf("%d%d", &n, &r) != EOF)
    {
        r_new = r;
        while(r--)
        {
            scanf("%d", &current);
            array[current] = 1;
        }

        if(n == r_new) printf("*");
        else for(int i = 1 ; i <= n ; i++) if(array[i] == 0) printf("%d ", i);

        printf("\n");
        memset(array, 0, sizeof(array));
    }

    return 0;
}
