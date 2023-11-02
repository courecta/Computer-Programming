#include <stdio.h>
#include <stdint.h>

uint32_t number = 0;
int32_t n = 0;
int16_t bits = 32;
int16_t space = 0, space2 = 0;

void BinaryForm ();
void positive ();

int main(){
    printf("Please enter the number: ");
    scanf("%d", &number);
    int32_t n = number;
    printf("%u\n", number);
    if (n > 2147483648){
        n = n - 2147483648;
    }
    BinaryForm(number, n);
    if (n > 0){
        printf("\r");
        positive();
    }
    printf("\n");
    return 0;
}

void BinaryForm(uint32_t number){
    if (number == 0){
        printf("00000000 00000000 00000000 00000000");
        return;
    }
    else if (number >= 2147483648 || number < -2147483648){
        printf("Error! Please enter a valid 32-bit signed integer");
        return;
    }
    if (number > 1) {
        BinaryForm(number / 2);
        space += 1;
        bits -= 1;
        if (space % 8 == 0){
            printf(" ");
            }
        }
    printf("%d", number % 2);
    }

void positive(){
    if (bits == 1){
        space = space2;
        BinaryForm(number);
        return;
    }
    printf("0");
    bits -= 1;
    space2 += 1;
    if (space2 % 8 == 0){
    printf(" ");
    }
    positive();
}