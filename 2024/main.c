#include <stdio.h>
#include <stdint.h>

int main(){

  int32_t choice = 0;
  int32_t hex = 0;
  int32_t bit1 = 0, bit2 = 0, bit3 = 0, bit4 = 0;
  printf("Please input a hex: ");
  scanf("%x", &hex);
  hex = hex % 16;
  printf("%d\n", hex);
  
  return 0;
}
