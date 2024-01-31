#include "myIrrational.h"

void error()
{
    printf("Please choose a valid option!");
    exit(EXIT_FAILURE);
}

uint16_t getNum()
{
    uint16_t num = 0;
    printf("Please enter n (16-bits unsigned): ");
    if(scanf("%u", &num) == 0)
    {
            printf("\nError, invalid input\n");
            getNum();
    } else return num;
}

int32_t calculate_two( const uint16_t n, double *ptrTwo )
{
    double val = 1.0;
    for(uint16_t i = 1 ; i <= n ; i++)
    {
        printf("n = %hu: %.15lf (%.15lf)\n", i, val, val - sqrt(2.0));
        val = 1.0 + (1.0 / (1.0 + val));
    }
    *ptrTwo = val;
    return EXIT_SUCCESS;
}

int32_t calculate_gregoryLeibniz( const uint16_t n )
{
    
}

int32_t calculate_nilakantha( const uint16_t n )
{
    
}

int32_t calculate_euler( const uint16_t n, double *euler )
{
    
}