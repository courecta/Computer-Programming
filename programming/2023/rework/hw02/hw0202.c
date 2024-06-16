#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

int main()
{
  int64_t firstNum = 0, secondNum = 0;
  // handling user input error
  printf("Please enter the first number: ");
  scanf("%ld", &firstNum);
  if(firstNum < 0 || firstNum > INT_MAX)
  {
    printf("Error, wrong input (0 ~ 2^31-1)!\n");
    return 0;
  }

  printf("Please enter the second number: ");
  scanf("%ld", &secondNum);
  if(secondNum < 0 || secondNum > INT_MAX)
  {
    printf("Error, wrong input (0 ~ 2^31-1)!\n");
    return 0;
  }

  uint64_t product = (uint64_t)firstNum * (uint64_t)secondNum;

  // Finding max length for spaces and lines
  int64_t maxLength = 1;
  uint64_t temp = 0;
  if(product < (uint64_t)firstNum || product < (uint64_t)secondNum || product == 0)
  {
    if(firstNum >= secondNum)
    {
      temp = firstNum;
    } else temp = secondNum;
  } else temp = product;
  
  while(temp >= 10)
  {
    temp /= 10;
    maxLength++;
  }

  maxLength = (2 * maxLength) + 1;


  // handling first number spacing
  uint64_t first_digit = 1;
  uint64_t temp_first = firstNum;
  if(temp_first > 0)
  {
    while(temp_first >= 10)
    {
      temp_first /= 10;
      first_digit++;
    }
    first_digit = (2 * first_digit) - 1;
  }

  // handle second number spacing
  uint64_t second_digit = 1;
  uint64_t temp_second = secondNum;
  if(temp_second > 0)
  {
    while(temp_second >= 10)
    {
      temp_second /= 10;
      second_digit++;
    }
    second_digit = (2 * second_digit) - 1;
  }
  //uint64_t b_digit = second_digit;
  // handle product number spacing
  uint64_t product_digit = 1;
  uint64_t temp_c = product;
  if(temp_c > 0)
  {
    while(temp_c >= 10)
    {
      temp_c /= 10;
      product_digit++;
    }
    product_digit = (2 * product_digit) - 1;
  }

  // Printing first number
  uint32_t first_line = maxLength - first_digit;
  while(first_line--) printf(" ");
  
  if(firstNum != 0){
  uint64_t temp_a = firstNum;
  uint64_t divisor_a = 1;

  while(temp_a >= 10)
  {
    divisor_a *= 10;
    temp_a /= 10;
  }

  temp_a = firstNum;

  while(divisor_a > 0)
  {
    printf("%ld ", temp_a / divisor_a);
    temp_a %= divisor_a;
    divisor_a /= 10;
  }
  } else printf("0");
  printf("\n");

  // Printing second number
  uint32_t second_line = (maxLength - second_digit) - 2;
  printf("*)");
  
  while(second_line--) printf(" ");

  if(secondNum != 0)
  {
  uint64_t temp_b = secondNum;
  uint64_t divisor_b = 1;

  while(temp_b >= 10)
  {
  divisor_b *= 10;
  temp_b /= 10;
  }

  temp_b = secondNum;
  
  while(divisor_b > 0)
  {
    printf("%ld ", temp_b / divisor_b);
    temp_b %= divisor_b;
    divisor_b /= 10;
  }
  } else printf("0");
  printf("\n");

  for(int64_t i = 0 ; i < maxLength ; ++i) printf("-");
  printf("\n");

  // Printing middle process
  /*if(secondNum > 9 && product != 0)
  {
    uint64_t current_b_digit = 0;
    uint64_t current_product = 0;

    for(uint64_t i = 0 ; i < b_digit ; ++i)
    {
      current_b_digit = 
    }

    for(int64_t i = 0 ; i < maxLength ; ++i) printf("-");
    printf("\n");
  }*/

  // Printing product
  for(uint64_t i = 0 ; i < maxLength - product_digit ; ++i) printf(" ");
  if(product != 0)
  {
    uint64_t temp_product = product;
    uint64_t divisor_product = 1;

    while(temp_product >= 10)
    {
    divisor_product *= 10;
    temp_product /= 10;
    }

    while(divisor_product > 0)
    {
      printf("%ld ", product / divisor_product);
      product %= divisor_product;
      divisor_product /= 10;
    }
  } else printf("0");
  printf("\n");
  return 0;
}
