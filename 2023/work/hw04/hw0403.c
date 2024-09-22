#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include "gsat.h"

void display();
void scores(int32_t score[][6], int32_t percentage);
void sort(int32_t array[]);
int32_t top, percentage, count = 1;
int32_t array[STUDENT_NUMBER];

int main (){

    display(0);

    return 0;
}

void display(int32_t top){

switch (top) {
    
    case 1:
        printf("TOP 12%%");
        scores(score, 12);
        break;
    case 2:
        printf("TOP 25%%");
        scores(score, 25);
        break;
    case 3:
        printf("TOP 50%%");
        scores(score, 50);
        break;
    case 4:
        printf("TOP 75%%");
        scores(score, 75);
        break;
    case 5:
        printf("TOP 88%%");
        scores(score, 88);
        break;
    case 6:
        exit(0);
        break;
    default:
        printf("\t CHINESE | ENGLISH | MATH_A  | MATH_B  | SOCIAL  | SCIENCE\n");
        display(1);
        break;
    }
    return;
}

void scores(int32_t score[][6], int32_t percentage){

    for(int32_t i = 0 ; i < 6 ; i++){
        for(int32_t j = 0 ; j < STUDENT_NUMBER ; j++){
            array[j] = score[j][i];
        }
        sort(array);
        double sortedValue = (double) STUDENT_NUMBER * percentage/100;
        int32_t value = ceil(sortedValue);
        if(i == 0){
        printf("%7d", array[value]);
        }
        else {
        printf("%10d", array[value]);
        }
        if(i == 5){
            printf("\n");
        }
        for(int32_t k = 0; k < STUDENT_NUMBER; k++){
            array[k] = 0;
            }
    }
    ++count;
    display(count);

}

void sort(int32_t array[]){

    if ( STUDENT_NUMBER < 2){
        return;
    }

    for(int32_t i = 0 ; i < STUDENT_NUMBER ; i++){
        for(int32_t j = i + 1 ; j < STUDENT_NUMBER ; j++){
            if (array[i] < array[j]){
                int32_t temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }    
    }
    return;
}