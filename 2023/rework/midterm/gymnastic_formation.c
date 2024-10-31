#include <stdio.h>

static long long int girl_weight = -1;
static long long int boy_weight = -1;

void setup_girl_weight(unsigned int kg);
void setup_boy_weight(unsigned int kg);
long long int afford_weight(int x, int y);
double calculate_weight(int x, int y);

int main(){

    unsigned int girl = 0, boy = 0;
    int x = 0, y = 0;
    scanf("%d%d", &girl, &boy);
    setup_girl_weight(girl);
    setup_boy_weight(boy);

    scanf("%d%d", &x, &y);

    printf("%lld\n", afford_weight(x, y));

    return 0;
}

void setup_girl_weight(unsigned int kg)
{
    girl_weight = kg;
    return;
}

void setup_boy_weight(unsigned int kg)
{
    boy_weight = kg;
    return;
}

long long int afford_weight(int x, int y)
{
    if((girl_weight == -1 || boy_weight == -1) || (x < 0 || y < 0) || (y > x)) return 0;

    long long int self_weight = x % 2 ? boy_weight : girl_weight;
    return calculate_weight(x, y) - self_weight;
}

double calculate_weight(int x, int y)
{
    long long int self_weight = x % 2 ? boy_weight : girl_weight;

    if(x == 0 && y == 0) return self_weight;

    if(y == 0) return self_weight + calculate_weight(x - 1, 0) / 2.0;

    if(y == x) return self_weight + calculate_weight(x - 1, x - 1) / 2.0;

    return self_weight + calculate_weight(x - 1, y - 1) / 2.0 + calculate_weight(x - 1, y) / 2.0;
}