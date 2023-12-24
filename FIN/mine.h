#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/*
A player selects a cell to open it. If a player opens a mined cell, the game ends. Otherwise,
the opened cell displays either a number, indicating the number of mines diagonally and/or
adjacent to it, or a blank tile (or ”0”), and all adjacent non-mined cells will automatically
be opened. For your simplicity, the board size is fixed to 16 × 30.
You need to implement a function to print the board result after hitting a given position.
The user will input the board, which is a 16×30 two-dimensional array. Each element should
be -1 or -2 where -1 implies there is no mine at that position and -2 implies there is a mine
at that position. The user also input two int32_t, row and col, to indicate the hit position,
which is board[row][col]. You need to modify the board array to the board result after
hitting a position.
*/

// board: game board. If the cell is a mine , the value is -2; otherwise , -1.
// row, col: implies hitting board[row][col].
// Note that the row and col should start from the top left corner and start from 0.
// Return -1 if the input is invalid and return 1 is hit the mine; otherwise return 0.

int32_t hit( int32_t board[16][30], int32_t row, int32_t col );
void printBoard(int32_t board[16][30]); //<-- for testing purposes

/*
Let’s give you an example. However, for my own good, I just give you a small size board
example.
Before

int32_t board[4][5] = { {-1,-1,-1,-2,-1},
{-1,-2,-1,-1,-1},
{-1,-2,-1,-1,-1},
{-1,-1,-2,-1,-1} };

Case 1: hitting (1,2), set the value to the number of surrounding mines.
int32_t board[4][5] = { {-1,-1,-1,-2,-1},
{-1,-2, 3,-1,-1},
{-1,-2,-1,-1,-1},
{-1,-1,-2,-1,-1} };

Case 2: hitting (1,1), mine is blown up and you do not need to modify the array value.
Just return 1.
int32_t board[4][5] = { {-1,-1,-1,-2,-1},
{-1,-2,-1,-1,-1},
{-1,-2,-1,-1,-1},
{-1,-1,-2,-1,-1} };

Case 3: hitting (2,4), since there is no surrounding mine , set the value to 0.
You should also open surrounding blocks.
int32_t board[4][5] = { {-1,-1,-1,-2,-1},
{-1,-2,-1, 1, 1},
{-1,-2,-1, 1, 0},
{-1,-1,-2, 1, 0} };

*/