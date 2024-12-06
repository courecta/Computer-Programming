#pragma once

#include "week.h"

Week* addWeek(Week* current, const std::string& schedule);
void editWeek(Week* week, const std::string& newSchedule);
void removeWeek(Week*& current);