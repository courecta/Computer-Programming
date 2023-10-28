#include <stdio.h>
#include <stdint.h>

int main(){

int32_t digit;
int16_t oneDetected, thirtytwoDetected, sixtyFiveDetected, nineDetected, seventyeightDetected;
int16_t sevenDetected, zeroDetected, fiveDetected, threeDetected;
int16_t fourDetected, sixDetected, eightDetected;
int32_t ruleOne = 0, ruleTwo = 0, ruleThree = 0;

    while (digit != -1){

        printf("Please input the digit: ");
        scanf("%d", &digit);

        if (digit == 1){
            oneDetected += 1;
            ruleOne += 1;
        }
        else if (digit == 3){
            thirtytwoDetected += 1;
        }
        if (thirtytwoDetected == 1 && digit == 2){
            thirtytwoDetected += 1;
            ruleOne += 1;
        }
        else if (thirtytwoDetected == 1 && digit != 2){
            thirtytwoDetected -= 1;
        }
        if (oneDetected == 1 && digit == 6){
            sixtyFiveDetected += 1;
        }
        else if (oneDetected == 1 && digit != 6){
            oneDetected -= 1;
        }

        if (sixtyFiveDetected == 1 && digit == 5){
            sixtyFiveDetected += 1;
            ruleOne += 1;
        }
        else if (sixtyFiveDetected == 1 && digit != 5){
            sixtyFiveDetected -= 1;
            ruleOne -= 1;
        }

        if (ruleOne == 2 && digit == 9){
            nineDetected += 1;
            ruleOne += 1;
        }
        else if (ruleOne == 2 && digit == 7){
            seventyeightDetected += 1;
        }
        else (digit != 9 && digit != 7){
            ruleOne == 0;
        }

        if (seventyeightDetected == 1 && digit == 8){
            seventyeightDetected += 1;
            ruleOne += 1;
        }



    }

    if (digit == -1){
        
        printf("SUCCESS!\n");
    }

    return 0;
}