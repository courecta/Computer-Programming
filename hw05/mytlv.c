#include "mytlv.h"

static uint8_t len1 = 0, len2 = 0;
static uint16_t bookMark = 0, lengthVal = 0;
static uint64_t number = 0, prev = 0;

int32_t run( uint8_t *pByteArray , int32_t size )
{
    uint8_t buffer = 0;
    for ( bookMark = 0 ; bookMark < size ; )
    {
        buffer = pByteArray[bookMark];
        //if (buffer > UCHAR_MAX) return -1;
        len1 = pByteArray[bookMark + 1];
        len2 = pByteArray[bookMark + 2];
        lengthVal = len1 | len2;
        bookMark += 3;
        if (action(pByteArray, buffer, size) == -1) return -1;
        prev = number;
        bookMark += lengthVal;
    }

}

int32_t action(uint8_t *pByteArray , int8_t current, int32_t size)
{
    switch (current)
    {
        case 1:
        assign(pByteArray, current, size);
        break;
        case 2 ... 5:
        if (math(pByteArray, current, size) == -1) return -1;
        break;
        case 6 ... 7:
        concat(pByteArray, current, size);
        break;
        case 8:
        if (zeroOut(pByteArray, current, size) == -1) return -1;
        break;
        case 9:
        if (print(pByteArray, current, size) == -1) return -1;
        break;
        case 10:
        if (cancel(pByteArray, current, size) == -1) return -1;
        break;
        default:
        break;
    }
    return 0;
}

int32_t assign(uint8_t *pByteArray , int8_t current, int32_t size)
{
    uint64_t num = 0;
    for (size_t i = bookMark ; i < lengthVal ; ++i)
    {
        num = (num << 8) | pByteArray[i];
    }
    number = num;
    return 0;
}

int32_t math(uint8_t *pByteArray , int8_t current, int32_t size)
{
    if (current == 4 || current == 5)
    {
        if (len1 != 0 || len2 != 0) return -1;
    }
    
    uint64_t num = 0;

    switch (current){
    
    case 2:
    
    for (size_t i = bookMark ; i < lengthVal ; ++i)
    {
        num = (num << 8) | pByteArray[i];
    }
    number += num;
    break;

    case 3:
    for (size_t i = bookMark ; i < lengthVal ; ++i)
    {
        num = (num << 8) | pByteArray[i];
    }
    number *= num;
    break;

    case 4:
    number /= 2;
    break;

    case 5:
    number /= 10;
    break;

    return 0;

    }
}

int32_t concat(uint8_t *pByteArray , int8_t current, int32_t size)
{


    return 0;
}

int32_t zeroOut(uint8_t *pByteArray , int8_t current, int32_t size)
{
    if (len1 != 0 || len2 != 0) return -1;

    number = 0;

    return 0;

}

int32_t print(uint8_t *pByteArray , int8_t current, int32_t size)
{
    if (len1 != 0 || len2 != 0) return -1;

    printf("%ld", number);

    return 0;

}

int32_t cancel(uint8_t *pByteArray , int8_t current, int32_t size)
{
    if (len1 != 0 || len2 != 0) return -1;
    
    int32_t cancelCounts = 0;
    for (size_t i = bookMark ; i < size ; i += 2)
    {
        lengthVal += 2;
        if (pByteArray[i] == 10)
        {
            cancelCounts++;
        }
    }
    if (cancelCounts % 2 != 0)
    {
        number = prev;
    }

    return 0;

}
