#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void init();
void byteCount(int32_t num);
int32_t num = 0;

int main (){
    void (*func) (int32_t);
    func = init;
    func(num);
    return 0;
}

void init(){
    printf("enter a number: ");
    scanf("%d", &num);
    byteCount(num);
}

void byteCount(int32_t num){

uint8_t *ptr =  (uint8_t *) &num;

    for(int32_t i = 0 ; i < 4 ; i++){
        printf("Byte at %p is %02X\n", ptr + i, *(ptr + i));
    }
    printf("----------\n");
    if (num == 0){
        return;
    }
    init();
}