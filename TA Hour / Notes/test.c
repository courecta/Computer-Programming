#include "test.h"

int main()
{
    char array[UCHAR_MAX] = {0};
    char arrayRev[UCHAR_MAX] = {0};
    int size = sizeof(array)/sizeof(array[0]);
    fgets(array, UCHAR_MAX, stdin);

    int j = 0;
    for (int i = size ; i >= 0 ; --i)
    {
    if (array[i] == 0)
    {
        continue;
    }
    else
    {
        arrayRev[j] = array[i];
        ++j;
    }
    }

    printf("Reversed: ");
    puts(arrayRev);

}