#include <stdio.h>
#include <stdint.h>

int main(){

    int64_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
    int64_t endYear = 0, endMonth = 0, endDay = 0, endHour = 0, endMinute = 0, endSecond = 0;
    int64_t totalYear = 0, totalMonth = 0, totalDay = 0, totalHour = 0,totalMinute = 0, totalSecond = 0;

    printf("Start Time: ");
    scanf("%ld-%ld-%ld %ld:%ld:%ld", &year, &month, &day, &hour, &minute, &second);
    printf("End Time: ");
    scanf("%ld-%ld-%ld %ld:%ld:%ld", &endYear, &endMonth, &endDay, &endHour, &endMinute, &endSecond);
    
    endYear = endYear - year;

        if (endMonth - month > 0){
        endMonth = endMonth - month;
        totalMonth = endMonth + (12 * endYear);  
        }
        else if (endMonth - month < 0){
        endMonth = month - endMonth;
        totalMonth = (12 * endYear) - endMonth;  
        }

        if (endDay - day > 0){
            endDay = endDay - day;
            totalDay = endDay + (totalMonth * 30);
        }
        else if (endDay - day < 0){
            endDay = day - endDay;
            totalDay = (totalMonth * 30) - endDay;
        }

        if (endHour - hour > 0){
            endHour = endHour - hour;
            totalHour = endHour + (totalDay * 24);
        }
        else if (endHour - hour < 0){
            endHour = hour - endHour;
            totalHour = (totalDay * 24) - endHour;
        }

        if (endMinute - minute > 0){
            endMinute = endMinute - minute;
            totalMinute = endMinute + (totalHour * 60);
        }
        else if (endMinute - minute < 0){
            endMinute = minute - endMinute;
            totalMinute = (totalHour * 60) - endMinute;
        }

        if (endSecond - second > 0){
            endSecond = endSecond - second;
            totalSecond = endSecond + (totalMinute * 60);
        }
        else if (endSecond - second < 0){
            endSecond = second - endSecond;
            totalSecond = (totalMinute * 60) - endSecond;
        }
        else if (endSecond == second){
            totalSecond = totalMinute * 60;
        }

    printf("duration: %ld sec\n", totalSecond);
    // printf("%ld-%ld-%ld %ld:%ld:%ld\n", totalYear, totalMonth, totalDay, totalHour, totalMinute, totalSecond);

    return 0;
}