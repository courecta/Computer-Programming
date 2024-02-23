#include "myMultiplication.h"

int main(){

  // a represents the first number while b represents the second number
  uint32_t a = 0, b = 0;


  printf("Please enter the first number");
  scanf("%d", &a);
  printf("Please enter the second number");
  scanf("%d", &b);

  multiply(a, b);

    return 0;
}
