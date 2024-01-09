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
    if (A < 0 || A > 10 || X < 2 || X > 100){
        printf("NOOOOOO u kant du that :(\n");
        actionTime();
    }
    roller (A, 0, X, 0, 0, 0, 0);

}

void rollAdXkYplusB(){

    printf("Please enter A, X, Y, B: ");
    scanf("%d %d %d %d", &A, &X, &Y, &B);
    if (A < 0 || A > 10 || X < 2 || X > 100 || Y < 0 || Y > 10 || B < -10 || B > 10){
        printf("NOOOOOO u kant du that :(\n");
        actionTime();
    }
    roller(A, B, X, Y, 0, 0, 0);

}

void rollCustom(){

    printf("Please enter A, X, H, L, C, B: ");
    scanf("%d %d %d %d %d %d", &A, &X, &H, &L, &C, &B);
    if (A < 0 || A > 10 || X < 2 || X > 100 || H < 0 || H > 10 || L < 0 || L > 10 || C < 0 || C > 10 || B < -10 || B > 10){
        printf("NOOOOOO u kant du that :(\n");
        actionTime();
    }
    roller(A, B, X, 0, H, L, C);

}

void roller( int32_t A, int32_t B, int32_t X, int32_t Y, int32_t H, int32_t L, int32_t C ){
srand( time(0) );

int32_t array[A];
for (int32_t i = 0 ; i < A ; i++){
    array[i] = 1 + rand() % X;
}

printf("\n");

    for (int32_t i = 1 ; i <= A && i <= 5 ; i++){
    printf("%d.\t\t", i);
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A && i <= 5 ; i++){
    printf(" ╱|、\t\t");
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A && i <= 5 ; i++){
    printf("(˚ˎ 。7\t\t");
    }
    printf("\n");
    for (int32_t i = 0 ; i < A && i < 5 ; i++){
    printf("|、"n" ˜〵\t", array[i]);
    }
    printf("\n");
    for (int32_t i = 1 ; i <= A && i <= 5 ; i++){         
    printf("じ しˍ,)ノ\t");
    }
    printf("\n");

    if (A > 5){
    printf("\n");
    for (int32_t i = 6 ; i <= A ; i++){
    printf("%d.\t\t", i);
    }
    printf("\n");
    for (int32_t i = 6 ; i <= A ; i++){
    printf(" ╱|、\t\t");
    }
    printf("\n");
    for (int32_t i = 6 ; i <= A ; i++){
    printf("(˚ˎ 。7\t\t");
    }
    printf("\n");
    for (int32_t i = 5 ; i < A ; i++){
    printf("|、"n" ˜〵\t", array[i]);
    }
    printf("\n");
    for (int32_t i = 6 ; i <= A ; i++){         
    printf("じ しˍ,)ノ\t");
    }
    printf("\n");
    }

int32_t arrayY[Y];

    if( Y != 0){
        int32_t chooseY = 0;
        printf("Please choose %d dice from above: ", Y);
        for (int32_t i = 0 ; i < Y ; i++){
        scanf("%d", &chooseY);
        arrayY[i] += array[chooseY]; 
        }
        printf("\n");

    for (int32_t i = 1 ; i <= Y && i <= 5 ; i++){
    printf(" ╱|、\t\t");
    }
    printf("\n");
    for (int32_t i = 1 ; i <= Y && i <= 5 ; i++){
    printf("(˚ˎ 。7\t\t");
    }
    printf("\n");
    for (int32_t i = 0 ; i < Y && i < 5 ; i++){
    printf("|、"n" ˜〵\t", array[Y]);
    }
    printf("\n");
    for (int32_t i = 1 ; i <= Y && i <= 5 ; i++){         
    printf("じ しˍ,)ノ\t");
    }
    printf("\n");

if (Y > 5){
    printf("\n");
    for (int32_t i = 6 ; i <= Y ; i++){
    printf("%d.\t\t", i);
    }
    printf("\n");
    for (int32_t i = 6 ; i <= Y ; i++){
    printf(" ╱|、\t\t");
    }
    printf("\n");
    for (int32_t i = 6 ; i <= Y ; i++){
    printf("(˚ˎ 。7\t\t");
    }
    printf("\n");
    for (int32_t i = 5 ; i < Y ; i++){
    printf("|、"n" ˜〵\t", array[i]);
    }
    printf("\n");
    for (int32_t i = 6 ; i <= Y ; i++){         
    printf("じ しˍ,)ノ\t");
    }
    printf("\n");
    }

}

    int32_t highest = array[0];
    int32_t lowest = array[0];

    for (int32_t i = 0; i < A; i++) {
        if (array[i] > highest) {
            highest = array[i];
        }
        if (array[i] < lowest) {
            lowest = array[i];
        }
    }

printf("result : ");
int32_t result = 0;
if (Y != 0){
    for (int32_t i = 0; i < Y; i++) {
        if (arrayY[i] == 0){
            continue;
        }
        printf("%d + ", arrayY[i]);
} 
} else {
for (int32_t i = 0; i < A; i++) {
        if (array[i] == 0){
            continue;
        }
        printf("%d + ", array[i]);
}
}

if (B != 0){
    printf("\b\b+ %d = ", B);
} else {
    printf("\b\b= ");
}

if (Y != 0){
    for (int32_t i = 0; i < Y; i++) {
        result += arrayY[i];
}
} else {
for (int32_t i = 0; i < A; i++) {
        result += array[i];
    }
}
printf("%d\n", result + B);
actionTime();
}

void feature(){
    int32_t j = 0, i = 0;
    sleep(1);
    printf("Sorry, I don't have a featured prepared  :(((\n");
    sleep(2);
    while(i < 3){
        printf(".\n");
        sleep(1);
        i++;
    }
    sleep(3);
    printf("but");
    sleep(1);
    while(i < 6){
        printf(".");
        sleep(1);
        i++;
    }
    printf("\n");
    sleep(4);
    printf("I can");
    sleep(1);
    while(i < 9){
        printf(".");
        sleep(1);
        i++;
    }
    printf("\n");
    sleep(5);
    printf("MAKE YOUR COMPUTER EXPLODE!!!\n");
    sleep(1);
for (int32_t i = 0 ; i <= 690 ; i++){
    printf(" ╱|、\n"
           "(˚ˎ 。7 \n" 
           "|、 ˜〵 \n"         
           "じ しˍ,)ノ \n");
    printf("\n");
    usleep(500);
    }
printf("hehe just kidding ♡\n");
exit(0);
}