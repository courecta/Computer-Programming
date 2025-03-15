#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ANSI color codes
#define COLOR_RED "\033[31m"
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

// Frame count
#define FRAMES (2)

typedef struct _sMoveRecord
{
    // Move a piece on (x1,y1) to (x2,y2)
    uint8_t x1, y1;
    uint8_t x2, y2;
} sMoveRecord;

typedef struct _sXiangqiRecord
{
    // Board
    uint8_t board[10][9];
    // Removed Pieces. They should be stored in the their remove order.
    uint8_t red[16];
    uint8_t black[16];
    // Movement Records
    uint32_t movementNumber;
    sMoveRecord *pMovements;
} sXiangqiRecord;

// Replace defines with an enum for piece types
typedef enum {
    EMPTY = 0,
    // Red pieces (1-7)
    RED_GENERAL = 1,    // 帥
    RED_ADVISOR = 2,    // 仕 
    RED_ELEPHANT = 3,   // 相
    RED_CHARIOT = 4,    // 俥
    RED_HORSE = 5,      // 傌
    RED_CANNON = 6,     // 炮
    RED_SOLDIER = 7,    // 兵
    // Black pieces (8-14)
    BLACK_GENERAL = 8,  // 將
    BLACK_ADVISOR = 9,  // 士
    BLACK_ELEPHANT = 10, // 象
    BLACK_CHARIOT = 11, // 車
    BLACK_HORSE = 12,   // 馬
    BLACK_CANNON = 13,  // 包
    BLACK_SOLDIER = 14  // 卒
} XiangqiPiece;

// Initialize and Free
sXiangqiRecord *initXiangqiRecord(void);
void freeXiangqiRecord(sXiangqiRecord *);

// Print all information in the structure. The print format is determined by yourself.
// The only requirement is that it should look like a board and every piece should be colored in blue and red.
// Input:
// sXiangqiRecord *: the current Xiangqi record.
// Return:
// -1 if the input is NULL; otherwise , 0.
int32_t printXiangqiRecord(sXiangqiRecord *);

// Slide show the play process.
// That is, display the board and update the board in 2 seconds till the end.
// Input:
// sXiangqiRecord *: the current Xiangqi record.
// Return:
// -1 if the input is NULL; otherwise , 0.
int32_t printXiangqiPlay(sXiangqiRecord *);

// Move the piece on board[x1][y1] to board[x2][y2].
// Input:
// sXiangqiRecord *: the current Xiangqi record.
// x1, y1: the piece location.
// x2, y2: the destination.
// Output:
// sXiangqiRecord *: You should update the board , removed pieces , and append this movement to the record.
// Return:
// 0: successful; -1: error.
// Some example errors:
// 1. NULL input.
// 2. Board boundary check.
// 3. The movement should be in turn , right?
// ... others
int32_t moveXiangqiRecord(sXiangqiRecord *, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);