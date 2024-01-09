#include "myIrrational.h"

int main(){

    uint16_t n = 0;
    double *sqrt_two;
    double *pi_value;
    printf("Please enter n (16-bits unsigned): ");

    for(;;)
    {
        if(scanf("%u", n) == 1)
        {
            break;
        }
        else
        {
            printf("\nError, invalid input\n");
            printf("Please enter n (16-bits unsigned): ");
            continue;
        }
    }

    calculate_irr(n, sqrt_two, pi_value);

    return 0;
}