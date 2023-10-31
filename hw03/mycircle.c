#include "mycircle.h"

int32_t r;

/*

int32_t set_radius( double r);
double get_circle_circumference();
double get_tangent_area( double x );
double get_inner_regular_polygon_area( int32_t n );
double get_outer_regular_polygon_area( int32_t n );

*/

double set_radius(double r){
    printf("Please enter a radius: ");
    scanf("%d", &r);
    
    if (r <= 0){
        return -1;
    }
    else {
        printf("Your radius is %d\n", r);
        get_circle_circumference();
        return 0;
    }
}

double get_circle_circumference(){
    if (set_radius() = -1){
        return -2;
    }
    else {
        double c = (2 * r) * M_PI;
        printf("Your circumference is %d", c);
    }
}

double get_circle_area(){
    if (set_radius() = -1){
        return -3;
    }
    else {
        double a = (r * r) * M_PI;
        printf("Your area is %d", a);
    }
}

double get_tangent_area(double x){
    if (set_radius() = -1){
        return -4;
    }
    else {

        int32_t x = 0;
        printf("Please enter a point, x, for the tangent line of the circle: ");
        scanf("%d", &x);

        if (x > r || x < (r * -1)){
            return -4;
        }

        double y = sqrt((r * r) - (x * x));
        



    }