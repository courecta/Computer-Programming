#include "xiangqi.h"
// Ask TAs : is the slide show supposed to be 1 or 2 seconds?
int main(void) {
    // Game 1: Basic valid moves (no captures)
    sXiangqiRecord *game1 = initXiangqiRecord();
    if (game1 == NULL) {
        printf("Failed to initialize Xiangqi game1\n");
        return -1;
    }
    
    printf("Game 1: Basic valid moves (no captures)\n");
    // Move 1: Red horse from (0,1) to (2,0)
    moveXiangqiRecord(game1, 0, 1, 2, 0);
    // Move 2: Black horse from (9,1) to (7,0)
    moveXiangqiRecord(game1, 9, 1, 7, 0);
    
    printf("Game 1 End Board:\n");
    printXiangqiRecord(game1);
    /* Expected Board for Game 1:
       (A sample expected layout based on the moves; adjust as needed)
       9| r . e a g a e h r
       8| . . . . . . . . .
       7| h . . . . . . . .
       6| s . s . s . s . s
       5| . . . . . . . . .
       4| S . S . S . S . S
       3| . . . . . . . . .
       2| . H . . . . . . .
       1| C . C . C . C . C
       0| R . R . R . R . R 
    */
    
    freeXiangqiRecord(game1);
    
    // Game 2: Complex game with captures
    sXiangqiRecord *game2 = initXiangqiRecord();
    if (game2 == NULL) {
        printf("Failed to initialize Xiangqi game2\n");
        return -1;
    }
    
    printf("\nGame 2: Complex game with captures\n");
    // Move 1: Red chariot forward
    moveXiangqiRecord(game2, 0, 0, 2, 0);
    
    // Move 2: Black horse moves
    moveXiangqiRecord(game2, 9, 1, 7, 2);
    
    // Move 3: Red cannon to position for attack
    moveXiangqiRecord(game2, 2, 1, 7, 1);
    
    // Move 4: Black soldier advances
    moveXiangqiRecord(game2, 6, 4, 5, 4);
    
    // Move 5: Red cannon captures black cannon (demonstration of capture)
    moveXiangqiRecord(game2, 7, 1, 7, 7);
    
    // Move 6: Black chariot moves to prepare for attack
    moveXiangqiRecord(game2, 9, 8, 8, 8);
    
    // Move 7: Red soldier advances
    moveXiangqiRecord(game2, 3, 4, 4, 4);
    
    // Move 8: Black chariot captures red soldier
    moveXiangqiRecord(game2, 8, 8, 4, 8);
    
    // Move 9: Red horse moves to prepare attack
    moveXiangqiRecord(game2, 0, 1, 2, 2);
    
    // Move 10: Black elephant moves
    moveXiangqiRecord(game2, 9, 2, 7, 4);
    
    // Move 11: Red chariot captures black soldier
    moveXiangqiRecord(game2, 2, 0, 5, 0);
    
    // Move 12: Black horse captures red soldier
    moveXiangqiRecord(game2, 7, 2, 5, 3);
    
    printf("Game 2 End Board:\n");
    printXiangqiRecord(game2);
    /* Expected Board for Game 2:
       (A sample expected final board layout based on these moves; adjust as needed)
       9| . . e a g a e h r
       8| . . . . . . . . .
       7| . . . . . . . . .
       6| s . s . s . s . s
       5| . . . h . . . . .
       4| . . . . . . . . .
       3| . . . . . . . . .
       2| . . . . . . . . .
       1| C . C . C . C . C
       0| . . . . . . . . R
    */
    
    printf("\nStarting slide show of Game 2:\n");
    printXiangqiPlay(game2);
    
    freeXiangqiRecord(game2);
    
    return 0;
}