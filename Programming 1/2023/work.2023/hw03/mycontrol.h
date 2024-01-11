#pragma once

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void initialize( double x, double y, double a);
int32_t forward( double length );
int32_t clock_turn( double x );
int32_t counterclock_turn( double x );
int32_t print();