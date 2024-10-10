#include <stdio.h>

int main(){

  int first_num = 0, operate = 0, second_num = 0, end_flag = 1;
  printf("Practical Calculator\n");
  printf("Input example: 100 1 30 means 100 + 30.\nFor the second integer, 1, 2, 3, 4, and 5 refer to +, -, *, /, and %, respectively.\nExit by inputting 0 0 0.\n");
  
  do
  {
    printf("\nYour input...>");
    scanf("%d%d%d", &first_num, &operate, &second_num);

    switch(operate)
    {
      case 0:
        if(first_num == 0 && second_num == 0)
        {
          end_flag = 0;
        } else
        {
          printf("\nInput 0 0 0 to end the program.\n");
        }
      break;

      case 1 ... 5:
        if((operate == 4 || operate == 5) && second_num == 0)
        {
          while(second_num == 0)
          {
            printf("\nThe divisor cannot be zero.\nPlease input the second operand again...>");
            scanf("%d", &second_num);
          }
        }
        switch(operate)
        {
          case 1:
            printf("\n%d + %d = %d\n", first_num, second_num, (first_num + second_num));
          break;

          case 2:
            printf("\n%d - %d = %d\n", first_num, second_num, (first_num - second_num));
          break;

          case 3:
            printf("\n%d * %d = %d\n", first_num, second_num, (first_num * second_num));
          break;

          case 4:
            printf("\n%d / %d = %d\n", first_num, second_num, (first_num / second_num));
          break;

          case 5:
            printf("\n%d %% %d = %d\n", first_num, second_num, (first_num % second_num));
          break;
        }
      break;

      default:
        printf("\nInvalid operator! The second operator should be in {1, 2, 3, 4, 5}.\n");
      // break;
    }
  } while(end_flag == 1);

  printf("\nThanks for using the calculator. Bye!\n");

  return 0;
}
