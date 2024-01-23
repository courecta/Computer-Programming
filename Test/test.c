#include "test.h"

int main(){
    
    uint16_t n = 0;
    //double sqrt2 = sqrt(2);
    printf("Please enter n (16-bit unsigned): ");
    scanf("%hu", &n);

    double val = 1.0;
    for(uint16_t i = 1 ; i <= n ; i++)
    {
        val = 1.0 + (1.0 / (1.0 + val));    
        printf("n = %hu: %.15lf (%.15lf)\n", i, val, val - sqrt(2));
    }

    return 0;
}