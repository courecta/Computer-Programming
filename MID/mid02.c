#include <stdio.h>
#include <stdint.h>

int main(){

    int32_t n, l;

    printf("Please enter n: ");
    scanf("%d", n);
    printf("Please enter the edge length: ");
    scanf("%d", l);

    if (n <= 0 || l <= 0){
        printf("Error!\n");
        return 1;
    }

    


    return 0;
}