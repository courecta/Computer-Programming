#include <stdio.h>
#include <string.h>

int main(){

    int z, i, m, l, iteration, cases = 1;
    int visited_node[10000] = {0};

    while(1)
    {
        scanf("%d %d %d %d", &z, &i, &m, &l);
        if(z == 0 && i == 0 && m == 0 && l == 0) break;

        iteration = 0;
        while(visited_node[l] == 0)
        {
            ++iteration;
            visited_node[l] = iteration;
            l = (z * l + i) % m;
        }

        printf("Case %d: %d\n", cases, iteration - visited_node[l] + 1);
        memset(visited_node, 0, sizeof(visited_node));
        cases++;
    }

    return 0;
}