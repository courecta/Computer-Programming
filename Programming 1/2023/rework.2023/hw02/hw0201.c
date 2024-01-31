#include "myIrrational.h"

int main(){

    uint16_t n = 0;
    double *sqrt_two;
    double *euler;

    n = getNum();

    int32_t pass = calculate_irr(n, sqrt_two, pi_value, euler);

    if(pass == 1) return 1;

    return 0;
}