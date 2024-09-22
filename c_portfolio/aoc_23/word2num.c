#include "trebuchet.h"

int extract(char input_buffer[])
{
  int sum = 0, j = 0, num[UINT8_MAX] = {0};
  int size = strlen(input_buffer);

  for(int i = 0 ; i < size ;)
  {
    if(strncmp(&input_buffer[i], "one", 3) == 0)
    {
      num[j] = 1;
      j++;
      i += 3;
    }
    else if(strncmp(&input_buffer[i], "two", 3) == 0)
    {
      num[j] = 2;
      j++;
      i += 3;
    }
    else if(strncmp(&input_buffer[i], "three", 5) == 0)
    {
      num[j] = 3;
      j++;
      i += 5;
    }
    else if(strncmp(&input_buffer[i], "four", 4) == 0)
    {
      num[j] = 4;
      j++;
      i += 4;
    }
    else if(strncmp(&input_buffer[i], "five", 4) == 0)
    {
      num[j] = 5;
      j++;
      i += 4;
    }
    else if(strncmp(&input_buffer[i], "six", 3) == 0)
    {
      num[j] = 6;
      j++;
      i += 3;
    }
    else if(strncmp(&input_buffer[i], "seven", 5) == 0)
    {
      num[j] = 7;
      j++;
      i += 5;
    }
    else if(strncmp(&input_buffer[i], "eight", 5) == 0)
    {
      num[j] = 8;
      j++;
      i += 5;
    }
    else if(strncmp(&input_buffer[i], "nine", 4) == 0)
    {
      num[j] = 9;
      j++;
      i += 4;
    }
    else if(isdigit(input_buffer[i]))
    {
      num[j] = input_buffer[i] - '0';
      j++;
      i++;
    }
    else i++;
  }

  if(j > 1)
  {
    sum = num[j - 1];
    while(j--)
    {
      if(j != 0) continue;
      else sum += num[j] * 10;
    }
  } else sum = (num[0] * 10) + num[0];
  return sum;
}
