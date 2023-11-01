#pragma once

#include <math.h>
#include <stdio.h>
#include <stdint.h>

int32_t set_radius( double r);
double get_circle_circumference();
double get_circle_area();
double get_tangent_area( double x );
double get_inner_regular_polygon_area( int32_t n );
double get_outer_regular_polygon_area( int32_t n );