#include "mycircle.h"

double r, x, n;
static int16_t set = 0;

double set_radius(double r){
    if (r <= 0){
        return -1;
    }
    set += 1;
    return 0;

}

double get_circle_circumference(){
    if (set != 1){
        return -2;
    }
    else {
        double c = (2 * r) * M_PI;
    }
}

double get_circle_area(){
    if (set != 1){
        return -3;
    }
    else {
        double a = (r * r) * M_PI;
    }
}

double get_tangent_area(double x){
    if (set != 1){
        return -4;
    }
    else {
        if (x > r || x < (r * -1)){
            return -4;
        }

        double y = sqrt((r * r) - (x * x));
        



    }

double get_inner_regular_polygon_area( int32_t n ){
     if (set != 1){
        return -5;
    } else {

    }

}
double get_outer_regular_polygon_area( int32_t n ){
    if (set != 1){
        return -6;
    } else {
        
    }
}