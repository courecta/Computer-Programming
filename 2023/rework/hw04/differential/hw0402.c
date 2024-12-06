#include <stdio.h>
#include <stdlib.h>

void print_equation(int degree, int coefficients[]);

int main(){
    // initialize degrees
    int f_degree = 0, g_degree = 0;
    
    // degrees of f(x)
    printf("Please enter f(x) degree: ");
    scanf("%d", &f_degree);
    int f_coefficients[f_degree + 1];

    // coefficients of f(x)
    printf("Please enter f(x) coefficients: ");
    for(int i = 0 ; i <= f_degree ; i++) scanf("%d", &f_coefficients[i]);

    // degrees of g(x)
    printf("Please enter g(x) degree: ");
    scanf("%d", &g_degree);
    int g_coefficients[g_degree + 1];

    // coefficients of g(x)
    printf("Please enter g(x) coefficients: ");
    for(int i = 0 ; i <= g_degree ; i++) scanf("%d", &g_coefficients[i]);

    // leading coefficient must be non-zero
    if(f_coefficients[0] != 0 && g_coefficients[0] != 0)
    {
        // print f(x)
        printf("f(x): ");
        print_equation(f_degree, f_coefficients);

        // print g(x)
        printf("g(x): ");
        print_equation(g_degree, g_coefficients);

        // calculate derivative of f(x)
        if(f_degree > 0)
        {
            int f_differentiated[f_degree];
            for(int i = 0 ; i < f_degree ; i++) f_differentiated[i] = f_coefficients[i] * (f_degree - i);
        }

        // calculate derivative of g(x)
        if(g_degree > 0)
        {
            int g_differentiated[g_degree];
            for(int i = 0 ; i < g_degree ; i++) g_differentiated[i] = g_coefficients[i] * (g_degree - i);
        }
        
        //product rule calculation
        printf("(f(x)g(x))': ");
        // f(x)g'(x)
        int f_gprime_size = (f_degree >= g_degree) ? f_degree : g_degree;
        int f_gprime[f_gprime_size];
        int k = 0;
        for(int i = 0 ; i < f_degree + 1 ; i++)
        {
            for(int j = 0 ; j < g_degree ; j++)
            {
                if(f_coefficients[i] == 0 || g_differentiated[j] == 0) continue;
                f_gprime[k] = 
            }
        }

    }

    return 0;
}

void print_equation(int degree, int coefficients[])
{
    int real_degree = degree;
  
    while (real_degree > 0 && coefficients[real_degree] == 0) real_degree--;
    
    for (int i = 0; i <= real_degree; i++) {
        int coeff = coefficients[i];
        int power = degree - i;

        if (coeff == 0) continue;

        // Print coefficient
        if(coeff > 0 && i > 0) printf("+ ");
        if(coeff < 0) printf("- ");
        if(abs(coeff) != 1 || power == 0) printf("%d", abs(coeff));

        if(power > 0) printf("x");
        if(power > 1) printf("^%d", power);

        printf(" ");
    }
    printf("\n");
}