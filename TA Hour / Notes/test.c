#include "test.h"

int32_t puzzle[9][9] = { { 4, 1, 0, 0, 0, 0, 0, 0, 0 },

                         { 0, 0, 9, 0, 0, 0, 0, 0, 0 },

                         { 6, 5, 2, 0, 0, 1, 0, 4, 0 },

                         { 0, 0, 4, 0, 0, 8, 3, 0, 2 },

                         { 0, 6, 8, 0, 4, 0, 1, 5, 0 },

                         { 2, 0, 5, 3, 0, 0, 7, 0, 0 },

                         { 0, 4, 0, 5, 0, 0, 2, 1, 6 },

                         { 0, 0, 0, 0, 0, 0, 8, 0, 0 },

                         { 0, 0, 0, 0, 0, 0, 0, 7, 3 } }; 

int32_t is_safe( int32_t x, int32_t y, int32_t num ){
    for( int32_t i = 0 ; i < 9; i++ ){
    if( puzzle[i][y] == num ){
    return 0;
        }
    }
    for( int32_t i = 0 ; i < 9; i++ ){
    if( puzzle[x][i] == num ){
    return 0;
        }
    }
    return 1;
}

int main(){
    
    
    
    return 0;
}