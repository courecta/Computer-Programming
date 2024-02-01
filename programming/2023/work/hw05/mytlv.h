#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

// Return -1 if the inputs are invalid. Otherwise , Return 0.
int32_t run( uint8_t *pByteArray , int32_t size );

int32_t action(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t assign(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t math(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t concat(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t zeroOut(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t print(uint8_t *pByteArray , int8_t current, int32_t size);
int32_t cancel(uint8_t *pByteArray , int8_t current, int32_t size);