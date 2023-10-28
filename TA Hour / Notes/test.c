#include <stdio.h>
#include <stdint.h>

int32_t bits = 32;

void BinaryForm(int32_t number, int32_t bits);

int main() {
    int32_t number;  // Example negative number
    printf("Please enter the number: ");
    scanf("%d", &number);

    if (number > 0)

    BinaryForm(number, bits);
    printf("\n");

    return 0;
}

void BinaryForm(int32_t number, int32_t bits) {
    
    if (number == 0) {
    return;
    }

    if (number > 0){

    }

    BinaryForm(number / 2, bits - 1);
    int32_t bit = (number % 2);
    printf("%d", bit);

}