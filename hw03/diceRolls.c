#include "diceRolls.h"

int32_t A = 0, B = 0, X = 0, Y = 0, H = 0, L = 0, C = 0, nL = 0, choice;

void welcome(){
    printf("Hiiii!!! Welcom tu de dicey kitty cafe!!!\n"
    "uwu so smrt so u kno how tu pley dis gam rite???\n"
    "me dint fink ull nid tu reed da README den hehehe...\n"
    "0. bleh ^ x ^ (*dies*)\n"
    "1. roll a die!\n"
    "2. roll som dice wit X sidessss!!\n"
    "3. roll som dice wit X sidesz and keep Y (and add a lil B ^0^)!!!\n"
    "4. roll AdXkhHkILkcC+B!!!!!!!!!!!!!!!!!!\n"
    "5. Awsum fiturr ^ 3 ^\n");
    actionTime();
}

void actionTime(){
    printf("----------------------------------------\n"
    "ur akshion? ");
    scanf("%d", &choice);
    choices(choice);
}

void choices( int32_t choice){
    
    switch (choice){

        case 0:
            getOut();
            break;
        case 1:
            roll1d6();
            break;
        case 2:
            rollAdX();
            break;
        case 3:
            rollAdXkYplusB();
            break;
        case 4:
            rollCustom();
            break;
        case 5:
            feature();
            break;
        default:
            printf("NOOOO u cant do dat :((((( pls chos 0 to 5!!!!\n");
            printf("\n");
            actionTime();
            break;

    }
}

void getOut(){
    printf("bai bai see u nex taim!!!!!!\n");
    printf("\n"
"        へ     ♡     ╱|、\n"
"     ૮ - ՛)         (` - 7\n"
"     / ⁻  ៸|        |、⁻〵\n"
" 乀 (ˍ, ل ل         じしˍ,)ノ\n");

exit(0);

}

void roll1d6(){
    srand( time(0) );
    int32_t number1d6 = 1 + rand() % 6;
    printf("\n");
    printf(" ╱|、\n"
           "(˚ˎ 。7 \n" 
           "|、"n" ˜〵 \n"         
           "じ しˍ,)ノ \n", number1d6);
    printf("ur result iz : %d\n", number1d6);
    printf("\n");
    actionTime();
}

void rollAdX(){
    
    printf("Please enter A, X: ");
    scanf("%d %d", &A, &X);
    roller (A, 0, X, 0, 0, 0, 0);

}

void rollAdXkYplusB(){

    printf("Please enter A, X, Y, B: ");
    scanf("%d %d %d %d", &A, &X, &Y, &B);
    roller(A, B, X, Y, 0, 0, 0);

}

void rollCustom(){

    printf("Please enter A, X, H, L, C, B: ");
    scanf("%d %d %d %d %d %d", &A, &X, &H, &L, &C, &B);
    roller(A, B, X, 0, H, L, C);

}

void roller( int32_t A, int32_t B, int32_t X, int32_t Y, int32_t H, int32_t L, int32_t C ){
srand( time(0) );

    for (int32_t i = 1 ; i <= A ; i++){
    printf(" ╱|、\t\t");
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A ; i++){
    printf("(˚ˎ 。7\t\t");
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A ; i++){
    printf("|、"n" ˜〵\t", 1 + rand() % X);
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A ; i++){         
    printf("じ しˍ,)ノ\t");
    }

}

void feature(){
    
}