#include "mycontrol.h"

int main(){
    initialize(0, 0, 0);
    int a = forward(3);
    int b = clock_turn(M_PI);
    int c = counterclock_turn(-M_PI);
    int d = print();
    
    printf("%d %d %d %d", a, b, c, d);

    return 0;
}