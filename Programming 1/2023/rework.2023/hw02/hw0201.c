#include "myIrrational.h"

int main(){

    uint16_t n = 0;
    int32_t pass = 0, twoVal = 0, gregoryLeibniz = 0, nilakantha = 0, eulerVal = 0;
    double *sqrt_two;
    double *euler;

    n = getNum();
    char operation;
    for(;;)
    {
    for( int32_t i = 0 ; i < 60 ; i++) printf("=");
    printf("\nWhich constant do you want to display?\n"
    "1. The Square root of 2 by continuous fraction\n"
    "2. The Gregory-Leibniz representation of pi\n"
    "3. The Nilakantha representation of pi\n"
    "4. The infinite series representation of euler's constant\n");
    scanf("%c", &operation);
    switch (operation)
        {
        case '1':
        twoVal = calculate_two(n, sqrt_two);
        if(twoVal == 1) error();
        break;

        case '2':
        int32_t gregoryLeibniz = calculate_gregoryLeibniz(n);
        if(gregoryLeibniz == 1) error();
        break;

        case '3':
        int32_t nilakantha = calculate_nilakantha(n);
        if(nilakantha == 1) error();
        break;

        case '4':
        int32_t eulerVal = calculate_euler(n, euler);
        if(eulerVal == 1) error();
        break;

        default:

        break;
        }
    }
    
    return 0;
}