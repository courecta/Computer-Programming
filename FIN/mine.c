#include "mine.h"

int32_t hit(int32_t board[16][30], int32_t row, int32_t col)
{
    // Check for invalid input
    if (row < 0 || row >= 16 || col < 0 || col >= 30) return -1;

    // Check if the hit cell contains a mine
    if (board[row][col] == -2) return 1;

    if (board[row][col] != -1) return 0;

    int32_t minesCount = 0;

    for (int32_t i = -1; i <= 1; ++i) {
        for (int32_t j = -1; j <= 1; ++j) {
            int32_t newRow = row + i;
            int32_t newCol = col + j;

            // Check if the new position is within bounds
            if (newRow >= 0 && newRow < 16 && newCol >= 0 && newCol < 30) {
                // Check if the cell contains a mine
                if (board[newRow][newCol] == -2) {
                    ++minesCount;
                }
            }
        }
    }

    // Update the hit cell with the number of surrounding mines
    board[row][col] = minesCount;

    // If no surrounding mines, open adjacent cells
    if (minesCount == 0) {
        for (int32_t i = -1; i <= 1; ++i) {
            for (int32_t j = -1; j <= 1; ++j) {
                int32_t newRow = row + i;
                int32_t newCol = col + j;

                // Check if the new position is within bounds
                if (newRow >= 0 && newRow < 16 && newCol >= 0 && newCol < 30) {
                    // Recursively call hit on adjacent cells
                    hit(board, newRow, newCol);
                }
            }
        }
    }

    return 0;
}

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