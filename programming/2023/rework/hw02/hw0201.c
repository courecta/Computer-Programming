
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

// constants definitions
#define PI 3.14159265358979323846

// function to get number of user
uint16_t getNum();

// function to get user action
void await_input();
void menu( uint16_t n );

// functions to calculate the irrational constants
double calculate_two( const uint16_t n );
void calculate_pi( const uint16_t n );
double calculate_euler( const uint16_t n );

// function to print lines
void print_line();

int main()
{
  menu(0); 
  return 0;
}

void menu( uint16_t n )
{
  while(n == 0)
  {
    n = getNum();
    if(n != 0) break;
  }

    int32_t operation = 0;
    static double two = 0, euler = 0;
    print_line();
    printf("Which constant do you want to display?\n");

    printf("1. The Square root of 2 by continuous fraction");
    if(two != 0) printf(" (%.15lf)", two);

    printf("\n2. The Gregory-Leibniz and Nilakantha representation of pi");

    printf("\n3. The infinite series representation of euler's constant");
    if(euler != 0) printf(" (%.15lf)", euler);

    printf("\n4. Choose another number");
    printf("\n5. Exit the program\n");
    printf("Your choice: ");
    scanf("%d", &operation);
    switch (operation)
    {
        case 1:
        two = calculate_two(n);
        break;

        case 2:
        calculate_pi(n);
        break;

        case 3:
        euler = calculate_euler(n);
        break;

        case 4:
        uint16_t newNum = getNum(n);
        menu(newNum);
        break;

        case 5:
        printf("eiting...\n");
        exit(EXIT_SUCCESS);
        break;

        default:
        printf("Invalid option!\n");
        break;

    }
    menu(n);
  return;
}

uint16_t getNum()
{
    uint16_t num = 0;
    printf("Please enter n (16-bits unsigned): ");
    scanf("%hu", &num);
    if( num < 1 )
    {
            printf("\nError, invalid input\n");
            return 0;
    } else return num;
}

double calculate_two( const uint16_t n )
{
    print_line();
    double val = 1.0;
    double preVal = 0;
    for(uint16_t i = 1 ; i <= n ; i++)
    {
        printf("n = %hu: %.15lf (%.15lf)\n", i, val, val - sqrt(2.0));
        preVal = val;
        val = 1.0 + (1.0 / (1.0 + val));
    }

    await_input();
    return preVal;

}

void calculate_pi( const uint16_t n )
{
    print_line();
    double val_gl = 4.0, valNil = 3.0, denominator = 3.0, denominatorNil = 2.0, sign = 1.0;

    for(uint16_t i = 1 ; i <= n ; i++)
    { 
        printf("n = %hu:\n\tGregory-leibniz series: %.15lf (%.15lf)\n", i, val_gl, fabs(val_gl - PI));
        printf("\tNilakantha series: %.15lf (%.15lf)\n", valNil, fabs(valNil - PI));
        
        val_gl = val_gl - (sign * ( 4 / denominator));
        valNil = valNil + (sign * (4 / ((denominatorNil) * (denominatorNil + 1) * (denominatorNil + 2))));

        denominator += 2;
        denominatorNil += 2;
        
        sign *= -1;
    }

    await_input();
    return;

}

double calculate_euler( const uint16_t n )
{
    print_line();
    double val = 1.0, factorial = 1.0, preVal = 0;;
    for( uint16_t i = 1 ; i <= n ; i++ )
    {
        printf("n = %hu: %.15lf (%.15lf)\n", i, val, val - exp(1.0));
        factorial *= i;
        preVal = val;
        val += (1 / factorial);
    }

    await_input();
    return preVal;

}

void print_line()
{
    for( int32_t i = 0 ; i < 80 ; i++) printf("-");
    printf("\n");
    return;
}

void await_input()
{
    char temp[UCHAR_MAX];
    getchar();
    printf("Return to menu to continue? [Y/n] ");
    scanf("%s", temp);
    for( uint8_t i = 0 ; temp[i] != '\0' ; i++)
    {
        if(temp[i] == 'N' || temp[i] == 'n')
        {
            printf("exiting...\n");
            exit(EXIT_SUCCESS);
        }
        else if(temp[i] == 'Y' || temp[i] == 'y')
        {
            return;
        }
        else continue;
    }
    await_input();
}

