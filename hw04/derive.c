#include "derive.h"
int32_t  fDegree = 0, gDegree = 0, leftDegree = 0, rightDegree = 0, topArray = 0, bottomArray = 0;

int32_t* derivedF;
int32_t* derivedG;
int32_t* degreeStoredF;
int32_t* degreeStoredG;
int32_t* leftValues;
int32_t* rightValues;
int32_t* derivedDegreesF;
int32_t* derivedDegreesG;
int32_t* productEq;
int32_t* productDegrees;
int32_t* top;
int32_t* topDegree;
int32_t* bottom;
int32_t* bottomDegree;

void freed(){
    
    free(derivedF);
    free(derivedG);
    free(degreeStoredF);
    free(degreeStoredG);
    free(leftValues);
    free(rightValues);
    free(derivedDegreesF);
    free(derivedDegreesG);
    free(productEq);
    free(productDegrees);

}

void error(int16_t exitCode){
    
    printf("Error!\nReason: ");

    switch (exitCode){

    case 0:
    printf("Invalid degree\n");
    break;

    case 1:
    printf("First coefficient cannot be zero\n");
    break;

    case 2:
    printf("Invalid coefficient detected\n");
    break;

    case 3:
    printf("Division by zero detected");
    break;
    
    default:
    break;

    }
    exit(-1);
}

void display(int32_t degree[], int64_t Coefficients[]){

    for (size_t i = 0 ; i <= degree[0] ; i++){
        if (i >= 1){
            if (Coefficients[i] == 0){
                continue;
            }
            else if (Coefficients[i] > 0){
                printf("+ ");
            }
            else {
                printf("- ");
            }
        }
        if (Coefficients[i] == 1 && degree[i] != -1){
            printf("x");
        }
        else if (Coefficients[i] == -1 && degree[i] != -1){
            printf("-x");
        }
        else if (degree[i] > 0 && Coefficients[i] > 0){
            printf("%ldx", Coefficients[i]);
        }
        else if (degree[i] > 0 && Coefficients[i] < 0 && i == 0){
            printf("%ldx", Coefficients[i]);
        }
        else if (degree[i] > 0 && Coefficients[i] < 0){
            printf("%dx", abs(Coefficients[i]));
        }
        
        if(degree[i] == 0){
            printf("%d",abs(Coefficients[i]));
        }
        else if (degree[i] == 1){
            printf(" ");
        }
        else {
        printf("^%d ",degree[i]);
        }
    }
    printf("\n");
    freed();
}

void equation(){

    printf("Please enter f(x) degree: ");
     if(scanf("%d", &fDegree) != 1){
        error(0);
     }
      else if(fDegree < 0 || fDegree > UINT8_MAX){
        error(0);
     }

    fDegree += 1;
    
    int64_t fCoefficients[fDegree];
    printf("Please enter f(x) coefficients: ");
    for(size_t i = 0 ; i < fDegree ; i++){
        
        if (scanf("%ld", &fCoefficients[i]) != 1){
            error(2);
        }
        else if (fCoefficients[i] < INT32_MIN || fCoefficients[i] > INT32_MAX){
            error(2);
        }

        if (fCoefficients[0] == 0){
            error(1);
        }
        
    }

    printf("Please enter g(x) degree: ");
     if(scanf("%d", &gDegree) != 1){
        error(0);
     }
     if(gDegree < 0 || gDegree > UINT8_MAX){
        error(0);
     }

    gDegree += 1;
    
    int64_t gCoefficients[gDegree]; 
    printf("Please enter g(x) coefficients: ");
    for(int32_t i = 0 ; i < gDegree ; i++){
        if (scanf("%ld", &gCoefficients[i]) != 1){
            error(2);
        }
        else if (gCoefficients[i] < INT32_MIN || gCoefficients[i] > INT32_MAX){
            error(2);
        }

        if (gCoefficients[0] == 0){
            error(1);
        }
        
    }
    displayEq(fCoefficients, gCoefficients);
}

void displayEq(int64_t fCoefficients[], int64_t gCoefficients[]){
    printf("f(x): ");

    int32_t tempF[fDegree];
    for (size_t i = 0 ; i < fDegree ; i++){
        tempF[i] = (fDegree - 1) - i;
    }

    display(tempF, fCoefficients);

    printf("g(x): ");

    int32_t tempG[gDegree];
    for (size_t i = 0 ; i < gDegree ; i++){
        tempG[i] = (gDegree - 1) - i;
    }

    display(tempG, gCoefficients);

    printf("(f(x)g(x))': ");
    product(fCoefficients, gCoefficients, 0);

    quotient(fCoefficients, gCoefficients);
}

void product(int64_t fCoefficients[], int64_t gCoefficients[], size_t quotientOn){

leftDegree = (fDegree - 1) * gDegree;
rightDegree = fDegree * (gDegree - 1);
int32_t productSpace = leftDegree + rightDegree;

degreeStoredF = (int32_t*)malloc(fDegree * sizeof(int32_t));
degreeStoredG = (int32_t*)malloc(gDegree * sizeof(int32_t));
productDegrees = (int32_t*)malloc(productSpace * sizeof(int32_t));
leftValues = (int32_t*)malloc(leftDegree * sizeof(int32_t));
rightValues = (int32_t*)malloc(rightDegree * sizeof(int32_t));

int32_t k = 0;

for (size_t i = 0 ; i < fDegree ; ++i){
    if (fCoefficients[i] != 0){
        degreeStoredF[k] = (fDegree - 1) - i;
        ++k;
    }
}

k = 0;

for (size_t i = 0 ; i < gDegree ; ++i){
    if (gCoefficients[i] != 0){
        degreeStoredG[k] = (gDegree - 1) - i;
        ++k;
    }
}

int32_t m = 0;
k = 0;

derive(fDegree,fCoefficients, 1);
derive(gDegree, gCoefficients, 2);

for (size_t i = 0 ; i < fDegree - 1 ; ++i){
    if (derivedF[i] != 0){
        for (size_t j = 0 ; j < gDegree ; ++j){
            if (gCoefficients[j] != 0 && degreeStoredG[j] >= 0){
            leftValues[k] = derivedF[i] * gCoefficients[j];
            productDegrees[m] = derivedDegreesF[i] + degreeStoredG[j];
            ++k;
            ++m;
            }
        }
    }
}

k = 0;

for (size_t i = 0 ; i < gDegree - 1 ; ++i){
    if (derivedG[i] != 0){
        for (size_t j = 0 ; j < fDegree ; ++j){
            if (fCoefficients[j] != 0 && degreeStoredF[j] >= 0){
            rightValues[k] = derivedG[i] * fCoefficients[j];
            productDegrees[m] = derivedDegreesG[i] + degreeStoredF[j];
            ++k;
            ++m;
            }
        }
    }
}

if (quotientOn == 1){
for (size_t i = 0 ; i < k ; ++i){
    rightValues[i] *= -1;
    }
}

productEq = (int32_t*)malloc(productSpace * sizeof(int32_t));
k = 0;

for (size_t i = 0 ; i < leftDegree ; ++i){
    if (leftValues[i] != 0 && productDegrees[i] >= 0){
    productEq[k] = leftValues[i];
    ++k;
    }
}

for (size_t i = 0 ; i < rightDegree ; ++i){
    if (rightValues[i] != 0 && productDegrees[i] >= 0){
    productEq[k] = rightValues[i];
    ++k;
    }
}

int16_t negatives = 0;

for (size_t i = 0 ; i < productSpace - 1; ++i){
    for (size_t j = i + 1 ; j < productSpace ; ++j){
        if (productDegrees[i] == productDegrees[j]){
            productEq[i] += productEq[j];
            productEq[j] = 0;
            productDegrees[j] = -1;
        }
    }
}

for (size_t i = 0 ; i < productSpace - 1 ; ++i){
    for (size_t j = i + 1 ; j < productSpace ; ++j){
        if (productDegrees[i] < productDegrees[j]){
            int32_t temp = productDegrees[i];
            productDegrees[i] = productDegrees[j];
            productDegrees[j] = temp;

            int32_t temp2 = productEq[i];
            productEq[i] = productEq[j];
            productEq[j] = temp2;
        }
    }
}

for (size_t i = 0 ; i < productSpace ; ++i){
    if (productDegrees[i] == -1){
        ++negatives;
    }
}

int32_t productFinalDegree = (productDegrees[0]) + 1;
int32_t productSize = productSpace - negatives; 
int64_t products[productSize];
int32_t zeroCount = 0;

for (size_t i = 0 ; i < productFinalDegree ; ++i){
    products[i] = productEq[i];
    if (products[i] == 0){
        ++zeroCount;
    }
}

if (zeroCount == productFinalDegree){
    printf("0");
    freed();
    } 
    else if (quotientOn == 1){
    
    top = (int32_t*)malloc(productFinalDegree * sizeof(int32_t));
    topDegree = (int32_t*)malloc(productFinalDegree * sizeof(int32_t));
    topArray = productFinalDegree;

    for (size_t i = 0 ; i < productFinalDegree ; ++i){
    top[i] = productEq[i];
    topDegree[i] = productDegrees[i];
        }
    }
    else {
        display(productDegrees, products);
    }

}

void quotient(int64_t fCoefficients[], int64_t gCoefficients[]){

// [f'(x) * g(x) - g'(x) * f(x)] / [g(x)]^2
product(fCoefficients, gCoefficients, 1);

int32_t topDegrees[topArray];
int64_t topValue[topArray];

for (size_t i = 0 ; i < topArray ; ++i){
    topDegrees[i] = topDegree[i];
    topValue[i] = top[i];
}

square(gCoefficients);

printf(" f(x)   ");
display(topDegrees, topValue);
printf("(----)': \n");

free(top);
free(topDegree);
free(bottom);
free(bottomDegree);
}

void square(int64_t gCoefficients[]){

int32_t gSquared = pow(gDegree, 2);

bottom = (int32_t*)malloc(gSquared * sizeof(int32_t));
bottomDegree = (int32_t*)malloc(gSquared * sizeof(int32_t));

int32_t k = 0, m = 0;

for (size_t i = 0 ; i < gDegree ; ++i){
    if (gCoefficients[i] != 0){
        for (size_t j = 0 ; j < gDegree ; ++j){
            if (gCoefficients[j] != 0 && degreeStoredG[i] >= 0){
            bottom[k] = gCoefficients[i] * gCoefficients[j];
            bottomDegree[m] = degreeStoredG[i] + degreeStoredG[j];
            ++k;
            ++m;
            }
        }
    }
}

int16_t negatives = 0;

for (size_t i = 0 ; i < gSquared - 1; ++i){
    for (size_t j = i + 1 ; j < gSquared ; ++j){
        if (bottomDegree[i] == bottomDegree[j]){
            bottom[i] += bottom[j];
            bottom[j] = 0;
            bottomDegree[j] = -1;
        }
    }
}

for (size_t i = 0 ; i < gSquared - 1 ; ++i){
    for (size_t j = i + 1 ; j < gSquared ; ++j){
        if (bottomDegree[i] < bottomDegree[j]){
            int32_t temp = bottomDegree[i];
            bottomDegree[i] = bottomDegree[j];
            bottomDegree[j] = temp;

            int32_t temp2 = bottom[i];
            bottom[i] = bottom[j];
            bottom[j] = temp2;
        }
    }
}

for (size_t i = 0 ; i < gSquared ; ++i){
    if (bottom[i] == -1){
        ++negatives;
    }
}

int32_t bottomFinalDegree = (productDegrees[0]) + 1;
int32_t bottomTrue = gSquared - negatives; 
int32_t zeroCount = 0;

for (size_t i = 0 ; i < bottomFinalDegree ; ++i){
    if (bottom[i] == 0){
        ++zeroCount;
    }
}

if (zeroCount == bottomFinalDegree){
    freed();
    error(3);
} else {
    
    }

}

void derive(int32_t degree, int64_t Coefficients[], int8_t mode){

    if (mode == 1){
        derivedF = (int32_t*)malloc(degree * sizeof(int32_t));
        derivedDegreesF = (int32_t*)malloc(degree * sizeof(int32_t));

        int32_t k = 0;

        for(size_t i  = 0 ; i < degree ; ++i){
        derivedF[i] = Coefficients[i];
        derivedF[i] *= (degree - 1) - i;
            derivedDegreesF[k] = ((degree - 1) - i) - 1;
            ++k;
        }
        
    }
    else if (mode == 2) {
        derivedG = (int32_t*)malloc(degree * sizeof(int32_t));
        derivedDegreesG = (int32_t*)malloc(degree * sizeof(int32_t));

        int32_t k = 0;

        for(size_t i  = 0 ; i < degree ; ++i){
        derivedG[i] = Coefficients[i];
        derivedG[i] *= (degree - 1) - i;
            derivedDegreesG[k] = ((degree - 1) - i) - 1;
            ++k;
        }
        
    }

}