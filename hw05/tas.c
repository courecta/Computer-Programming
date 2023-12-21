#include "tas.h"

size_t init = 0;

void button_set_frame(uint8_t **src, size_t *size, const uint8_t button , const uint64_t start_frame , const uint64_t end_frame){

    if (size == NULL || end_frame < start_frame)
    {
    return;
    }

    if (init == 0)
    {
    *size = end_frame + 1;
    *src = calloc(*size, sizeof(uint8_t));
    init++;
    }

    if (end_frame > *size)
    {
    *size = end_frame + 1;
    *src = realloc(*src, *size * sizeof(uint8_t));
    }

    for ( size_t i = start_frame ; i <= end_frame ; ++i)
    {
    (*src)[i] |= button;
    }

    return;

}

void button_unset_frame(uint8_t *src, const size_t size, const uint8_t button , const uint64_t start_frame , const uint64_t end_frame){

    if (size == 0 || end_frame < start_frame)
    {
    return;
    }

    for (size_t i = start_frame ; i <= end_frame ; ++i)
    {
    if (i >= size) break;
    src[i] &= ~(button);
    }

    return;

}