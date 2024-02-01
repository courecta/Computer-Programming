#include "mycircle.h"

double r, x, n, radius, previousR;
static int16_t set = 0;

int32_t set_radius(double r){
    if (r <= 0){
    return -1;
    }
    if (set >= 1 && r<= 0){
    radius = previousR;
    return radius;
    }
    set += 1;
    radius = r;
    previousR = r;
    return radius;

}

double get_circle_circumference(){
    if (set < 1){
    return -2;
    }

    double c = (2 * radius) * M_PI;
    return c;
}

double get_circle_area(){
    if (set < 1){
    return -3;
    }
    
    double area_circle = (radius * radius) * M_PI;
    return area_circle;
}

double get_tangent_area(double x){
    if (set < 1){
    return -4;
    }
    if (x > r || x < (r * -1)){
    return -5;
    }

    double y = sqrt((radius * radius) - (x * x));

    if (y == 0){
    return -6;
    }

    double m = -1 * (x / y);

    if (m == 0){
    return -7;
    }
        
    double b = y - (m * x);
    double xint = b;
    double yint = -(b)/m;
    double area_tangent = (xint * yint)/2;
    return area_tangent;

}

double get_inner_regular_polygon_area( int32_t n ){
    if (set < 1){
    return -8;
    }
    if (n < 3){
    return -9;
    }

    double inner_area = (n / 2) * (radius * radius) * sin((2 * M_PI) / n);
    return inner_area;

}

double get_outer_regular_polygon_area( int32_t n ){
    if (set < 1){
    return -10;
    }
    if (n < 3){
    return -11;
    }
    
    double outer_area = (n / 2) * (2 * radius * sin(M_PI / n)) * (radius * cos(M_PI / n));
    return outer_area;
}