#include <stdio.h>

void fill(int x, int y, int array[][5]);
void add(int x, int y, int a[][5], int b[][5], int c[][5]);
void multiply(int x, int y, int a[][5], int b[][5], int c[][5]);

int main(){
    constexpr int max = 5;
    int operation = 0, x = 0, y = 0;
    int a[max][max] = {0};
    int b[max][max] = {0};
    int c[max][max] = {0};

    printf("Please input the function...> ");
    scanf("%d", &operation);
    operation--;
    printf("Please specify the dimesions of the array (x and y) [max. 5]...> ");
    scanf("%d%d", &x, &y);

    if(x <= max && y <= max)
    {
        printf("Please input the first matrix:\n");
        fill(x, y, a);
        printf("Please input the second matrix:\n");
        if(operation)
        {
            fill(y, x, b);
            multiply(x, y, a, b, c);
            printf("\nThe result of the multiplication is:\n\n");
        }
        else
        {
            fill(x, y, b);
            add(x, y, a, b, c);
            printf("\nThe result of the addition is:\n\n");
        }
        for(int i = 0 ; i < x ; i++)
        {
            for(int j = 0 ; j < x ; j++)
            {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
    }
    return 0;
}

void fill(int x, int y, int array[][5])
{
    for(int i = 0 ; i < x ; i++)
    {
        for(int j = 0 ; j < y ; j++)
        {
            scanf("%d", &array[i][j]);
        }
    }
}

void add(int x, int y, int a[][5], int b[][5], int c[][5])
{
    for(int i = 0 ; i < x ; i++)
        {
            for(int j = 0 ; j < y ; j++)
            {
                c[i][j] = a[i][j] + b[i][j];
            }
        }
}

void multiply(int x, int y, int a[][5], int b[][5], int c[][5])
{
    for(int i = 0 ; i < x ; i++)
    {
        for(int j = 0 ; j < x ; j++)
        {
            c[i][j] = 0;
            for(int k = 0 ; k < y ; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}