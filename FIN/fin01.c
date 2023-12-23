#include "frac.h"

int main()
{
    int32_t a = 2;
    int32_t b = 6;
    int32_t c = 5;
    int32_t d = 9;
    int32_t x;
    int32_t y;

    int32_t return_value = frac_add(&x, &y, a, b, c, d );
    printf("%d / %d code = %d\n", x, y, return_value);

    return_value = frac_del(&x, &y, a, b, c, d );
    printf("%d / %d code = %d\n", x, y, return_value);

    return_value = frac_mul(&x, &y, a, b, c, d );
    printf("%d / %d code = %d\n", x, y, return_value);

    return_value = frac_div(&x, &y, a, b, c, d );
    printf("%d / %d code = %d\n", x, y, return_value);

    return 0;
}