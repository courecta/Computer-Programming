#pragma once

#include <stdio.h>
#include <stdlib.h>

#define EMPTY (0)
#define RED_GENERAL (1)
#define RED_ADVISOR (2)
#define RED_ELEPHANT (3)
#define RED_HORSE (4)
#define RED_CHARIOT (5)
#define RED_CANNON (6)
#define RED_SOLDIER (7)
#define BLACK_GENERAL (11)
#define BLACK_ADVISOR (12)
#define BLACK_ELEPHANT (13)
#define BLACK_HORSE (14)
#define BLACK_CHARIOT (15)
#define BLACK_CANNON (16)
#define BLACK_SOLDIER (17)

// Return -1 if the board is invalid;
// otherwise , print all red side possible move to checkmate the
// black general and return the possible move number.

// Note the print should follow the piece order.
int checkmate(int board[10][9]);

int MOVE_GENERAL( int x , int y ,int times, int board[10][9] , int ans_general[5][2], int *check_general_ptr);
int MOVE_HORSE( int x, int y,int times, int board[10][9], int ans_horse[5][2], int *check_horse_ptr);
int MOVE_CHARIOT( int x, int y, int board[10][9], int ans_chariot[5][2] , int *check_chariot_ptr);
int MOVE_CANNON( int x, int y, int board[10][9], int ans_cannon[5][2] , int *check_cannon_ptr);
int MOVE_SOLDIER( int x, int y, int times, int board[10][9], int ans_soldier[5][2], int *check_soldier_ptr );
int CHARIOT_EAT_GENERAL(int x, int y, int board[10][9], int ans_chariot[5][2], int *check_chariot_ptr);
int CANNON_EAT_GENERAL(int x, int y, int board[10][9], int ans_cannon[5][2], int *check_cannon_ptr);

