#include <stdio.h>

int main(){

  int d(0), g(0), r(0), y(0);

  printf("please input the amount of your annual deposit, your goal savings, and the annual interest rate...> ");
  scanf("%d %d %d", &d, &g, &r);
  if(d < 0 || g < 0 || r < 0) {printf("input error.\n"); return 0;}
  
  int c(0);

  while(c <= g)
  {
    c = (c + d) * (1 + r/100.0);
    y++;
    printf("year %d: %d\n", y, c);
  }
  
  printf("After %d year(s), you have $%d, reaching the goal of $%d\n", y, c, g);

  return 0;
}
