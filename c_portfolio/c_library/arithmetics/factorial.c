#include <stdio.h>
int factorial(int n);

int main(int argc, char* argv[]){
  long num = 0;
  for(int i = 1 ; i < argc ; i++)
  {
    
  }
  printf("%d! = %d\n", num, factorial(num));
  return 0;
}

int factorial(int n)
{
  if(n == 0 || n == 1) return 1;
  return n * factorial(n - 1);
}
