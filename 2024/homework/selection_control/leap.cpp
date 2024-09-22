#include <stdio.h>

int main(){

  int year(0);
  scanf("%d", &year);

  if(year % 400 == 0  || (year % 4 == 0 && year % 100 != 0))
  {
    printf("%d is a leap year\n", year);
  } else printf("%d is NOT a leap year\n", year);

  return 0;
}
