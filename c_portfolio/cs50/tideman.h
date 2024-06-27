#pragma once

// libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// macro constants
#define MAX 9

// function prototypes
bool vote(int rank, string name, int ranks[]);
bool cycle_pairs(int start, int endCase);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// global variables cause cs50 is kinda cringe
int32_t preferences[MAX][MAX];
bool locked[MAX][MAX];
int32_t pair_count;
int32_t candidate_count;

// structs
typedef struct
{
  int32_t winner;
  int32_t loser;
}
pair;

string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];
