#include "mychar.h"

int islower2(int c)
{
    return (c >= 'a' && c <= 'z');
}

int isupper2(int c)
{
    return (c >= 'A' && c <= 'Z');
}

int ialpha2(int c)
{
    return (islower2(c) || isupper2(c));
}

int isdigit2(int c)
{
    return (c >= '0' && c <= '9');
}

int toupper2(int c)
{
    return (islower2(c) ? c - ('a' - 'A') : c);
}

int tolower2(int c)
{
    return (isupper2(c) ? c + ('a' - 'A') : c);
}