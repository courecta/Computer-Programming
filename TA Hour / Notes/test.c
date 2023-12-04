#include "test.h"

int main () {
   double val = (double) 161567 * 88 / 100;

   int32_t value = ceil(val);

   printf("value = %d\n", value);

   return(0);
}
