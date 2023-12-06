#include "derive.h"
uint8_t  fDegree, gDegree;
//int32_t degree;
//int32_t coefficients[degree];

void error(){
    printf("Error!\n");
    exit(1);
}

void equation(){

    printf("Please enter f(x) degree: ");
    scanf("%hhd", &fDegree);
    fDegree += 1;
    int32_t fCoefficients[fDegree];
    printf("Please enter f(x) coefficients: ");
    for(int32_t i = 0 ; i < fDegree ; i++){
        scanf("%d", &fCoefficients[i]);
    }                                                                                                                                                       

    if (fCoefficients[0] == 0){
        error();
    }

    printf("Please enter g(x) degree: ");
    scanf("%hhd", &gDegree);
    gDegree += 1;
    int32_t gCoefficients[gDegree]; 
    printf("Please enter g(x) coefficients: ");
    for(int32_t i = 0 ; i < gDegree ; i++){
        scanf("%d", &gCoefficients[i]);
    }

    if (gCoefficients[0] == 0){
        error();
    }

    displayEq(fCoefficients, gCoefficients);

}

void displayEq(int32_t fCoefficients[], int32_t gCoefficients[]){
    displayf(fCoefficients);
    displayg(gCoefficients);
    product(fCoefficients, gCoefficients);
    quotient(fCoefficients, gCoefficients);
}

void displayf(int32_t fCoefficients[]){

printf("f(x): ");
    for (int32_t i = 0, j = fDegree - 1 ; i < fDegree, j >= 0 ; i++, j--){
        if (i >= 1){
            if (fCoefficients[i] == 0){
                continue;
            }
            else if (fCoefficients[i] > 0){
                printf("+ ");
            }
            else {
                printf("- ");
            }
        }
        if (fCoefficients[i] == 1 && j != 0){
            printf("x");
        }
        else if (fCoefficients[i] == -1 && j != 0){
            printf("-x");
        }
        else if (j != 0){
            printf("%dx", fCoefficients[i]);
        }
        
        if(j == 0){
            printf("%d",abs(fCoefficients[i]));
        }
        else{
        printf("^%d ",j);
        }
    }
printf("\n");
}

void displayg(int32_t gCoefficients[]){

printf("g(x): ");
    for (int32_t i = 0, j = gDegree - 1 ; i < gDegree, j >= 0 ; i++, j--){
        if (i >= 1){
            if (gCoefficients[i] == 0){
                continue;
            }
            else if (gCoefficients[i] > 0){
                printf("+ ");
            }
            else {
                printf("- ");
            }
        }
        if (gCoefficients[i] == 1 && j != 0){
            printf("x");
        }
        else if (gCoefficients[i] == -1 && j != 0){
            printf("-x");
        }
        else if (j != 0){
            printf("%dx", gCoefficients[i]);
        }
        
        if(j == 0){
            printf("%d",abs(gCoefficients[i]));
        }
        else{
        printf("^%d ",j);
        }
    }
printf("\n");

}

void product(int32_t fCoefficients[], int32_t gCoefficients[]){

printf("(f(x)g(x))': ");


}

void quotient(int32_t fCoefficients[], int32_t gCoefficients[]){

}

void derive(int32_t degree, int32_t coefficients[]){



}