#include <stdio.h>

int main(){

  int fib[1000] = {0}, sum = 0;
  fib[0] = 0;
  fib[1] = 1;

  for(int i = 2 ; ; i++)
  {
    fib[i] = fib[i - 1] + fib[i - 2];
    if(fib[i] >= 4000000) break;
    if(fib[i] % 2 == 0) sum += fib[i];
  }

  printf("%d\n", sum);

  return 0;
}
