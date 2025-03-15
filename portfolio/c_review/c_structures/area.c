#include <stdio.h>

typedef struct {
    int x, y;
} point;

typedef struct {
    point upper_left;
    point lower_right;
} rectangle;

int abs(int x);
int area(rectangle shape);

int main(void){
    point p1, p2;

    printf("Enter X and Y of P1: ");
    scanf("%d%d", &p1.x, &p1.y);

    printf("Enter X and Y of P2: ");
    scanf("%d%d", &p2.x, &p2.y);

    rectangle r = {p1, p2};

    printf("Area of the rectangle is %d\n", area(r));

    return 0;
}

int area(rectangle shape)
{
    int length = abs(shape.lower_right.x - shape.upper_left.x);
    int width = abs(shape.lower_right.y - shape.upper_left.y);
    return length * width;
}

int abs(int x)
{
    return (x < 0) ? (-1 * x) : x;
}