#include "game.h"

int64_t height = 0, width = 0; 

void initial()
{

    printf("Please input the height of the map: ");
    scanf("%ld", &height);
    printf("Please input the width of the map: ");
    scanf("%ld", &width);
    clear();
    build(height, width);

}

void build()
{

    int64_t field[height + 1][width + 1];
    for (size_t i = 0 ; i <= height ; ++i)
    {
        for (size_t j = 0 ; j <= width ; ++j)
        {
            if (i == 0 || i == height) field[i][j] = HORZ_BORDER;
            if (j == 0 || j == width) field[i][j] = VERT_BORDER;
            if (i != 0 && i != height && j != 0 && j != width) field[i][j] = EMPTY_SPACE;
        }
    }

    int64_t obstacleCount = round((height * width) / 40);
    srand(time(NULL));

    for (;;)
    {
        if (obstacleCount == 0) break;
        int64_t obstacleSpawnVert = rand() % (height - 2);
        int64_t obstacleSpawnHorz = rand() % (width - 2);
        if (field[obstacleSpawnVert][obstacleSpawnHorz] != EMPTY_SPACE) continue;
        field[obstacleSpawnVert][obstacleSpawnHorz] = OBSTACLE;
        --obstacleCount;
    }

    for (;;)
    {
        int64_t playerSpawnVert = rand() % (height - 2);
        int64_t playerSpawnHorz = rand() % (width - 2);
        if (field[playerSpawnVert][playerSpawnHorz] == EMPTY_SPACE && field[playerSpawnVert - 1][playerSpawnHorz] == EMPTY_SPACE)
        {
            field[playerSpawnVert][playerSpawnHorz] = PLAYER;
            break;
        }
    }

    for (;;)
    {
        int64_t computerSpawnVert = rand() % (height - 2);
        int64_t computerSpawnHorz = rand() % (width - 2);
        if (field[computerSpawnVert][computerSpawnHorz] == EMPTY_SPACE && field[computerSpawnVert - 1][computerSpawnHorz] == EMPTY_SPACE)
        {
            field[computerSpawnVert][computerSpawnHorz] = ENEMY1;
            break;
        }
    }

    for (;;)
    {
        int64_t computerSpawnVert = rand() % (height - 2);
        int64_t computerSpawnHorz = rand() % (width - 2);
        if (field[computerSpawnVert][computerSpawnHorz] == EMPTY_SPACE && field[computerSpawnVert - 1][computerSpawnHorz] == EMPTY_SPACE)
        {
            field[computerSpawnVert][computerSpawnHorz] = ENEMY2;
            break;
        }
    }

    for (;;)
    {
        int64_t computerSpawnVert = rand() % (height - 2);
        int64_t computerSpawnHorz = rand() % (width - 2);
        if (field[computerSpawnVert][computerSpawnHorz] == EMPTY_SPACE && field[computerSpawnVert - 1][computerSpawnHorz] == EMPTY_SPACE)
        {
            field[computerSpawnVert][computerSpawnHorz] = ENEMY3;
            break;
        }
    }

    for (int32_t i = 0; i < height; i++)
    {
        for (int32_t j = 0; j < width; j++)
        {
            if (field[i + 1][j] == PLAYER)
            {
                printf("I");
            }
            else if (field[i + 1][j] == ENEMY1 || field[i + 1][j] == ENEMY2 || field[i + 1][j] == ENEMY3)
            {
                printf("I");
            }
            else if (field[i][j] == HORZ_BORDER)
            {
                printf("-");
            } else if (field[i][j] == VERT_BORDER)
            {
                printf("|");
            } else if (field[i][j] == EMPTY_SPACE)
            {
                printf(" ");
            }
            else if (field[i][j] == OBSTACLE)
            {
                printf("R");
            }
            else if (field[i][j] == PLAYER)
            {
                printf("P");
            }
            else if (field[i][j] == ENEMY1 || field[i][j] == ENEMY2 || field[i][j] == ENEMY3 )
            {
                printf("C ");
            }
            
        }
        printf("\n");
    }


}

