#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main()
{

  uint64_t initial = 0;
  int64_t recurring = 0;
  uint8_t startMonth = 0;
  uint16_t startYear = 0;
  uint8_t endMonth = 0;
  uint16_t endYear = 0;
  double rate = 0;
  uint64_t total = 0;
  double netIncome = 0.0, currentRate = 0.0;

  printf("Initial Investment: ");
  scanf("%lu", &initial);

  printf("Recurring Monthly Investment: ");
  scanf("%ld", &recurring);

  printf("Start Month: ");
  scanf("%hhu", &startMonth);

  printf("Start Year: ");
  scanf("%hu", &startYear);

  printf("End Month: ");
  scanf("%hhu", &endMonth);

  printf("End Year: ");
  scanf("%hu", &endYear);

  printf("Annual Rate of Return (%%): ");
  scanf("%lf", &rate);

  rate /= 1200;

  printf("--- Output ---\n");

  // handle error

  if(initial < 1 || initial > pow(10, 7) || recurring < 0 || recurring > pow(10, 7) || startMonth < 1 || startMonth > 12 || endMonth < 1 || endMonth > 12 || startYear < 1 || endYear < 1 || startYear > pow(10, 4) || endYear > pow(10, 4) || rate < 1 || rate > 100)
  {
    printf("error!\n");
    return 0;
  }

  uint16_t i = startYear, j = startMonth;

  while(i < endYear || j <= endMonth)
  {
    printf("%u.%u) ", i, j);

    printf("%lu/", initial);

    total = initial + netIncome;

    printf("%lu/", total);

    printf("%.0lf/", floor(netIncome));

    netIncome = netIncome + (total * rate);

    currentRate = (netIncome / total) * 100;

    printf("%.2lf%%", currentRate - 1.00);

    printf("\n");

    initial += recurring;

    if((j + 1) > 12)
    {
      j = 1;
      i++;
    } else j++;
  }

  return 0;
}
