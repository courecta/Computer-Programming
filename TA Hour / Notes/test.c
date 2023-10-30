#include <stdio.h>

int x, y;

void message();
void printSum(int x, int y);


int main(void){

  message();

  return 0;
}

void message(){
    printf("Welcome to the battlegrounds...");
    printSum(x, y);
}

void printSum (int x, int y){
  printf("Please give two integers\n");
  scanf("%d %d", &x,&y);
  printf("%d + %d is %d\n",x,y,x+y);
}
