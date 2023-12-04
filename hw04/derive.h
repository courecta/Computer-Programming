#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

void equation();
void displayEq(int32_t fCoefficients[], int32_t gCoefficients[]);
void displayf(int32_t fCoefficients[]);
void displayg(int32_t gCoefficients[]);
void product(int32_t fCoefficients[], int32_t gCoefficients[]);
void quotient(int32_t fCoefficients[], int32_t gCoefficients[]);
void error();
void derive(int32_t degree, int32_t coefficients[]);