#include "utils.h"
#include <iostream>

int validateInput(int min, int max)
{
    int choice;
    while(true)
    {
        std::cin >> choice;
        if(std::cin.fail() || choice < min || choice > max)
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid number! Please enter a number between " << min << " and " << max << ": ";
        } else return choice;
    }
}
