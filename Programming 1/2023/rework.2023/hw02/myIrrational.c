#include "myIrrational.h"

uint16_t getNum()
{
    uint16_t num;
    printf("Please enter n (16-bits unsigned): ");
    if(scanf("%u", &num) == 0)
    {
            printf("\nError, invalid input\n");
            getNum();
    } else return num;
}

int32_t calculate_irr( const uint16_t n, double *ptrTwo, double *euler )
{
    
}

void calculate_two( const uint16_t n, double *ptrTwo )
{

}

void calculate_gregoryLeibniz( const uint16_t n )
{
    
}

void calculate_nilakantha( const uint16_t n )
{
    
}

void calculate_euler( const uint16_t n, double *euler )
{
    
}