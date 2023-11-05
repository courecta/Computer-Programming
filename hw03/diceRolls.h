#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define n "\e[0;105m%d\e[0m"

void welcome();
void actionTime();
void roller( int32_t A, int32_t B, int32_t X, int32_t Y, int32_t H, int32_t L, int32_t C );
void choices( int32_t choice);
void roll1d6();
void rollAdX();
void rollAdXkYplusB();
void rollCustom();
void feature();
void getOut();

