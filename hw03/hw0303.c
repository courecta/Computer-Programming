#include <stdio.h>
#include <stdint.h>
#include <math.h>

int32_t number;

int32_t BinaryForm (int32_t number);
int32_t twosCompliment (int32_t number);
int32_t Counter (int32_t count);

int main(){

    printf("Please enter the number: ");
    scanf("%d", &number);
    printf("binary = %d, two's compliment = %d, and count is %d\n", BinaryForm(number), Counter(count));

    if (number > 0){
        for (int32_t i = 0 ; i < Counter(count) ; i++){
            if (i % 8 == 0){
                printf(" ");
                continue;
            }
            printf("0");
        }
    }
    else if (number < 0){
        for (int32_t i = 0 ; i < Counter(count) ; i++){
            if (i % 8 == 0){
                printf(" ");
                continue;
            }
            printf("1");
        }
    }

    return 0;
}

int32_t BinaryForm (int32_t number){
    int32_t binaryNumber = 0;
    for (int32_t i = 0; number != 0; i++){
        int32_t remainderCount = number % 2;
        int32_t powerTo32 = pow(10, i);
        
        binaryNumber = binaryNumber + (remainderCount * powerTo32);
        number /= 2;
    }
    return binaryNumber;

}

int32_t twosCompliment (int32_t number){
    BinaryForm(binaryNumber);

}

int32_t Counter (int32_t count){
    if (number > 0){
    BinaryForm (binaryNumber);
    }
    else if (number < 0){
        
    }

}