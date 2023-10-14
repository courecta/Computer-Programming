#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(){

  int64_t number1, number2, product, limit = pow(2, 31);
    printf("PLease enter the first number: ");
    scanf("%ld", &number1);
    printf("PLease enter the second number: ");
    scanf("%ld", &number2);

    if (0 > number1 || 0 > number2 || limit <= number1 || limit < number2){
      printf("ERROR - You have entered an invalid input!\n");
      return 1;
    }

    product = number1 * number2;
    int64_t divisor1 = 1000000000, divisor2 = 1000000000;
    while (divisor1 > 0){
      int32_t digitPlace1 = (number1 / divisor1) % 10;
      if (digitPlace1 == 0 && int32_t zero1 == 0){
        printf(" ");
      }
      else {
      printf("%d", digitPlace1);
      zero1 += 1;
      }
      if (divisor1 > 1){
        printf(" ");
      }
      divisor1 /= 10;
    }
    printf("\n");
    printf("*)");

    while (divisor2 > 0){
      int32_t digitPlace2 = (number2 / divisor2) % 10;
      if (digitPlace2 == 0 && int32_t zero2 == 0){
        printf(" ");
      }
      else {
      printf("%d ", digitPlace2);
      zero2 += 1;
      }

      if (divisor2 > 1){
        printf(" ");
      }
      divisor2 /= 10;
    }
    printf("\n");

    while ()
    
    for (int32_t i = 0 ; i >= (2 * product) + 1 ; i++){
      printf("-");
    }
    
    printf("\n");

    printf("  ");
    int32_t divisorProd = 1000000000;
    while (divisorProd > 0){
      int32_t digitPlaceProd = (product / divisorProd) % 10;
      if (digitPlaceProd == 0 && int32_t zeroProd == 0){
        printf(" ");
      }
      else {
      printf("%d ", digitPlaceProd);
      zeroProd += 1;
      }

      if (divisorProd > 1){
        printf(" ");
      }
      divisorProd /= 10;
    }
    printf("\n");

  return 0;
}