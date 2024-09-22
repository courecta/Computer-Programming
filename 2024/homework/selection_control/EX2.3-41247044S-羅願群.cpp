#include <stdio.h>

int main(){

  int first = 0, second = 0, operate = 0, end = 0;

  printf("Advanced Calculator\nInput example: 100 1 30 means 100 + 30\nFor the second integer, 1, 2, 3, 4, and 5 refer to +, -, *, /, and % respectively.\nYour input...> ");
  scanf("%d%d%d", &first, &operate, &second);
 
  if(second == 0 && operate >= 4)
  {
    printf("The divisor/modulus cannot be zero.\nPlease input the second operand again...> ");
    scanf("%d", &second);
    if(second == 0)
    {
      // using some ANSI escape sequences! If they don't work then ggs
      printf("\e[1;31mI have no more patience.\n\e[0;37m");
      end++;
    }
  }
  if(end == 0)
  {
    switch(operate)
    {
      case 1:
      printf("%d + %d = %d\n", first, second, first + second);
      break;
    
      case 2:
      printf("%d - %d = %d\n", first, second, first - second);
      break; 
   
      case 3:
      printf("%d * %d = %d\n", first, second, first * second);
      break;
    
      case 4:
      printf("%d / %d = %d\n", first, second, first / second);
      break;
    
      case 5:
      printf("%d %% %d = %d\n", first, second, first % second);
      break;
    
      default:
      printf("Invalid operator! The second integer is supposed to be within 1 ~ 6 range.\n");
      break;

    }
  }
    return 0;
}
