#pragma once

#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>
#include <string.h>

#define EOF (-1)

int my_printf(const char* format, ...);
int my_fputs(const char *str);
int my_fputc(char c);

void reverse(char *str, int length);
int int_to_str(int num, char *buffer, int precision, int force_sign, int space_sign);
int float_to_str(double num, char *buffer, int precision);
int uint_to_str(unsigned int num, char *buffer);
int uint_to_hex_str(unsigned int num, char *buffer, int uppercase);
int uint_to_oct_str(unsigned int num, char *buffer);
int float_to_exp_str(double num, char *buffer, int precision, int uppercase);
void format_exponent(int exponent, char *exp_buffer, int uppercase);