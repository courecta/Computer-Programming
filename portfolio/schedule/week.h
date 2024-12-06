#pragma once

#include <iostream>
#include <string>

struct Week{
  std::string schedule;
  Week* prev;
  Week* next;
};

Week* createWeek(std::string schedule);
void deleteWeek(Week*& week);