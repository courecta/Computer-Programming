#include "nav.h"

Week* navToPrev(Week* current)
{
    if(current->prev == nullptr || current == nullptr)
    {
        std::cout << "No previous week available\n";
        return current;
    }
    return current->prev;
}

Week* navToNext(Week* current)
{
    if(current->next == nullptr || current == nullptr)
    {
        std::cout << "No next week available\n";
        return current;
    }
    return current->next;
}