#include "mycontrol.h"

static int16_t initial = 0;
double x, y, a, length, x;
double xCurrent, yCurrent, aCurrent;

void initialize( double x, double y, double a){
    initial += 1;
    xCurrent = x;
    yCurrent = y;
    aCurrent = a;
}

int32_t forward( double length ){
    if (initial != 1){
        return -1;
    } else {
    xCurrent = (length * cos(aCurrent)) + xCurrent;
    yCurrent = (length * sin(aCurrent)) + yCurrent;
    }
}

int32_t clock_turn( double x ){
    if (initial != 1){
        return -1;
    } else {
    aCurrent = aCurrent - x;
    }
}

int32_t counterclock_turn( double x ){
    if (initial != 1){
        return -1;
    } else {
    aCurrent = aCurrent + x;
    }
}

int32_t print(){
    if (initial != 1){
        return -1;
    } else {
    printf("position: (%.2lf,%.2lf), angle: %.2lf\n", xCurrent, yCurrent, aCurrent / M_PI);
    }
}