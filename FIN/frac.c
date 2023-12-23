#include "frac.h"

int32_t frac_add( int32_t *x, int32_t *y, int32_t a, int32_t b, int32_t c, int32_t d )
{
    if (b == 0 || d == 0 || x == NULL || y == NULL) return -1;
    
    int32_t denominator = 0;
    
    if (b != d)
    {
        denominator = b * d;
        a *= d;
        c *= b;
    }

    *x = a + c;

    if (*x == 0)
    {
        *x = 0;
        *y = 1;
        return 0;
    }

    if (denominator != 0)
    {
        *y = denominator;
    }
    else *y = b;

    int32_t factor = gcd(*x, *y);

    *x /= abs(factor);
    *y /= abs(factor);

    if (*x < 0 && *y < 0)
    {
        *x = abs(*x);
        *y = abs(*y);
    }
    else if (*x > 0 && *y < 0)
    {
        *y = abs(*y);
        *x = (-1) * *x;
    }

    return 0;

}

int32_t frac_del( int32_t *x, int32_t *y, int32_t a, int32_t b, int32_t c, int32_t d )
{
    if (b == 0 || d == 0 || x == NULL || y == NULL) return -1;

    if (b == 0 || d == 0) return -1;

    int32_t denominator = 0;

    if (b != d)
    {
        denominator = b * d;
        a *= d;
        c *= b;
    }

    *x = a - c;
    
    if (*x == 0)
    {
        *x = 0;
        *y = 1;
        return 0;
    } 

    if (denominator != 0)
    {
        *y = denominator;
    }
    else *y = b;

    int32_t factor = gcd(*x, *y);

    *x /= abs(factor);
    *y /= abs(factor);

    if (*x < 0 && *y < 0)
    {
        *x = abs(*x);
        *y = abs(*y);
    }
    else if (*x > 0 && *y < 0)
    {
        *y = abs(*y);
        *x = (-1) * *x;
    }

    return 0;

}

int32_t frac_mul( int32_t *x, int32_t *y, int32_t a, int32_t b, int32_t c, int32_t d )
{
    if (b == 0 || d == 0 || x == NULL || y == NULL) return -1;

    if (a == 0 || c == 0)
    {
        *x = 0;
        *y = 1;
        return 0;
    }

    *x = a * c;
    *y = b * d;

    int32_t factor = gcd(*x, *y);

    *x /= abs(factor);
    *y /= abs(factor);

    if (*x < 0 && *y < 0)
    {
        *x = abs(*x);
        *y = abs(*y);
    }
    else if (*x > 0 && *y < 0)
    {
        *y = abs(*y);
        *x = (-1) * *x;
    }

    return 0;
}

int32_t frac_div( int32_t *x, int32_t *y, int32_t a, int32_t b, int32_t c, int32_t d )
{
    if (b == 0 || c == 0 || x == NULL || y == NULL) return -1;

    if (a == 0 || d == 0)
    {
        *x = 0;
        *y = 1;
        return 0;
    }

    *x = a * d;
    *y = b * c;

    int32_t factor = gcd(*x, *y);

    *x /= abs(factor);
    *y /= abs(factor);

    if (*x < 0 && *y < 0)
    {
        *x = abs(*x);
        *y = abs(*y);
    }
    else if (*x > 0 && *y < 0)
    {
        *y = abs(*y);
        *x = (-1) * *x;
    }

    return 0;

}

int32_t gcd(int32_t a, int32_t b)
{
    a = ( a > 0) ? a : -a;
    b = ( b > 0) ? b : -b;

    while(a != b)
    {
        if(a > b) a -= b;
        else b -= a;
    }

    return a;

}