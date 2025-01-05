#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX (256)

int winner(char player1[MAX], char player2[MAX]);

int main(void){
    char player1[MAX] = {'\0'};
    char player2[MAX] = {'\0'};

    printf("Player 1: ");
    scanf("%s", player1);
    printf("Player 2: ");
    scanf("%s", player2);

    int result = winner(player1, player2);
    if(result == 0) printf("Tie!\n");
    if(result == 1) printf("Player 1 wins!\n");
    if(result == 2) printf("Player 2 wins!\n");

    return 0;
}

int winner(char player1[MAX], char player2[MAX])
{
    int value[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int score1 = 0, score2 = 0;
    for(int i = 0; player1[i] != '\0'; i++)
    {
        if(!isalpha(player1[i])) continue;
        score1 += value[tolower(player1[i]) - 'a'];
    }
    for(int i = 0; player2[i] != '\0'; i++)
    {
        if(!isalpha(player2[i])) continue;
        score2 += value[tolower(player2[i]) - 'a'];
    }
    if(score1 == score2) return 0;
    return (score1 > score2) ? 1 : 2;
}