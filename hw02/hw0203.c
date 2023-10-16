#include <stdio.h>
#include <stdint.h>

  int32_t number;

int main () {

    for (;;){
      if ( number != 0){
        printf("Please enter an Integer: ");
        scanf("%d", &number);
      }
    }

  return 0;
}