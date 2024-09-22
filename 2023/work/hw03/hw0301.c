#include "mycircle.h"

int main(){
    set_radius(2);
    double a = get_inner_regular_polygon_area(5);
    double b = get_outer_regular_polygon_area(6);
    set_radius(-1);
    double c = get_inner_regular_polygon_area(5);
    double d = get_outer_regular_polygon_area(6);
    set_radius(3);
    double e = get_inner_regular_polygon_area(5);
    double f = get_outer_regular_polygon_area(6);
    printf("%lf %lf %lf %lf %lf %lf", a, b, c, d, e, f);
}