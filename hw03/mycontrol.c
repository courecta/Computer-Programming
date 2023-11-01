#include "mycontrol.h"

static int16_t initial = 0;
double x, y, a, length, x;

void initialize( double x, double y, double a){
    initial += 1;
}

int32_t forward( double length ){
    if (initial != 1){
        return -1;
    } else {
    x = length * cos(a);
    y = length * sin(a);
    }
}

int32_t clock_turn( double x ){
    if (initial != 1){
        return -1;
    } else {
    a = a - x;
    }
}

int32_t counterclock_turn( double x ){
    if (initial != 1){
        return -1;
    } else {
    a = a + x;
    }
}

int32_t print(){
    if (initial != 1){
        return -1;
    } else {
    printf("position: (%.2lf,%.2lf), angle: %.2lf\n", x, y, a / M_PI);
    initial -= 1;
    }
}