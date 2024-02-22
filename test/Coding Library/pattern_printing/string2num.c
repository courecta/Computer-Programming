#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int64_t StrToInt(char *pStr);

int64_t StrToInt(char *pStr)
{
    //return atoi(pStr);

    int num = 0;
    char digit;
    
    while ((digit = *(pStr++)) != '\0')
    {
    if (digit < '0' || digit > '9')
    {
      return 0;  /* No valid conversion possible */
    }
    num *= 10;
    num += digit - '0';
    }
  
    return num;

}

int main()
{

    int64_t num = StrToInt("a2131231");
    
    (num == 0) ? printf("Error!\n") : printf("%ld\n", num);

    return 0;
}