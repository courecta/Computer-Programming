#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Input:
// const char *pStr: the input string to be split.
// const char *delim: the separator string.
// Output:
// char ***pppStrs: the substring list.
// Return:
// The number of split strings. If there is any error input , return -1.
int32_t mysplit(char ***pppStrs , const char *pStr, const char *delim);