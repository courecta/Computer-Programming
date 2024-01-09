#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/*
I promise that A is an nxn matrix (2d array).
However , you need to check if the inputs are valid.
You should malloc for x.
Return -1 if the inputs are invalid.
Return 0 if there is no solution.
Return 1 if there is only one solution.
Return 2 if there are more than one solutions.
*/

int32_t gaussian_elimination( int32_t n, int32_t *pA, int32_t *py, int32_t **px );

int64_t gcd(int64_t a, int64_t b);

int64_t lcm(int64_t a, int64_t b);