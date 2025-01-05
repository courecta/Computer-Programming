#include "matrix.h"

int main(void){

    int choice = 0, exit = 0;
    printf("Welcome to the matrix\n======================\n");

    while(!exit)
    {
        printf("1.\tMatrix Addition\n2.\tMatrix Multiplication\n3.\tGaussian Elimination\n4.\tDeterminant\n5.\tInversion\n6.\tEigenvector\n7.\tOrthogonality\n");
        printf("Choose an operation (-1 to exit):\t\n");
        scanf("%d", &choice);
        
        switch(choice)
        {
            case 1:
            add();
            break;

            case 2:
            multiply();
            break;

            case 3:
            gauss();
            break;

            case 4:
            determinant();
            break;

            case 5:
            inverse();
            break;

            case 6:
            eigen();
            break;

            case 7:
            orthogonal();
            break;

            case -1:
            exit++;
            break;
        }
    }
    return 0;
}

void add()
{
    int A[m][n], B[m][n], x = 0, y = 0;
    printf("Please enter M-rows and N-columns as a pair(m, n): ");
    scanf("%d%d", &x, &y);
    populateMatrix(x, y, A);
    printf("\nSecond Matrix\n");
    populateMatrix(x, y, B);
    printf("\n");
    addition(x, y, A, B);
}

void multiply()
{
    int A[m][n], B[n][p], x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    printf("Please enter M-rows and N-columns as a pair(m, n): ");
    scanf("%d%d", &x1, &y1);
    populateMatrix(x1, y1, A);
    printf("\nSecond Matrix\n");
    printf("Please enter N-rows and P-columns as a pair(n, p): ");
    do
    {
        scanf("%d%d", &x2, &y2);
    } while(x2 != y1); 
    populateMatrix(x, y, B);
    multiplication(x1, y1, y2 A, B);
}

void gauss()
{
    int32_t n = 3;

    // Flatten the matrix into a single array
    int32_t A[] = {
        1, 1, 1,
        1, 2, 4,
        1, 3, 9
    };
    int32_t y[] = {6, 17, 34};
    int32_t *x = NULL;
    int32_t x_answer[] = {1, 2, 3};

    int32_t result = gaussian_elimination(n, A, y, &x);

    if (result == 1) {
        printf("The solution is:\n");
        for (int32_t i = 0; i < n; ++i) {
            printf("x[%d] = %d\n", i, x[i]);
        }
        printf("example solution:\n");
        for (int32_t i = 0; i < n; ++i) {
            printf("x[%d] = %d\n", i, x_answer[i]);
        }
    } else {
        printf("Wrong Answer. Result should be 1.\n");
    }

    if (x != NULL) free(x);
}

void determinant()
{
    int matrix[n][n], size = 0;
    printf("Please enter the Row and Column count of the matrix (n): ");
    scanf("%d", &size);
    populateMatrix(size, size, matrix);
    printf("%d\n", findDeterminant(size, matrix));
}

void inverse()
{
    int matrix[n][n], size = 0;
    printf("Please enter the Row and Column count of the matrix (n): ");
    scanf("%d", &size);
    populateMatrix(size, size, matrix);
    if(findDeterminant(size, matrix))
    {
        invert(size, matrix);
    } else printf("det(matrix) = 0 --- MATRIX IS NON-INVERTIBLE ---\n");
}

void eigen()
{

}

void orthogonal()
{

}

void populateMatrix(int m, int n, int matrix[m][n])
{
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
}