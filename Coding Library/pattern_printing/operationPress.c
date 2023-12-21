#include <stdio.h>
#include <stdint.h>

void printBits(uint8_t num);

int main()
{
    uint8_t a = 223;
    uint8_t b = 172;
    uint8_t c = a &= ~b;

    printBits(c);

    return 0;
}

void printBits(uint8_t num){
    uint8_t size = sizeof(uint8_t);
    uint8_t maxPow = 1<<(size*8-1);
    printf("MAX POW : %u\n",maxPow);
    size_t i = 0;
    for(;i<size*8;++i)
    {
    // print last bit and shift left.
    printf("%u ",num&maxPow ? 1 : 0);
    num = num<<1;
    }
}