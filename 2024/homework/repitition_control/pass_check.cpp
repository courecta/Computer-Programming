#include <stdio.h>
#define CORRECT 117

int main(){

  int pass(0), attempts(4);

  printf("Please enter the password ...> ");
  
  do
  {
    scanf("%d", &pass);
    if(pass != CORRECT && attempts > 0) printf("Wrong passowrd. Input again...> ");
  } while(pass != CORRECT && attempts--);

  if(attempts > 0 || pass == CORRECT)
  {
    printf("Great! The program starts.\n");
  } else printf("Your account is locked. Please e-mail to the administrator.\n");

  return 0;
}
