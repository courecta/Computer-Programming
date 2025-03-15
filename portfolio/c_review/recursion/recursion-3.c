#include <stdio.h>

// recurrence relation: fun(n) = 1 + Σ (from k = 1 to n‑1) [fun(k) * fun(n‑k)], for n > 1
// fun(1) = 1
// fun(5) = 51

int fun(int n);

int fun(int n)
{
    int x = 1, k;
    if(n == 1)
        return x;
    for(k = 1; k < n; ++k)
        x = x + fun(k) * fun(n - k);
    return x;
}

int main(void){
    int n = 5;
    printf("fun(%d) = %d\n", n, fun(n));
}