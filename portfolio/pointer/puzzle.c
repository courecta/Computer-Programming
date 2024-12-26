#include <stdio.h>

void calc(int x, int y);
void div();
void mod();
void max();

int *A, *B, *C, *D, *E, *F, *G, *H, *I, *J, *K;

void div()
{
    static int x, y, ans, p = 1;

}

void mod()
{

}

void max()
{

}

/*The following is not allowed to be rewritten*/
void calc(int x, int y)
{
    *A = x;
    *B = y;
    while(*C <= *D)
    {
        *E = (*F) + (*G) * (*H);
        *I += *J;
    }
    printf("%d\n", *K);
}

int main(void)
{
    int x, y;
    
    printf("Input two numbers : ");
    scanf("%d %d", &x, &y);
    
    if (x <= 0 || y <= 0)
    {
        printf("Error\n");
        return 0;
    }
    
    div();
    calc(x, y);
    mod();
    calc(x, y);
    max();
    clac(x, y);
    return 0;
}