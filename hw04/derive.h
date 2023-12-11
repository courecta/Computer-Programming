#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// functions to handle displaying the initial equations
void equation();
void displayEq(int64_t fCoefficients[], int64_t gCoefficients[]);

// functions to handle displaying the product rules and quotient rules
void product(int64_t fCoefficients[], int64_t gCoefficients[], size_t quotientOn);
void quotient(int64_t fCoefficients[], int64_t gCoefficients[]);

// functions to handle errors and processes
void error(int16_t exitCode);
void derive(int32_t degree, int64_t coefficients[], int8_t mode);
void square(int64_t gCoefficients[]);

// function to print equations
void display(int32_t degree[], int64_t fCoefficients[]);

// function to free memory allocated
void freed();