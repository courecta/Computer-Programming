#pragma once

// Libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

// function to get number of user
uint16_t getNum();

// functions to calculate the irrational constants
int32_t calculate_two( const uint16_t n, double *ptrTwo );
int32_t calculate_gregoryLeibniz( const uint16_t n );
int32_t calculate_nilakantha( const uint16_t n );
int32_t calculate_euler( const uint16_t n, double *euler );

