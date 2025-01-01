#include <stdio.h>

int main(void){
    int h = 0, w = 0, d = 0;
    scanf("%d%d%d", &h, &w, &d);
    int sa = 0, v = 0;
    sa = 2 * ((d * w) + (d * h) + (w * h));
    v = h * w * d;
    printf("%d %d\n", sa, v);

    return 0;
}