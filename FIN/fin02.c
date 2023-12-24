#include "mine.h"

int main(){
    int32_t board[16][30];
    int i, j;

    // 初始化遊戲板，所有單元格初始設為 -1（無地雷）
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 30; j++) {
            board[i][j] = -1;
        }
    }

    // 隨機設置 10 個地雷
    srand(time(NULL));  // 初始化隨機數生成器
    for (i = 0; i <60; i++) {
        int r = rand() % 16;
        int c = rand() % 30;
        if (board[r][c] == -2) {
            i--;  // 如果已經有地雷，重新選擇位置
        } else {
            board[r][c] = -2;
        }
    }

    // 首次調用 hit 函數
    while(1){
    int row, col;
    printf("Where do you want to hit? : ");
    scanf("%d %d", &row, &col);
    int result = hit(board, row, col);
    if (result == 1)
        {
        printf("dead\n");
        exit(0);
        }
    printBoard(board);
    printf("result = %d\n", result);
    }
    return 0;
}