#include "myMultiplication.h"

bool check(const int64_t num)
{
  if(num < 0 || num > (pow(2,31) - 1)) return false;

  return true;
}

void multiply(const int64_t firstNum, const int64_t secondNum)
{



  return;
}

void print_num(const int64_t num, const int32_t length)
{
  int32_t n = num;
  static uint8_t callTime;
  callTime++;

  int32_t digits = count_digit(num);
  int32_t space = length - (digits * 2);
  if(callTime == 2)
  {
    space -= 2;
    printf("*)");
  }

  for( int32_t i = 0 ; i < space + 1 ; i++) printf(" ");

  int32_t temp = digits - 1;
  for( int32_t i = 0 ; i < digits ; i++)
  {
    if(num == 0)
    {
      printf("0");
      break;
    }

    n /= pow(10,temp);
    n %= 10;
    printf("%d ", n);
    temp -= 1;
    n = num;
  }
  printf("\n");
  return;
}

int32_t count_line(const int64_t num)
{
  return ((2 * num) + 1);
}

int32_t count_digit(const int64_t num)
{
  if(num == 0) return 1;
  return floor(log10(num) + 1);
}
