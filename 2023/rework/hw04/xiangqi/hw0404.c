#include "mychess.h"

// SOLDIER: 3 pts
// (3, 3) -> (6, 4),(7, 3) ,(兵不能後退)
int board1[10][9] = {
{0,0,0,0,RED_GENERAL,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,RED_SOLDIER,0,0,0,0,0},
{0,0,0,BLACK_ADVISOR,BLACK_GENERAL,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,RED_SOLDIER,0,0,0,0}
};

// HORSE: 4 pts
// (6, 1) -> (8, 2) (拐馬腳)
// (7, 8) -> (8, 6)
// 排序: 1 pts
int board2[10][9] = {
{0,0,0,0,RED_GENERAL,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,RED_HORSE,BLACK_SOLDIER,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,RED_HORSE},
{0,0,0,0,0,0,BLACK_CHARIOT,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// MISC: 6 pts
// CHARIOT: (6, 2) -> (6, 4), (9, 2) 2 pts
// CANNON: (3, 6) -> (3, 4), (9, 6) 2 pts
// 不同種(CHARIOT, CANNON) 排序 1 pts
// 同種排序: 1 pts
int board3[10][9] = {
{0,0,0,0,RED_GENERAL,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,RED_CANNON,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_SOLDIER,0,0,0,0},
{0,0,RED_CHARIOT,0,0,0,BLACK_SOLDIER,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,BLACK_ELEPHANT,0,BLACK_GENERAL,BLACK_ADVISOR,BLACK_ELEPHANT,0,0}
};

// GENERAL: 1 pts
// (0, 3) -> (0, 4)
int board4[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 兵在我方倒退 1 pts
int board5[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,RED_SOLDIER,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 兵在我方向左 1 pts
int board6[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,RED_SOLDIER,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 象在不對的位置 1 pts
int board7[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,RED_ELEPHANT,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 象過河 1 pts
int board8[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,RED_ADVISOR,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{RED_ELEPHANT,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 士沒斜走 1 pts
int board9[10][9] = {
{0,0,0,RED_GENERAL,0,0,0,0,0},
{0,0,0,0,RED_ADVISOR,RED_ADVISOR,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

// Invalid case: 將超出範圍 1 pts
int board10[10][9] = {
{0,0,0,0,RED_GENERAL,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,RED_SOLDIER,0,0,0,0,0,0,0},
{0,0,BLACK_GENERAL,0,0,0,0,0,0}
};

// Invalid case: 多一顆 1 pts
int board11[10][9] = {
{0,0,0,0,RED_GENERAL,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{RED_SOLDIER,0,RED_SOLDIER,0,RED_SOLDIER,0,RED_SOLDIER,0,RED_SOLDIER},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,RED_SOLDIER,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0},
{0,0,0,0,BLACK_GENERAL,0,0,0,0}
};

int total = 0;
int log_index = 0;
int score_log[11] = {0,0,0,0,0,0,0,0,0,0,0};

void enter_score(){
    printf("Enter score: ");
    int score;
    scanf("%d", &score);
    total += score;
    score_log[log_index] = score;
    log_index += 1;
    system("clear");
}

int main(){

    // 編號未重編 - 1 pts
    printf("--------- board 1 3 pts---------\n");
    printf("Correct answer: \n");
    printf("1) Move Soldier from (6,3) to (6,4)\n");
    printf("2) Move Soldier from (6,3) to (7,3)\n");
    printf("3) 兵不能後退(前面要答對一個)(9,4)->(8,4)\n");
    printf("Your answer: \n");
    checkmate(board1);
    enter_score();
    printf("--------- board 2 4 pts---------\n");
    printf("Correct answer: \n");
    printf("1) Move Horse from (6,1) to (8,2)\n");
    printf("2) Move Horse from (7,8) to (8,6)\n");
    printf("3) 拐馬腳(6,1) to (7,3)\n");
    printf("4) 排序(1,2 都要對)\n");
    printf("Your answer: \n");
    checkmate(board2);
    enter_score();
    printf("--------- board 3 6 pts---------\n");
    printf("Correct answer: \n");
    printf("1) Move Chariot from (6,2) to (6,4)\n\
    2) Move Chariot from (6,2) to (9,2)\n\
    3) Move Cannon from (3,6) to (3,4)\n\
    4) Move Cannon from (3,6) to (9,6)\n");
    printf("5) 不同種(CHARIOT, CANNON) 排序\n");
    printf("6) 同種排序(1&2 或 3&4)\n");
    printf("Your answer: \n");
    checkmate(board3);
    enter_score();
    printf("--------- board 4 1 pts---------\n");
    printf("Correct answer: \n");
    printf("1) Move General from (0,3) to (0,4)\n");
    printf("Your answer: \n");
    checkmate(board4);
    enter_score();
    int error_score = 0;
    int error[7] = {1,1,1,1,1,1,1};
    if(checkmate(board5) == -1){
        score_log[4] = 1;
        error_score += 1;
    }
    if(checkmate(board6) == -1){
        score_log[5] = 1;
        error_score += 1;
    }
    if(checkmate(board7) == -1){
        score_log[6] = 1;
        error_score += 1;
    }
    if(checkmate(board8) == -1){
        score_log[7] = 1;
        error_score += 1;
    }
    if(checkmate(board9) == -1){
        score_log[8] = 1;
        error_score += 1;
    }
    if(checkmate(board10) == -1){
        score_log[9] = 1;
        error_score += 1;
    }
    if(checkmate(board10) == -1){
        score_log[10] = 1;
        error_score += 1;
    }

    int nosort = 0;
    printf("編號有重編: ");
    scanf("%d", &nosort);

    printf("<p>--------- score ---------</p>\n");
    printf("<p>Normal score: %d</p>\n", total);
    printf("<p>Error score: %d</p>\n", error_score);
    printf("<p>編號: %d</p>\n", nosort);
    printf("<p>Total score: %d</p>\n", total + error_score + nosort);
    printf("<p>--------- score log ---------</p>\n");
    printf("<p>");
    for(int i = 0; i < 11; i++){
        printf("%d ", score_log[i]);
    }
    printf("</p>\n");
    enter_score();
    return 0;
}