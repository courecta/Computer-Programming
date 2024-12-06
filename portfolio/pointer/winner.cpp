#include <stdio.h>

void winner_takes_double(int* a, int* b, int* c);
void winner_takes_all(int* a, int* b, int* c);
int* max(int* n);

template<typename ... Args>
int* max(int* n, Args... args);

int main(){

    int choice(0), a(0), b(0), c(0);
    printf("welcome to the three-way winner takes...\n1. All\n2. Double\nWell, which would it be? >> ");
    scanf("%d", &choice);
    choice -= 1;
    printf("Now, enter your three numbers >> ");
    scanf("%d%d%d", &a, &b, &c);
    (choice) ? winner_takes_double(&a, &b, &c) : winner_takes_all(&a, &b, &c);
    printf("winner is\n1 = %d\n2 = %d\n3 = %d\n", a, b, c);
    return 0;
}

void winner_takes_double(int* a, int* b, int* c)
{
    int* winner = max(a, b, c);
    *winner *= 2;
}

void winner_takes_all(int* a, int* b, int* c)
{
    int sum = *a + *b + *c;

    int* winner = nullptr;
    winner = max(a, b, c);
    *a = *b = *c = 0;
    *winner = sum;

}

template<typename ... Args>
int* max(int* n, Args... args)
{
    int* max_rest = max(args...);
    return (*n > *max_rest) ? n : max_rest;
}

int* max(int* n)
{
    return n;
}