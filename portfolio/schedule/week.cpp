#include "week.h"

// CRUD for the editing the week

Week* createWeek(std::string schedule)
{
    Week* week = new Week;
    week->schedule = schedule;
    week->prev = nullptr;
    week->next = nullptr;
    return week;
}

void deleteWeek(Week*& week)
{
    if(week != nullptr)
    {
        if(week->prev != nullptr) week->prev->next = week->next;
        if(week->next != nullptr) week->next->prev = week->prev;w
        
        delete week;
        week = nullptr;
    }
}