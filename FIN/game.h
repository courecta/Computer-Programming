#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define HORZ_BORDER 9
#define VERT_BORDER 1
#define EMPTY_SPACE 0
#define clear() printf("\033[H\033[J")
#define OBSTACLE 8
#define PLAYER 7
#define ENEMY1 2
#define ENEMY2 3
#define ENEMY3 4

void initial();
void build();

/*
void printBoard(int32_t board[16][30]) {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 30; j++) {
            if (board[i][j] == -1) {
                printf(". ");
            } else if (board[i][j] == -2) {
                printf("* ");
            } else {
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}
*/
