#pragma once

// Libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

// constants definitions
#define PI 3.14159265358979323846

// function to get number of user
uint16_t getNum();

// function to get user action
void await_input();
void menu( const uint16_t n );

// functions to calculate the irrational constants
double calculate_two( const uint16_t n );
void calculate_pi( const uint16_t n );
double calculate_euler( const uint16_t n );

// function to print lines
void print_line();

