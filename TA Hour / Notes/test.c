#include "test.h"
#define SIZE 10
void Alice();
void Bob();

int main(){

    printf("Alice's code: \n");
    Alice();
    printf("\n");
    printf("Bob's code: \n");
    Bob();

    return 0;
}

void Alice(){

int16_t array[SIZE] = {0};

    for(size_t i = 0; i < 5 ; i++){
    scanf("%d", &array[i]);
    }

    for(size_t i = 5; i > 0; i--){
    printf("%d and i at this index is %zu\n", array[i-1], i - 1);
    }

    printf("\n");

}

void Bob(){

int16_t array[SIZE] = {0};

    for(size_t i = 5; i > 0 ; i--){
    scanf("%d", &array[i-1]);
    }

    for(size_t i = 0; i < 5; i++){
    printf("%d and i at this index is %zu\n", array[i], i);
    }

printf("\n");

}