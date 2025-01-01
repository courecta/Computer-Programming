#include <stdio.h>
#include <stdint.h>
#define MONTHS (12)

void populateCalendar(int date, int month_days[], uint8_t calendar[MONTHS][31]);

int main(void){

    int year = 0, date = 0, cases = 0, month = 0, day = 0;
    int array[10] = {0}, month_days[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t calendar[MONTHS][31] = {0};
    scanf("%d%d%d", &year, &date, &cases);
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) month_days[1]++;
    populateCalendar(date, month_days, calendar);

    for(int i = 0; i < cases; i++)
    {
        scanf("%d%d", &month, &day);
        if(month < 1 || month > MONTHS)
        {
            array[i] = -1;
            continue;
        }
        if(day < 1 || day > month_days[month - 1])
        {
            array[i] = -2;
            continue;
        }
        array[i] = calendar[month - 1][day - 1];
    }
    for(int i = 0; i < cases; i++) printf("%d ", array[i]);
    printf("\n");
    return 0;
}

void populateCalendar(int date, int month_days[], uint8_t calendar[MONTHS][31])
{
    int pointer = date;
    for(int i = 0; i < MONTHS; i++)
    {
        for(int j = 0; j < month_days[i]; j++)
        {
            if(pointer > 6) pointer = 0;
            calendar[i][j] = pointer;
            pointer++;
        }
    }
}