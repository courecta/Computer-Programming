#include <stdio.h>

// handlers
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);

// function pointer array
int (*operations[3])(int, int) = {add, subtract, multiply};

int main(void){
    int choice, result, a, b;
    
    while(1)
    {
        printf("Pick a choice: ");
        scanf("%d", &choice);
        if(choice == -1) break;
        printf("choose 2 numbers: ");
        scanf("%d%d", &a, &b);
        result = operations[choice](a, b);
        printf("%d\n", result);
    }

}

int add(int a, int b)
{
    return a + b;
}

int subtract(int a, int b)
{
    return a - b;
}

int multiply(int a, int b)
{
    return a * b;
}