#include "mychess.h"

int checkmate( int board[10][9] )
{
    //檢查棋盤是否有效
    int check_board[18];
    for ( int i = 0; i < 18; i++ )
    {
        check_board[i] = 0;
    }
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] < 0 || (board[i][j] > RED_SOLDIER && board[i][j] < BLACK_GENERAL) || board[i][j] > BLACK_SOLDIER )
                return -1;
            else
                check_board[board[i][j]] += 1;
            if ( board[i][j] == RED_GENERAL && (i>2 || j<3 || j>5) )//檢查紅將軍位置
                return -1;
            if ( board[i][j] == BLACK_GENERAL && (i<7 || j<3 || j>5) )//檢查黑將軍位置
                return -1;
            if ( board[i][j] == RED_ADVISOR )//檢查紅士位置
            {
                if( i == 0 && ( j==3 || j==5 ) )
                    continue;
                else if( i == 1 && j == 4 )
                    continue;
                else if( i == 2 && ( j==3 || j==5 ) )
                    continue;
                else
                    return -1;
            }
            if ( board[i][j] == BLACK_ADVISOR )//檢查黑士位置
            {
                if( i == 7 && ( j==3 || j==5 ) )
                    continue;
                else if( i == 8 && j == 4 )
                    continue;
                else if( i == 9 && ( j==3 || j==5 ) )
                    continue;
                else
                    return -1;
            }
            if ( board[i][j] == RED_ELEPHANT )//檢查紅象位置
            {
                if( i == 0 && ( j==2 || j==6 ) )
                    continue;
                else if( i == 2 && ( j==0 || j==4 || j==8 ) )
                    continue;
                else if( i == 4 && ( j==2 || j==6 ) )
                    continue;
                else
                    return -1;
            }
            if ( board[i][j] == BLACK_ELEPHANT )//檢查黑象位置
            {
                if( i == 5 && ( j==2 || j==6 ) )
                    continue;
                else if( i == 7 && ( j==0 || j==4 || j==8 ) )
                    continue;
                else if( i == 9 && ( j==2 || j==6 ) )
                    continue;
                else
                    return -1;
            }
            if ( board[i][j] == RED_SOLDIER )//檢查紅兵位置
            {
                if( (i == 3 || i == 4) && ( j==0 || j==2 || j == 4 || j == 6 || j == 8 ) )
                    continue;
                else if ( i >= 5 && i <= 9 )
                    continue;
                else return -1;
            }
            if ( board[i][j] == BLACK_SOLDIER )//檢查黑兵位置
            {
                if( (i == 5 || i == 6) && ( j==0 || j==2 || j == 4 || j == 6 || j == 8 ) )
                    continue;
                else if ( i >= 0 && i <= 4 )
                    continue;
                else return -1;
            }
        }
    }
    for ( int i = 0; i < 18; i++ )//檢查棋子數量
    {
        if ( i == RED_GENERAL || i== BLACK_GENERAL )
        {
            if ( check_board[i] != 1 )
                return -1;
        }
        else if ( (i >= RED_ADVISOR && i <= RED_CANNON) || (i >= BLACK_ADVISOR && i <= BLACK_CANNON) )
        {
            if ( check_board[i] > 2 )
                return -1;
        }
        else if ( i==RED_SOLDIER || i == BLACK_SOLDIER )
        {
            if ( check_board[i] > 5 )
                return -1;
        }
    }
    //輸出
    int check_general = 0, check_horse = 0, check_chariot = 0, check_cannon = 0, check_soldier = 0;
    int *check_general_ptr = &check_general, *check_horse_ptr = &check_horse, *check_chariot_ptr = &check_chariot, *check_cannon_ptr = &check_cannon, *check_soldier_ptr = &check_soldier;
    int ans_general[5][2]={0}, ans_horse[5][2]={0}, ans_chariot[5][2]={0}, ans_cannon[5][2]={0}, ans_soldier[5][2]={0};
    int number = 1;

    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] == RED_GENERAL )
            {
                if ( MOVE_GENERAL(i,j,1,board,ans_general,check_general_ptr) == 1 )
                {
                    for ( int k=0; k<*check_general_ptr; k++ )
                    {
                        printf("%d) Move General from (%d,%d) to (%d,%d)\n", number, i, j, ans_general[k][0], ans_general[k][1] );
                        number += 1;
                    }
                    *check_general_ptr = 0;
                }
            }
        }
    }
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] == RED_HORSE )
            {
                if ( MOVE_HORSE(i,j,1,board,ans_horse,check_horse_ptr) == 1 )
                {
                    for ( int k=0; k<*check_horse_ptr; k++ )
                    {
                        printf("%d) Move Horse from (%d,%d) to (%d,%d)\n", number, i, j, ans_horse[k][0], ans_horse[k][1] );
                        number += 1;
                    }
                    *check_horse_ptr = 0;
                }
            }
        }
    }
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] == RED_CHARIOT )
            {
                if ( MOVE_CHARIOT(i,j,board,ans_chariot,check_chariot_ptr) == 1 )
                {
                    for ( int k=0; k<*check_chariot_ptr; k++ )
                    {
                        printf("%d) Move Chariot from (%d,%d) to (%d,%d)\n", number, i, j, ans_chariot[k][0], ans_chariot[k][1] );
                        number += 1;
                    }
                    *check_chariot_ptr = 0;
                }
            }
        }
    }
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] == RED_CANNON )
            {
                if ( MOVE_CANNON(i,j,board,ans_cannon,check_cannon_ptr) == 1 )
                {
                    for ( int k=0; k<*check_cannon_ptr; k++ )
                    {
                        printf("%d) Move Cannon from (%d,%d) to (%d,%d)\n", number, i, j, ans_cannon[k][0], ans_cannon[k][1] );
                        number += 1;
                    }
                    *check_cannon_ptr = 0;
                }
            }
        }
    }
    for ( int i = 0; i < 10; i++ )
    {
        for ( int j = 0; j < 9; j++ )
        {
            if ( board[i][j] == RED_SOLDIER )
            {
                if ( MOVE_SOLDIER(i,j,1,board,ans_soldier,check_soldier_ptr) == 1 )
                {
                    for ( int k=0; k<*check_soldier_ptr; k++ )
                    {
                        printf("%d) Move Soldier from (%d,%d) to (%d,%d)\n", number, i, j, ans_soldier[k][0], ans_soldier[k][1] );
                        number += 1;
                    }
                    *check_soldier_ptr = 0;
                }
            }
        }
    }
}

int MOVE_GENERAL( int x , int y ,int times, int board[10][9] , int ans_general[5][2], int *check_general_ptr)
{
    int walk[2][2] = { {0,1}, {0,-1} };
    for ( int i = 0; i < 2; i++ )
    {
        if ( times == 1 )
        {
            int tmpx = x + walk[i][0];
            int tmpy = y + walk[i][1];
            if ( tmpx<0 || tmpx>2 || tmpy<3 || tmpy>5 )
                continue;
            if ( board[tmpx][tmpy]>=RED_GENERAL && board[tmpx][tmpy]<=RED_SOLDIER )
                continue;
            MOVE_GENERAL( tmpx, tmpy, 2, board, ans_general, check_general_ptr );
        }
        if ( times == 2 )
        {
            for ( int j = x+1; j<10 ; j++ )
            {
                if ( board[j][y] == BLACK_GENERAL )
                {
                    ans_general[*check_general_ptr][0] = x;
                    ans_general[*check_general_ptr][1] = y;
                    *check_general_ptr += 1;
                    return 0;
                }
                else if ( board[j][y] != EMPTY )
                    return 0;
            }
        }
    }
    if ( *check_general_ptr != 0 )
        return 1;
    else
        return 0;
}

int MOVE_HORSE( int x, int y,int times, int board[10][9], int ans_horse[5][2], int *check_horse_ptr)
{
    int walk[8][2] = { {-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1} };
    int leg[8][2] = { {-1,0}, {-1,0}, {0,-1}, {0,1}, {0,-1}, {0,1}, {1,0}, {1,0} };
    for ( int i = 0; i < 8; i++ )
    {
        int tmpx = x + walk[i][0];
        int tmpy = y + walk[i][1];
        int tmplegx = x + leg[i][0];
        int tmplegy = y + leg[i][1];
        if ( tmpx<0 || tmpx>9 || tmpy<0 || tmpy>8 )
            continue;
        if ( board[tmpx][tmpy]>=RED_GENERAL && board[tmpx][tmpy]<=RED_SOLDIER )
            continue;
        if ( board[tmplegx][tmplegy] != EMPTY )
            continue;
        if ( times == 1 )
            MOVE_HORSE( tmpx, tmpy, 2, board, ans_horse, check_horse_ptr );
        if ( times == 2 )
        {
            if ( board[tmpx][tmpy] == BLACK_GENERAL )
            {
                ans_horse[*check_horse_ptr][0] = x;
                ans_horse[*check_horse_ptr][1] = y;
                *check_horse_ptr += 1;
                return 0;
            }
            else
                continue;
        }
    }
    if ( *check_horse_ptr != 0 )
        return 1;
    else
        return 0;
}

int MOVE_CHARIOT( int x, int y, int board[10][9], int ans_chariot[5][2] , int *check_chariot_ptr)
{
    int block=0;
    for ( int i=x-1 ; i>=0 ; i-- )
    {
        if ( board[i][y]>=RED_GENERAL && board[i][y]<=RED_SOLDIER )
            break;
        if ( board[i][y]>=BLACK_GENERAL && board[i][y]<=BLACK_SOLDIER )
            block += 1;
        if ( block > 1 )
            break;
        else
            CHARIOT_EAT_GENERAL( i, y, board, ans_chariot, check_chariot_ptr );
    }
    block = 0;
    for ( int i=y-1 ; i>=0 ; i-- )
    {
        if ( board[x][i]>=RED_GENERAL && board[x][i]<=RED_SOLDIER )
            break;
        if ( board[x][i]>=BLACK_GENERAL && board[x][i]<=BLACK_SOLDIER )
            block += 1;
        if ( block > 1 )
            break;
        else
            CHARIOT_EAT_GENERAL( x, i, board, ans_chariot, check_chariot_ptr );
    }
    block = 0;
    for ( int i=y+1 ; i<=8 ; i++ )
    {
        if ( board[x][i]>=RED_GENERAL && board[x][i]<=RED_SOLDIER )
            break;
        if ( board[x][i]>=BLACK_GENERAL && board[x][i]<=BLACK_SOLDIER )
            block += 1;
        if ( block > 1 )
            break;
        else
            CHARIOT_EAT_GENERAL( x, i, board, ans_chariot, check_chariot_ptr );
    }
    block = 0;
    for ( int i=x+1 ; i<=9 ; i++ )
    {
        if ( board[i][y]>=RED_GENERAL && board[i][y]<=RED_SOLDIER )
            break;
        if ( board[i][y]>=BLACK_GENERAL && board[i][y]<=BLACK_SOLDIER )
            block += 1;
        if ( block > 1 )
            break;
        else
            CHARIOT_EAT_GENERAL( i, y, board, ans_chariot, check_chariot_ptr );
    }
    if ( *check_chariot_ptr != 0 )
        return 1;
    else
        return 0;
}

int MOVE_CANNON( int x, int y, int board[10][9], int ans_cannon[5][2] , int *check_cannon_ptr)
{
    int block=0;
    for ( int i=x-1 ; i >= 0 ; i-- )
    {
        if ( board[i][y]!= 0 )
            block += 1;
        if ( block == 0 )
            CANNON_EAT_GENERAL( i, y, board, ans_cannon, check_cannon_ptr );
        if ( block == 1 )
        {
            int landing_point=0;
            for ( int j=i-1 ; j >= 0 ; j-- )
            {
                if ( board[j][y]!= EMPTY )
                    landing_point += 1;
                if ( landing_point==1 && board[j][y]>=BLACK_GENERAL && board[j][y]<=BLACK_SOLDIER )
                {
                    CANNON_EAT_GENERAL( j, y, board, ans_cannon, check_cannon_ptr );
                    break;
                }
                if ( landing_point > 1 )
                    break;
            }
            break;
        }
    }
    block=0;
    for ( int i=y-1 ; i >= 0 ; i-- )
    {
        if ( board[x][i]!= 0 )
            block += 1;
        if ( block == 0 )
            CANNON_EAT_GENERAL( x, i, board, ans_cannon, check_cannon_ptr );
        if ( block == 1 )
        {
            int landing_point=0;
            for ( int j=i-1 ; j >= 0 ; j-- )
            {
                if ( board[x][j]!= EMPTY )
                    landing_point += 1;
                if ( landing_point==1 && board[x][j]>=BLACK_GENERAL && board[x][j]<=BLACK_SOLDIER )
                {
                    CANNON_EAT_GENERAL( x, j, board, ans_cannon, check_cannon_ptr );
                    break;
                }
                if ( landing_point > 1 )
                    break;
            }
            break;
        }
    }
    block=0;
    for ( int i=y+1 ; i <= 8 ; i++ )
    {
        if ( board[x][i]!= 0 )
            block += 1;
        if ( block == 0 )
            CANNON_EAT_GENERAL(x,i,board,ans_cannon,check_cannon_ptr);
        if ( block == 1 )
        {
            int landing_point=0;
            for ( int j=i+1 ; j < 8 ; j++ )
            {
                if ( board[x][j]!= EMPTY )
                    landing_point += 1;
                if ( landing_point==1 && board[x][j]>=BLACK_GENERAL && board[x][j]<=BLACK_SOLDIER )
                {
                    CANNON_EAT_GENERAL(x,j,board,ans_cannon,check_cannon_ptr);
                    break;
                }
                if ( landing_point > 1 )
                    break;
            }
            break;
        }
    }
    block=0;
    for ( int i=x+1 ; i <= 9 ; i++ )
    {
        if ( board[i][y]!= 0 )
            block += 1;
        if ( block == 0 )
            CANNON_EAT_GENERAL(i,y,board,ans_cannon,check_cannon_ptr);
        if ( block == 1 )
        {
            int landing_point=0;
            for ( int j=i+1 ; j < 9 ; j++ )
            {
                if ( board[j][y]!= EMPTY )
                    landing_point += 1;
                if ( landing_point==1 && board[j][y]>=BLACK_GENERAL && board[j][y]<=BLACK_SOLDIER )
                {
                    CANNON_EAT_GENERAL(j,y,board,ans_cannon,check_cannon_ptr);
                    break;
                }
                if ( landing_point > 1 )
                    break;
            }
            break;
        }
    }
    if ( *check_cannon_ptr != 0 )
        return 1;
    else
        return 0;
}

int MOVE_SOLDIER( int x, int y, int times, int board[10][9], int ans_soldier[5][2], int *check_soldier_ptr )
{
    int walk[3][2] = { {0,-1}, {0,1}, {1,0} };
    for ( int i = 0; i < 3; i++ )
    {
        int tmpx = x + walk[i][0];
        int tmpy = y + walk[i][1];
        if ( tmpx<0 || tmpx>9 || tmpy<0 || tmpy>8 )
            continue;
        if ( board[tmpx][tmpy]>=RED_GENERAL && board[tmpx][tmpy]<=RED_SOLDIER )
            continue;
        if ( times == 1 )
            MOVE_SOLDIER( tmpx, tmpy, 2, board, ans_soldier, check_soldier_ptr );
        if ( times == 2 )
        {
            if ( board[tmpx][tmpy] == BLACK_GENERAL )
            {
                ans_soldier[*check_soldier_ptr][0] = x;
                ans_soldier[*check_soldier_ptr][1] = y;
                *check_soldier_ptr += 1;
                return 0;
            }
            else
                continue;
        }
    }
    if ( *check_soldier_ptr != 0 )
        return 1;
    else
        return 0;
}

int CHARIOT_EAT_GENERAL(int x, int y, int board[10][9], int ans_chariot[5][2], int *check_chariot_ptr)
{
    for ( int i=x-1 ; i>=0 ; i-- )
    {
        if ( board[i][y]!=EMPTY && board[i][y]!=BLACK_GENERAL )
            break;
        if ( board[i][y] == BLACK_GENERAL )
        {
            ans_chariot[*check_chariot_ptr][0] = x;
            ans_chariot[*check_chariot_ptr][1] = y;
            *check_chariot_ptr += 1;
            return 0;
        }
    }
    for ( int i=y-1 ; i>=0 ; i-- )
    {
        if ( board[x][i]!=EMPTY && board[x][i]!=BLACK_GENERAL )
            break;
        if ( board[x][i] == BLACK_GENERAL )
        {
            ans_chariot[*check_chariot_ptr][0] = x;
            ans_chariot[*check_chariot_ptr][1] = y;
            *check_chariot_ptr += 1;
            return 0;
        }
    }
    for ( int i=y+1 ; i<=8 ; i++ )
    {
        if ( board[x][i]!=EMPTY && board[x][i]!=BLACK_GENERAL )
            break;
        if ( board[x][i] == BLACK_GENERAL )
        {
            ans_chariot[*check_chariot_ptr][0] = x;
            ans_chariot[*check_chariot_ptr][1] = y;
            *check_chariot_ptr += 1;
            return 0;
        }
    }
    for ( int i=x+1 ; i<=9 ; i++ )
    {
        if ( board[i][y]!=EMPTY && board[i][y]!=BLACK_GENERAL )
            break;
        if ( board[i][y] == BLACK_GENERAL )
        {
            ans_chariot[*check_chariot_ptr][0] = x;
            ans_chariot[*check_chariot_ptr][1] = y;
            *check_chariot_ptr += 1;
            return 0;
        }
    }
}

int CANNON_EAT_GENERAL(int x, int y, int board[10][9], int ans_cannon[5][2], int *check_cannon_ptr)
{
    int block=0;
    for ( int i=x-1 ; i>=0 ; i-- )
    {
        if ( board[i][y] != EMPTY )
            block += 1;
        if ( board[i][y]== RED_CANNON )
            block -= 1;
        if ( block>2 )
            break;
        if ( block == 2 && board[i][y] == BLACK_GENERAL )
        {
            ans_cannon[*check_cannon_ptr][0] = x;
            ans_cannon[*check_cannon_ptr][1] = y;
            *check_cannon_ptr += 1;
            return 0;
        }
    }
    block = 0;
    for ( int i=y-1 ; i>=0 ; i-- )
    {
        if ( board[x][i]!= EMPTY )
            block += 1;
        if ( board[x][i]== RED_CANNON )
            block -= 1;
        if ( block>2 )
            break;
        if ( block == 2 && board[x][i] == BLACK_GENERAL )
        {
            ans_cannon[*check_cannon_ptr][0] = x;
            ans_cannon[*check_cannon_ptr][1] = y;
            *check_cannon_ptr += 1;
            return 0;
        }
    }
    block = 0;
    for ( int i=y+1 ; i<=8 ; i++ )
    {
        if ( board[x][i]!= EMPTY )
            block += 1;
        if ( board[x][i]== RED_CANNON )
            block -= 1;
        if ( block>2 )
            break;
        if ( block == 2 && board[x][i] == BLACK_GENERAL )
        {
            ans_cannon[*check_cannon_ptr][0] = x;
            ans_cannon[*check_cannon_ptr][1] = y;
            *check_cannon_ptr += 1;
            return 0;
        }
    }
    block = 0;
    for ( int i=x+1 ; i<=9 ; i++ )
    {
        if ( board[i][y]!= EMPTY )
            block += 1;
        if ( board[i][y]== RED_CANNON )
            block -= 1;
        if ( block>2 )
            break;
        if ( block == 2 && board[i][y] == BLACK_GENERAL )
        {
            ans_cannon[*check_cannon_ptr][0] = x;
            ans_cannon[*check_cannon_ptr][1] = y;
            *check_cannon_ptr += 1;
            return 0;
        }
    }
}