#include "mymath.h"
#include "mygraph.h"

int main(){

    srand(time(NULL));
    int v = UniformRand(0, 100);

    if(IsOdd(v)) printf("%d is odd.\n", v);

    if(IsPrime(v)) printf("%d is prime.\n", v);

    printf("The Euclidean distance between (0, 0) and (1, 2) is %f\n", EuclideanDistance(0, 0, 1, 2));

    DrawLine(UniformRand(0, 20));
    printf("\n");

    DrawRectangle(UniformRand(0, 10), UniformRand(0, 10));
    printf("\n");

    DrawSquare(UniformRand(0, 10));
    printf("\n");

    return 0;
}