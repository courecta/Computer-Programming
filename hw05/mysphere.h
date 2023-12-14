#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*
Return -1 if the inputs are invalid.
Return 0 if no such a plane.
Otherwise , Return 1.
*/

int32_t get_cap_area( double r, double a, double b, double c, double d, double *pArea );