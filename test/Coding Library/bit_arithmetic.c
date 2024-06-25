#include <stdio.h>
#include <stdint.h>

void bit_arithmetics();

int main(){

  uint8_t action;

  do
  {
    printf("Bit Arithmetics & Operations\nList of actions:\n");
    scanf("&u", &action);
    
    printf("1. Basic Bit Arithmetics\
          \n2. Bit Operations\
          \n3. ");

    switch(action)
    {
      case 1:
      bit_arithmetics();
      break;

      case 2:
      break;

      default:
      return 0;

    }
  } while(action != 0);

  return 0;
}

void bit_arithmetics(){

  
  
  return;
}