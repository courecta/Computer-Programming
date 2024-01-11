#include <stdio.h>
#include <stdint.h>

uint8_t convert(uint8_t character);

int main(){

    uint8_t letter = 0;
    printf("Please enter a valid letter: ");
    scanf(" %c", &letter);

    if ( convert(letter) != 0){
        printf("%c\n", convert(letter));
    }
    else {
    printf("Error! Invalid alphabet character.\n");
    }

    return 0;

}

uint8_t convert(uint8_t character){

    switch (character){

        case 65 ... 90:
            character += 32;
        break;
        case 97 ... 122:
            character -= 32;
        break;
        default:
            return 0;
        break;

    }

    return character;

}