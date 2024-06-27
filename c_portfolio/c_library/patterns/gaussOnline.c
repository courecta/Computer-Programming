#include <stdio.h>
#include <stdint.h>

int main(){

    int32_t n;
    printf("\nEnter the order of matrix: ");
    scanf("%d",&n);
    int32_t A[n][n];
    int32_t y[n];
    printf("\nEnter the elements of augmented matrix row-wise:\n\n");
    
    for( size_t i = 1; i <= n; i++){
        
        for( size_t j = 1; j <= (n+1); j++){
            printf("A[%d][%d] : ", i,j);
            scanf("%f",&A[i][j]);
        }
    }

    gaussian_elimination(n, &A[0][0], &y[0], &x);
    
    printf("\nThe solution is: \n");
    for( size_t i = 1 ; i <= n; i++){
        
        printf("\nx%d=%f\t",i,x[i]); /* x1, x2, x3 are the required solutions*/
    }
    
    return(0);

}
int32_t gaussian_elimination( int32_t n, int32_t *pA, int32_t *py, int32_t **px ){

    if (n <= 0 || pA == NULL || py == NULL ){
        return -1;
    }
    
    double c = 0.0;

    *px = (int32_t *)malloc(n * sizeof(int32_t));

    for( size_t j = 1; j <= n; j++){ /* loop for the generation of upper triangular matrix*/
        for( size_t i = 1 ; i <= n ; i++){
            if(i > j){
                
                c = pA[i][j] / pA[j][j];
                
                for( size_t k = 1; k <= n + 1 ; k++){
                    
                    pA[i][k] = pA[i][k] - c * pA[j][k];
                }
            }
        }
    }
    
    py[n] = pA[n][n+1] / pA[n][n];
    
    /* this loop is for backward substitution*/
    
    for( size_t i = n - 1 ; i >= 1 ; i--){
        
        double sum = 0.0;
        for( size_t j = i + 1 ; j <= n ; j++){
            
            sum = sum + pA[i][j] * py[j];
        
        }
        
        (*px)[i] = (py[i][n+1] - sum) / pA[i][i];
    }


}