#include "mymath.h"

bool IsOdd(int v)
{
    return (v % 2 == 1);
}

bool IsPrime(int v)
{
    if(v <= 1) return false;
    if(v == 2) return true;
    if(v % 2 == 0) return false;
    int square = sqrt(v);
    for(int i = 3 ; i <= square ; i += 2)
    {
        if(v % i == 0) return false;
    }
    return true;
}

int Square(int v)
{
    return v * v;
}

int UniformRand(int lb, int ub) // [lb, ub]
{
    if(lb == ub) return lb;
    return lb + (rand() % (ub - lb + 1));
}

double EuclideanDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(Square(x2 - x1) + Square(y2 - y1));
}