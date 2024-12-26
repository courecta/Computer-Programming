#include <stdio.h>

int main(){

  int a(0), b(0);
  printf("Please input two integers, and I will show you the results of five arithmetic operation.\nYour input (seperated by a space)...> ");
  scanf("%d%d", &a, &b);
  if(b == 0)
  {
    printf("the second operand cannot be zero\nPlease input the second operand again...> ");
    scanf("%d", &b);
    if(b == 0)
    {
      printf("hey, do you understand what i said?\n");
      return 0;
    }
  }
  
  printf("%d + %d = %d\n", a, b, (a + b));
  printf("%d - %d = %d\n", a, b, (a - b));
  printf("%d * %d = %d\n", a, b, (a * b));
  printf("%d / %d = %.2f\n", a, b, ((float)a / (float)b));
  printf("%d %% %d = %d\n", a, b, (a % b)); 

  return 0;
}
