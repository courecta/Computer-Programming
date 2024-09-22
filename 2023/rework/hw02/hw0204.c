#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main()
{
  double initial = 0, recurring = 0, startMonth = 0, startYear = 0, endMonth = 0, endYear = 0, rate = 0, total = 0;
  double netIncome = 0.0, currentRate = 0.0;

  printf("Initial Investment:\t\t");
  scanf("%lf", &initial);

  printf("Recurring Monthly Investment:\t");
  scanf("%lf", &recurring);

  printf("Start Month:\t\t\t");
  scanf("%lf", &startMonth);

  printf("Start Year:\t\t\t");
  scanf("%lf", &startYear);

  printf("End Month:\t\t\t");
  scanf("%lf", &endMonth);

  printf("End Year:\t\t\t");
  scanf("%lf", &endYear);

  printf("Annual Rate of Return (%%):\t");
  scanf("%lf", &rate);

  printf("--- Output ---\n");

  // handle error

  if(initial < 1.0 || initial > 10000000.0 || recurring < 0.0 || recurring > 10000000.0 || startMonth < 1.0 || startMonth > 12.0 || endMonth < 1.0 || endMonth > 12.0 || startYear < 1.0 || endYear < 1.0 || startYear > 10000.0 || endYear > 10000.0 || rate < 1.0 || rate > 100.0)
  {
    printf("error!\n");
    return 0;
  }

  rate /= 1200.0;
  double max = pow(10, 15), investment = initial;
  total = initial;
  double truncated_rate = 0.0;

  int32_t i = (int32_t)startYear, j = (int32_t)startMonth;

  while(i < endYear || j <= endMonth)
  {
    currentRate = ((total - initial) / total) * 100000;
    
    if(((int32_t)currentRate % 10) < 5) currentRate /= 10;
    else currentRate = (currentRate / 10) + 1;

    if(initial > max || total > max || netIncome > max)
    {
      printf("%d.%02d) ", i, j);
      printf("*/*/*/*%%");
      printf("\n");
      goto jump;
    }
    
    printf("%d.%02d) ", i, j);
    printf("%.0lf/", initial);
    printf("%.0lf/", total);
    printf("%.0lf/", total - initial);

    truncated_rate = (int32_t)currentRate % 100;

    if((currentRate / 100) > 100.0) printf("100%%"); 
    else if(currentRate == 0.0) printf("0%%");
    else printf("%d.%.0lf%%", (int32_t)(currentRate / 100.0), truncated_rate);

    printf("\n");

    investment = total * (1.0 + rate);
    total = investment + recurring;
    initial += recurring;

jump:
    if((j + 1) > 12)
    {
      j = 1;
      i++;
    } else j++;
  }
  return 0;
}