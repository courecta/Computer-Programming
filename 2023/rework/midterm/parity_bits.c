#include <stdio.h>
#include <stdbool.h>

unsigned long long parity_2d(int n1, int n2, int n3, int n4, int n5);

int main(){

    int n1, n2, n3, n4, n5;
    scanf("%d%d%d%d%d", &n1, &n2, &n3, &n4, &n5);
    printf("%llu\n", parity_2d(n1, n2, n3, n4, n5));

    return 0;
}

unsigned long long parity_2d(int n1, int n2, int n3, int n4, int n5)
{
    bool parity_table[6][33] = {0};

    int num_list[6] = {n1, n2, n3, n4, n5};

    for(int i = 0 ; i < 6 ; i++)
    {
        for(int j = 31 ; j >= 0; j--) parity_table[i][31 - j] = (num_list[i] & (1 << j)) ? 1 : 0;
    }

    for(int i = 0; i < 5 ; i++)
    {
        int count = 0;
        for(int j = 0 ; j < 32 ; j++) count = parity_table[i][j] ? count + 1 : count;
        parity_table[i][32] = (count % 2) ? 0 : 1;
    }

    for(int i = 0 ; i < 33 ; i++)
    {
        int count = 0;
        for(int j = 0 ; j < 5 ; j++) count = parity_table[j][i] ? count + 1 : count;
        parity_table[5][i] = (count % 2) ? 0 : 1;
    }

    unsigned long long parity_num = 0;
    for(int i = 0 ; i < 33 ; i++) parity_num |= ((unsigned long long)(parity_table[5][i]) << (32 - i));

    return parity_num;
}