#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#define MAX_TLV_TYPES (11)

typedef struct{
    uint8_t type;
    uint16_t length;
    uint8_t* value;
} TLV;

extern uint64_t save;
extern int8_t set_num, set_save;

// function pointer type for TLV handlers
typedef int (*TLVHandler)(TLV* tlv, uint64_t* number);

// Return -1 if the inputs are invalid. Otherwise , Return 0.
int32_t run(uint8_t* pByteArray, int32_t size);
size_t take(uint8_t *bytes);
int32_t parse(uint8_t *pByteArray, int32_t size, int32_t* index, TLV* tlv);
int32_t dispatch(TLV* tlv, uint64_t* number);

// operations
int32_t skip(TLV* tlv, uint64_t* number);
int32_t set(TLV* tlv, uint64_t* number);
int32_t add(TLV* tlv, uint64_t* number);
int32_t multiply(TLV* tlv, uint64_t* number);
int32_t divide_two(TLV* tlv, uint64_t* number);
int32_t divide_ten(TLV* tlv, uint64_t* number);
int32_t prepend(TLV* tlv, uint64_t* number);
int32_t append(TLV* tlv, uint64_t* number);
int32_t initialize(TLV* tlv, uint64_t* number);
int32_t print(TLV* tlv, uint64_t* number);
int32_t cancel(TLV* tlv, uint64_t* number);
extern TLVHandler tlv_handler[MAX_TLV_TYPES];