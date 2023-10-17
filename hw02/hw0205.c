#include <stdio.h>
#include <stdint.h>

int main () {

    int32_t length = 0, layers = 0;

    printf("Please input the length: ");
    scanf("%d", &length);
    if (length <= 3){
      printf("Error. Length too short!\n");
      return 1;
    }
    printf("Please input the number of layers: ");
    scanf("%d", &layers);
    if (layers <= 1){
      printf("Error. Length too short!\n");
      return 1;
    }

  return 0;
}