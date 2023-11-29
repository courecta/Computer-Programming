#include "derive.h"

uint32_t  fDegree, gDegree;

void equation(){

    printf("Please enter f(x) degree: ");
    scanf("%d", &fDegree);
    fDegree += 1;
    int32_t fCoefficients[fDegree];
    printf("Please enter f(x) coefficients: ");
    for(int32_t i = 0 ; i < fDegree ; i++){
        scanf("%d", &fCoefficients[i]);
    }                                                                                                                                                       

    printf("Please enter g(x) degree: ");
    scanf("%d", &gDegree);
    gDegree += 1;
    int32_t gCoefficients[gDegree]; 
    printf("Please enter g(x) coefficients: ");
    for(int32_t i = 0 ; i < gDegree ; i++){
        scanf("%d", &gCoefficients[i]);
    }

    displayEq(fCoefficients, gCoefficients);

}

void displayEq(int32_t fCoefficients[], int32_t gCoefficients[]){

    printf("(f(x)g(x))': ");
    printf("");

void product(){



}

void quotient(){

}