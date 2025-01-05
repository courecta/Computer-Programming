#include "tlv.h"

uint64_t save = 0;
int8_t set_num = -1, set_save = -1;

TLVHandler tlv_handler[MAX_TLV_TYPES] = {
    skip, set, add, multiply, divide_two, divide_ten, prepend, append, initialize, print, cancel
};

int32_t run(uint8_t* pByteArray, int32_t size)
{
    if(pByteArray == NULL)
    {
        printf("pByteArray == NULL\n");
        return -1;
    }

    uint64_t number = 0;
    set_num = -1;
    set_save = -1;
    save = 0;

    int32_t index = 0;
    
    while(index < size)
    {
        TLV tlv;
        if(parse(pByteArray, size, &index, &tlv) == -1)
        {
            if(index == size) break;
            return -1;
        }
        if(dispatch(&tlv, &number) == -1) return -1;
        free(tlv.value);
    }
    return 0;
}

int32_t parse(uint8_t *pByteArray, int32_t size, int32_t* index, TLV* tlv)
{
    if(pByteArray[*index] == 0xFF)
    {
        *index = size;
        return -1;
    }

    tlv->type = pByteArray[*index];
    tlv->length = pByteArray[*index + 1] | (pByteArray[*index + 2] << 8);

    if(*index + 3 + tlv->length > size) return -1;

    tlv->value = malloc(tlv->length);
    if(!tlv->value) return -1;
    
    memcpy(tlv->value, &pByteArray[*index + 3], tlv->length);

    *index += 3 + tlv->length;

    return 0; 
}

int32_t dispatch(TLV* tlv, uint64_t* number)
{
    if(tlv->type >= MAX_TLV_TYPES) return skip(tlv, number);
    
    return tlv_handler[tlv->type](tlv, number);
}

int32_t skip(TLV* tlv, uint64_t* number)
{
    save = *number;
    return 0;
}

int32_t set(TLV* tlv, uint64_t* number)
{
    uint64_t value = 0;
    for(uint16_t i = 0; i < tlv->length; i++)
    {
        value = value * 10 + tlv->value[i];
    }
    save = *number;
    set_save = set_num;
    set_num = 1;
    *number = value;
    return 0;
}

int32_t add(TLV* tlv, uint64_t* number)
{
    if(set_num == -1) return -1;

    uint64_t value = 0;
    for(uint16_t i = 0; i < tlv->length; i++)
    {
        value = value * 10 + tlv->value[i];
    }
    save = *number;
    *number += value;
    return 0;
}

int32_t multiply(TLV* tlv, uint64_t* number)
{
    if(set_num == -1) return -1;

    uint64_t value = 0;
    for(uint16_t i = 0; i < tlv->length; i++)
    {
        value = value * 10 + tlv->value[i];
    }
    save = *number;
    *number *= value;
    return 0;
}

int32_t divide_two(TLV* tlv, uint64_t* number)
{
    if(set_num == -1 || tlv->length != 0) return -1;
    save = *number;
    *number /= 2;
    return 0;
}

int32_t divide_ten(TLV* tlv, uint64_t* number)
{
    if(set_num == -1 || tlv->length != 0) return -1;
    save = *number;
    *number /= 10;
    return 0;
}

int32_t prepend(TLV* tlv, uint64_t* number)
{
    if(set_num == -1) return -1;

    uint64_t value = 0;
    for(uint16_t i = 0; i < tlv->length; i++)
    {
        value = value * 10 + tlv->value[i];
    }
    save = *number;

    while(*number > 0)
    {
        value *= 10;
        *number /= 10;
    }
    *number = value + save;
    return 0;
}

int32_t append(TLV* tlv, uint64_t* number)
{
    if(set_num == -1) return -1;

    uint64_t value = 0;
    for(uint16_t i = 0; i < tlv->length; i++)
    {
        value = value * 10 + tlv->value[i];
    }
    save = *number;

    uint64_t temp = value;
    uint64_t multiplier = 1;
    while(temp > 0) {
        multiplier *= 10;
        temp /= 10;
    }

    *number = (*number * multiplier) + value;
    return 0;
}

int32_t initialize(TLV* tlv, uint64_t* number)
{
    if(tlv->length != 0) return -1;

    save = *number;
    set_num = 1;
    *number = 0;
    return 0;
}

int32_t print(TLV* tlv, uint64_t* number)
{
    if(set_num == -1 || tlv->length != 0) return -1;
    printf("%lu\n", *number);
    return 0;
}

int32_t cancel(TLV* tlv, uint64_t* number)
{
    uint64_t temp = *number;
    *number = save;
    save = temp;
    return 0;
}
