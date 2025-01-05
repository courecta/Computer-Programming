#include <stdio.h>

// return_type (*pointer_name)(parameter_list);
int (*func_ptr)(int, int);
// we declare a func_ptr as a POINTER to a FUNCTION
// here it RETURNS an int, and takes two ints as arguments

int add(int a, int b);

int main(void){
    func_ptr = add;
    int result = func_ptr(5, 3);
    printf("%d\n", result);
    return 0;
}

int add(int a, int b)
{
    return a + b;
}