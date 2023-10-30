#include <stdio.h>
#include <stdint.h>

/*Steps on how to convert binary to 2's compliment

1. Invert the 0's and 1's
2. add a 1 at the end

*/

int32_t number;

void initialize();
int32_t BinaryForm (int32_t number);
int32_t twosCompliment (int32_t number);
int32_t printer (int32_t count);

int main(){

    message();

    return 0;
}

void message(){
    printf("Please enter the number: ");
    scanf("%d", &number);
    BinaryForm();
}

int32_t BinaryForm (int32_t number){

    int32_t binaryNumber = number % 2    

    return binaryNumber;   
}

int32_t twosCompliment (int32_t number){

}