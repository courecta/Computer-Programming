#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main(){

  // Input checking
  int64_t number1 = 0, number2 = 0, product = 0, limit = pow(2, 31);
  int16_t zero1 = 0, zero2 = 0, zeroProd = 0;
    printf("PLease enter the first number: ");
    scanf("%ld", &number1);
    printf("PLease enter the second number: ");
    scanf("%ld", &number2);

    if (0 > number1 || 0 > number2 || limit <= number1 || limit < number2){
      printf("ERROR - You have entered an invalid input!\n");
      return 1;
    }

    // Space formatting
    product = number1 * number2;
    int64_t divisorProd = 1000000000;
    int32_t lineCount = 0, digitCount, zeroMet = 0;
    while (divisorProd > 0){
      digitCount = (product / divisorProd) % 10;
      if (digitCount == 0 && zeroMet == 0){
      }
      else if (digitCount != 0 && zeroMet == 0){
        zeroMet += 1;
      }
      if (zeroMet != 0){
        lineCount += 1;
      }
      divisorProd /= 10;
    }

    int32_t spaceDiv1 = 1000000000, zeroMet1 = 0, firstCount = 0, spaceCount1;
    while (spaceDiv1 > 0) {
      spaceCount1 = (number1 / spaceDiv1) % 10;
      if (spaceCount1 == 0 && zeroMet1 == 0){
      }
      else if (spaceCount1 != 0 && zeroMet1 == 0){
        zeroMet1 += 1;
      }
      if (zeroMet1 != 0){
        firstCount += 1;
      }
      spaceDiv1 /= 10;
    }

    int32_t spaceDiv2 = 1000000000, zeroMet2 = 0, secondCount = 0, spaceCount2;
    while (spaceDiv2 > 0) {
      spaceCount2 = (number2 / spaceDiv2) % 10;
      if (spaceCount2 == 0 && zeroMet2 == 0){
      }
      else if (spaceCount2 != 0 && zeroMet2 == 0){
        zeroMet2 += 1;
      }
      if (zeroMet2 != 0){
        secondCount += 1;
      }
      spaceDiv2 /= 10;
    }

    int16_t lines = (2 * lineCount) + 1, lines1 = (2 * firstCount) + 1, lines2 = (2 * secondCount) + 1;

    int16_t longestLine = 0;
    if (number1 >= number2 && number1 >= product){
      longestLine += lines1;
    }
    else if (number2 >= number1 && number2 >= product){
      longestLine += lines2;
    }
    else{
      longestLine += lines;
    }

    int16_t simple1 = 0, simple2 = 0;
    if (number1 == 0){
      simple1 += 1;
    }
    if (number2 == 0){
      simple2 += 1;
    }

    int16_t space1 = longestLine - (2 * firstCount);
    for (int16_t i = 0 ; i <= space1 ; i++){
      printf(" ");
    }

    int64_t divisor1 = 1000000000, divisor2 = 1000000000;
    if (simple1 == 1){
      printf("\b\b0");
    }
    else if (simple1 != 1){
    while (divisor1 > 0){
      int32_t digitPlace1 = (number1 / divisor1) % 10;
      if (digitPlace1 == 0 && zero1 == 0){
      }
      else {
      printf("%d ", digitPlace1);
      zero1 += 1;
      }
      divisor1 /= 10;
    }
    }
    printf("\n");
    printf("*)");

    int16_t space2 = longestLine - (2 * secondCount);
    for (int16_t i = 0 ; i <= space2 - 2 ; i++){
      printf(" ");
    }

    if (simple2 == 1){
    printf("\b\b0");
    }
    else if (simple2 != 1){
    while (divisor2 > 0){
      int32_t digitPlace2 = (number2 / divisor2) % 10;
      if (digitPlace2 == 0 && zero2 == 0){
      }
      else {
      printf("%d ", digitPlace2);
      zero2 += 1;
      }
      divisor2 /= 10;
    }
  }
    printf("\n");
    divisorProd = 1000000000;

    for (int32_t i = 0 ; i < longestLine ; i++){
      printf("-");
    }
    printf("\n");

    // Partial product calculations and formatting

    
    printf("\n");

    // Outputting and formatting the product
    if (product == 0 || number2 <= 9){
    for (int32_t i = 0 ; i < longestLine ; i++){
      printf("-");
    }
    printf("\n");
    }
    if (product == 0){
      for (int16_t i = 0 ; i < longestLine - 1 ; i++){
        printf(" ");
      }
      printf("0\n");
    }
    else {
    printf("  ");
    while (divisorProd > 0){
      int32_t digitPlaceProd = (product / divisorProd) % 10;
      if (digitPlaceProd == 0 && zeroProd == 0){
      }
      else {
      printf("%d ", digitPlaceProd);
      zeroProd += 1;
      }
      divisorProd /= 10;
    }
    printf("\n");
  }

  return 0;
}