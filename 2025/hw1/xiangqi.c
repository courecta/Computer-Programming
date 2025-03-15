#include "xiangqi.h"

sXiangqiRecord *initXiangqiRecord(void)
{
    sXiangqiRecord *record = (sXiangqiRecord *)malloc(sizeof(sXiangqiRecord));
    if (record == NULL)
        return NULL;

    // Initialize the board to empty
    memset(record->board, EMPTY, sizeof(record->board));
    
    // Initialize removed pieces arrays
    memset(record->red, 0, sizeof(record->red));
    memset(record->black, 0, sizeof(record->black));
    
    // Set up red pieces (bottom)
    record->board[0][0] = record->board[0][8] = RED_CHARIOT;
    record->board[0][1] = record->board[0][7] = RED_HORSE;
    record->board[0][2] = record->board[0][6] = RED_ELEPHANT;
    record->board[0][3] = record->board[0][5] = RED_ADVISOR;
    record->board[0][4] = RED_GENERAL;
    record->board[2][1] = record->board[2][7] = RED_CANNON;
    record->board[3][0] = record->board[3][2] = record->board[3][4] = 
    record->board[3][6] = record->board[3][8] = RED_SOLDIER;
    
    // Set up black pieces (top)
    record->board[9][0] = record->board[9][8] = BLACK_CHARIOT;
    record->board[9][1] = record->board[9][7] = BLACK_HORSE;
    record->board[9][2] = record->board[9][6] = BLACK_ELEPHANT;
    record->board[9][3] = record->board[9][5] = BLACK_ADVISOR;
    record->board[9][4] = BLACK_GENERAL;
    record->board[7][1] = record->board[7][7] = BLACK_CANNON;
    record->board[6][0] = record->board[6][2] = record->board[6][4] = 
    record->board[6][6] = record->board[6][8] = BLACK_SOLDIER;
    
    // Initialize movement record
    record->movementNumber = 0;
    record->pMovements = NULL;
    
    return record;
}

void freeXiangqiRecord(sXiangqiRecord *record)
{
    if (record != NULL) {
        if (record->pMovements != NULL) {
            free(record->pMovements);
        }
        free(record);
    }
}

// Helper function to get piece character representation
const char* getPieceSymbol(uint8_t piece)
{
    switch(piece)
    {
        case RED_GENERAL: return "G";    // 帥 (1)
        case RED_ADVISOR: return "A";    // 仕 (2)
        case RED_ELEPHANT: return "E";   // 相 (3)
        case RED_CHARIOT: return "R";    // 俥 (4)
        case RED_HORSE: return "H";      // 傌 (5)
        case RED_CANNON: return "C";     // 炮 (6)
        case RED_SOLDIER: return "S";    // 兵 (7)
        case BLACK_GENERAL: return "g";  // 將 (8)
        case BLACK_ADVISOR: return "a";  // 士 (9)
        case BLACK_ELEPHANT: return "e"; // 象 (10)
        case BLACK_CHARIOT: return "r";  // 車 (11)
        case BLACK_HORSE: return "h";    // 馬 (12)
        case BLACK_CANNON: return "c";   // 包 (13)
        case BLACK_SOLDIER: return "s";  // 卒 (14)
        default: return " ";
    }
}

int32_t printXiangqiRecord(sXiangqiRecord *record)
{
    if(record == NULL)
        return -1;

    printf("  0 1 2 3 4 5 6 7 8\n");
    printf(" +-----------------+\n");
    
    for(int i = 9; i >= 0; i--)
    {
        printf("%d|", i);
        
        for(int j = 0; j < 9; j++)
        {
            uint8_t piece = record->board[i][j];
            
            if (piece == EMPTY) {
                printf(" ");
            } else if (piece >= RED_GENERAL && piece <= RED_SOLDIER) {
                printf("%s%s%s", COLOR_RED, getPieceSymbol(piece), COLOR_RESET);
            } else {
                printf("%s%s%s", COLOR_BLUE, getPieceSymbol(piece), COLOR_RESET);
            }
            
            if (j < 8) {
                printf(" ");
            }
        }
        
        printf("|\n");
    }
    
    printf(" +-----------------+\n");
    
    // Print removed pieces
    printf("Removed Red: ");
    for (int i = 0; i < 16 && record->red[i] != 0; i++) {
        printf("%s%s%s ", COLOR_RED, getPieceSymbol(record->red[i]), COLOR_RESET);
    }
    printf("\n");
    
    printf("Removed Black: ");
    for (int i = 0; i < 16 && record->black[i] != 0; i++) {
        printf("%s%s%s ", COLOR_BLUE, getPieceSymbol(record->black[i]), COLOR_RESET);
    }
    printf("\n");
    
    return 0;
}

int32_t printXiangqiPlay(sXiangqiRecord *record)
{
    if (record == NULL) {
        return -1;
    }
    
    // Create a copy of the original record to work with
    sXiangqiRecord *tempRecord = initXiangqiRecord();
    if (tempRecord == NULL) {
        return -1;
    }
    
    // Print initial board
    printf("Initial board:\n");
    printXiangqiRecord(tempRecord);
    sleep(FRAMES);
    
    // Replay each move
    for (uint32_t i = 0; i < record->movementNumber; i++) {
        sMoveRecord move = record->pMovements[i];
        
        printf("\nMove %u: (%u,%u) to (%u,%u)\n", i+1, move.x1, move.y1, move.x2, move.y2);
        moveXiangqiRecord(tempRecord, move.x1, move.y1, move.x2, move.y2);
        printXiangqiRecord(tempRecord);
        sleep(FRAMES);
    }
    
    freeXiangqiRecord(tempRecord);
    return 0;
}

// Helper function to check if a piece is red
int isRed(uint8_t piece) {
    return piece >= RED_GENERAL && piece <= RED_SOLDIER;
}

// Helper function to check if a piece is black
int isBlack(uint8_t piece) {
    return piece >= BLACK_GENERAL && piece <= BLACK_SOLDIER;
}

// Helper function to check if it's red's turn
int isRedTurn(uint32_t moveNumber) {
    return moveNumber % 2 == 0;  // Red starts, so even numbers are red's turn
}

int32_t moveXiangqiRecord(sXiangqiRecord *record, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    if (record == NULL) {
        return -1;
    }
    
    // Check boundary
    if (x1 >= 10 || y1 >= 9 || x2 >= 10 || y2 >= 9) {
        return -1;
    }
    
    uint8_t piece = record->board[x1][y1];
    
    // Check if there's a piece at the source
    if (piece == EMPTY) {
        return -1;
    }
    
    // Check if it's the correct turn
    if ((isRedTurn(record->movementNumber) && !isRed(piece)) || 
        (!isRedTurn(record->movementNumber) && !isBlack(piece))) {
        return -1;
    }
    
    // Check if destination has a piece of the same color
    uint8_t destPiece = record->board[x2][y2];
    if ((isRed(piece) && isRed(destPiece)) || (isBlack(piece) && isBlack(destPiece))) {
        return -1;
    }
    
    // Note: A complete implementation would check each piece's valid movement patterns
    // This simplified version just allows any move that doesn't capture your own pieces
    
    // Record the captured piece if any
    if (destPiece != EMPTY) {
        if (isRed(destPiece)) {
            // Find first empty slot in red removed pieces
            for (int i = 0; i < 16; i++) {
                if (record->red[i] == 0) {
                    record->red[i] = destPiece;
                    break;
                }
            }
        } else {
            // Find first empty slot in black removed pieces
            for (int i = 0; i < 16; i++) {
                if (record->black[i] == 0) {
                    record->black[i] = destPiece;
                    break;
                }
            }
        }
    }
    
    // Update the board
    record->board[x2][y2] = piece;
    record->board[x1][y1] = EMPTY;
    
    // Record the movement
    record->movementNumber++;
    record->pMovements = (sMoveRecord *)realloc(record->pMovements, 
                                            record->movementNumber * sizeof(sMoveRecord));
    if (record->pMovements == NULL) {
        return -1;
    }
    
    record->pMovements[record->movementNumber - 1].x1 = x1;
    record->pMovements[record->movementNumber - 1].y1 = y1;
    record->pMovements[record->movementNumber - 1].x2 = x2;
    record->pMovements[record->movementNumber - 1].y2 = y2;
    
    return 0;
}