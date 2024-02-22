#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t getDigits();
void generator(int32_t* array, size_t start, size_t end);

int main(){
  
  srand((unsigned)time(NULL));

  int32_t* randomDigits = NULL;

  int32_t counter = getDigits();

  randomDigits = (int32_t*)calloc(counter, sizeof(int32_t));

  generator(randomDigits, 0, counter);
  printf("Your code is ");
  for (size_t i = 0 ; i < counter ; ++i){
  printf("%d", *randomDigits);
  randomDigits++;
  }
  
  printf("\n");
  return 0;
}

int32_t getDigits(){

  int32_t num = 0;
  printf("Please enter how much digits of random numbers you want: ");
  scanf("%d", &num);

  if (num == 0){
    exit(EXIT_FAILURE);
  }
  return num;
}

void generator(int32_t* array, size_t start, size_t end){
  
  if (end > start){
    array[start] = rand() % 10;
    generator(array, start + 1, end);
  }
}