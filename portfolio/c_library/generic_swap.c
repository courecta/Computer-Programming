#include <stdlib.h>
#include <string.h>

void swap(void *vp1, void *vp2, size_t size)
{
    void *tmp = malloc(size);
    if(tmp == NULL)
        return;

    memcpy(tmp, vp1, size); // tmp = a
    memcpy(vp1, vp2, size); // a = b
    memcpy(vp2, tmp, size); // b = tmp

    free(tmp);
}