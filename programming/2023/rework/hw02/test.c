#include <stdio.h>
#include <math.h>

int main()
{
  int a = 409708;
  int b = 6;
  
  while(b--)
  {
    printf("%d ", a % 10);
    a = floor(a / 10);
  }

  printf("\n");

  return 0;
}
