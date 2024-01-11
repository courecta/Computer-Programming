#include <stdio.h>
#include <stdint.h>

int main () {

  int32_t number;
  printf("Please enter n (16-bits unsigned): ");
  scanf("%d", &number);
  for (int32_t i = 1.0 ; i <= number ; i++){
    double result = 1.0/(2.0 + result));
    double errorRate = result - 1.41421356237309504880;
    printf("n = %d: %.15lf (%.15lf)\n", i, result, errorRate);
  }
  return 0;
}
