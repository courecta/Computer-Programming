#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>

uint32_t number = 0;
int16_t bits = 32;
int16_t space = 0, space2 = 0;

void BinaryForm (uint32_t number, int32_t n);
void positive (int32_t n);

int main(){
    printf("Please enter the number: ");
    scanf("%d", &number);
    int32_t n = number;
    printf("%d\n", n);
    BinaryForm(number, n);
    if (n > 0){
        printf("\r");
        positive(n);
    }
    printf("\n");
    return 0;
}

void BinaryForm(uint32_t number, int32_t n){
    if (number == 0){
        printf("00000000 00000000 00000000 00000000\n");
        exit(0);
    }
    else if (n >= INT_MAX){
        printf("Error! Please enter a valid 32-bit signed integer\n");
        exit(1);
    }

    if (number > 1) {
        BinaryForm(number / 2, n);
        space += 1;
        bits -= 1;
        if (space % 8 == 0){
            printf(" ");
            }
        }
    printf("%d", number % 2);
    }

void positive(int32_t n){
    if (bits == 1){
        space = space2;
        BinaryForm(number, n);
        return;
    }
    printf("0");
    bits -= 1;
    space2 += 1;
    if (space2 % 8 == 0){
    printf(" ");
    }
    positive(n);
}