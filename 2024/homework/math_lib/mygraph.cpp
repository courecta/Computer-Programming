#include "mygraph.h"

void DrawLine(int len)
{
    for(int i = 0 ; i < len ; i++) printf("-");
}

void DrawRectangle(int row, int col)
{
    if(row < 1)
    {
        printf("error! Invalid row number!\n");
        return;
    }

    if(col < 1)
    {
        printf("error! Invalid column number!\n");
        return;
    }

    if(row == 1 && col == 1)
    {
        printf("*\n");
        return;
    }

    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j) printf("*");
        printf("\n");
    }
}

void DrawSquare(int len)
{
    DrawRectangle(len, len);
}