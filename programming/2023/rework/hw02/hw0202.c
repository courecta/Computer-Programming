#include "myMultiplication.h"

int main(){

  int64_t firstNum = 0, secondNum = 0, product = 0;

  do
  {
    printf("Please enter the first number: ");
    scanf("%d", &firstNum);
    printf("Please enter the second number: ");
    scanf("%d", &secondNum);
    
    product = firstNum * secondNum;

    bool firstCheck = check(firstNum);
    bool secondCheck = check(secondNum);
      if(product > INT64_MAX)
      { 
      bool productCheck = false;
      }
    if(firstCheck == 0 || secondCheck == 0 || productCheck == 0) printf("Error! Invalid integer detected, please try again.\n");

  } while(firstCheck == 0 || secondCheck == 0 || productCheck == 0);

  int32_t lineLen = count_digit(firstNum) > count_digit(secondNum) ? count_line(count_digit(firstNum)) : count_line(count_digit(secondNum));
  lineLen = lineLen > count_line(count_digit(product)) ? lineLen : count_line(count_digit(product));

  print_num(firstNum,lineLen);
  print_num(secondNum,lineLen);

  for(int32_t i = 0; i < lineLen ; i++)
  {
    printf("-");
  }
  printf("\n");

  multiply(firstNum,secondNum);



  print_num(product,lineLen);

  return 0;
}
