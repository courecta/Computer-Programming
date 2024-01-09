#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main () {

    double invest, monthly, startMonth, startYear, endMonth, endYear, roR;
    int64_t limitInvest = pow(10, 7), limitYear = pow(10, 4);

    printf("Initial Investment:\t");
    scanf("%lf", &invest);
    printf("Recurring Monthly Investment: ");
    scanf("%lf", &monthly);
    printf("Starting Month:\t\t");
    scanf("%lf", &startMonth);
    printf("Starting Year:\t\t");
    scanf("%lf", &startYear);
    printf("End Month:\t\t");
    scanf("%lf", &endMonth);
    printf("End Year:\t\t");
    scanf("%lf", &endYear);
    printf("Annual Rate of Return :  ");
    scanf("%lf", &roR);

    if (invest < 1 || invest > limitInvest){
      printf("Error. The range you have entered for the Initial Investment is invalid!");
      return 1;
    }
    if (monthly < 0 || monthly > limitInvest){
      printf("Error. The range you have entered for the Recurring Monthly Investment is invalid!");
    return 1;
    }
    if (startMonth < 1 || startMonth > 12){
      printf("Error. The range you have entered for the Starting Month is invalid!");
    return 1;
    }
    if (startYear < 1 || startYear > limitYear){
      printf("Error. The range you have entered for the Starting Year is invalid!");
    return 1;
    }
    if (endMonth < 1 || endMonth > 12){
      printf("Error. The range you have entered for the Ending Month is invalid!");
    return 1;
    }
    if (endYear < 1 || endYear > limitYear){
      printf("Error. The range you have entered for the Ending Year is invalid!");
    return 1;
    }
    if (roR < 1 || roR > 100){
      printf("Error. The range you have entered for the rate of Return is invalid!");
    return 1;
    }

    printf("--- Output ---\n");

    int32_t timePass = (endMonth - startMonth) + ((endYear - startYear) * 12);
    double monthCount = 12 - startMonth, firstOver, totalWorth, totalGain, initialInvest = invest;

    roR = roR/12;

    for (int32_t i = 0 ; i <= timePass ; i++){
    
      if (i >= 1){
      totalWorth = (invest - initialInvest) + totalGain;
      }
      else {
        totalWorth = invest;
      }

     if (monthCount == i){
      startYear += 1;
      monthCount = 12;
      firstOver += 1;
     }

     if (startMonth > 12){
      startMonth = 1;
     }

     if (i % 12 == 0){
      startYear += 1;
     }

    printf("%lf.%lf) ", startYear, startMonth);
    printf("%lf/", invest);

    invest = invest + monthly;

    printf("%lf/", totalWorth);

    if (i >= 1){
    totalGain = ((roR/100) * invest);
    }
    else {
      totalGain = 0;
    }

    if (i >= 1){
      roR = (totalGain / totalWorth) * 100;
    }
    else {
      roR = 0;
    }

    printf("%lf/", totalGain);
    printf("%lf%%\n", roR);
    
    startMonth += 1;

    }


  return 0;
}