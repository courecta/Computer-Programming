#include <stdio.h>

#define UNIX_EPOCH_YEAR (1970)
#define DAYS_IN_LEAP (366)
#define DAYS_IN_NORMAL (365)
#define HOURS (24)
#define MINUTES (60)
#define SECONDS (60)

unsigned long long dtos(int year, int month, int day, int hour, int min, int sec);
int isLeap(int year);

int main() {

      int start_year = 0, start_mon = 0, start_day = 0, start_hour = 0, start_min = 0, start_sec = 0;
      int end_year = 0, end_mon = 0, end_day = 0, end_hour = 0, end_min = 0, end_sec = 0;

      printf("Start Time: ");
      scanf("%d-%d-%d %d:%d:%d", 
            &start_year,
            &start_mon,
            &start_day,
            &start_hour,
            &start_min,
            &start_sec);
    
      printf("End Time:   ");
      scanf("%d-%d-%d %d:%d:%d",
            &end_year,
            &end_mon,
            &end_day,
            &end_hour,
            &end_min,
            &end_sec);

      unsigned long long start = dtos(start_year, start_mon, start_day, start_hour, start_min, start_sec);
      unsigned long long end = dtos(end_year, end_mon, end_day, end_hour, end_min, end_sec);
      
      printf("Duration: %llu sec(s)\n", end - start);

    return 0;
}

unsigned long long dtos(int year, int month, int day, int hour, int min, int sec)
{
      unsigned long long elapsed = 0;
      
      int days_in_a_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      
      if(isLeap(year)) days_in_a_month[1] = 29;
      
      for(int i = UNIX_EPOCH_YEAR ; i < year ; i++)
      {
            elapsed += (isLeap(i) ? DAYS_IN_LEAP : DAYS_IN_NORMAL) * HOURS * MINUTES * SECONDS;
      }

      for(int i = 0 ; i < month - 1; i++)
      {
            elapsed += days_in_a_month[i] * HOURS * MINUTES * SECONDS;
      }

      elapsed += (day - 1) * HOURS * MINUTES * SECONDS;
      elapsed += hour * MINUTES * SECONDS;
      elapsed += min * SECONDS;
      elapsed += sec;

      return elapsed;
}

int isLeap(int year)
{
      if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) return 1;
      return 0; 
}