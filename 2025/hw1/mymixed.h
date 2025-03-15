#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sMixedNumber {
    int32_t integer;      // Integer part of the mixed fraction
    int32_t numerator;    // Numerator part of the fraction
    int32_t denominator;  // Denominator part of the fraction (cannot be 0)
} sMixedNumber;

// Print the number in the latex style.
// The fraction part must be in the reduced form.
// Ex: 1\frac{2}{3}
// -1 if NULL input; otherwise, 0.
int32_t mixed_print(sMixedNumber *pNumber);

// Input the number.
// Ex: 1\frac{2}{3}
// -1 if NULL input or other invalid inputs; otherwise, 0.
int32_t mixed_input(sMixedNumber *pNumber , const char *str);

// -1 if NULL input; otherwise, 0.
// pNumber = r1 + r2
int32_t mixed_add(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 - r2
int32_t mixed_sub(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 * r2
int32_t mixed_mul(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 / r2
int32_t mixed_div(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// Compare two mixed numbers.
// If r1 > r2, return 1. If r1 = r2, return 0. If r1 < r2, return -1. For any errors, return -2.
int32_t mixed_compare(const sMixedNumber r1, const sMixedNumber r2);