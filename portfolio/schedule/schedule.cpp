#include "schedule.h"

Week* addWeek(Week* current, const std::string& schedule)
{
    Week* newWeek = createWeek(schedule);

    newWeek->prev = current;
    if(current != nullptr)
    {
        newWeek->next = current->next;
        if(current->next != nullptr) current->next->prev = newWeek;
        current->next = newWeek;
    }
    return newWeek;
}

void editWeek(Week* week, const std::string& newSchedule)
{
    if(week != nullptr) week->schedule = newSchedule;
}

void removeWeek(Week*& current)
{
    if(current == nullptr) return;

    Week* temp = current;

    if(current->prev != nullptr) current = current->prev;
    else if(current->next != nullptr) current = current->next;
    else current = nullptr;

    deleteWeek(temp);
}