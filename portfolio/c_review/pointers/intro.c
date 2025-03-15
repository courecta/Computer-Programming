#include <stdio.h>
#include <assert.h>

int main(void){

    int x = 5, *ptr = &x;

    // the above is equivalent to,
    // 
    // int x = 5;
    // int *ptr;
    // ptr = &x;

    printf("Initial ptr: %d\n", *ptr);

    *ptr = 4;

    printf("After reassignment, ptr: %d\nOriginal x variable: %d\n", *ptr, x);

    // This is only possible due to the dereference operator, *
    // Also, always make sure to initialize the pointer
    // To "initialize" a pointer, is to point it towards an address, like *ptr = &var;
    // Note that the dereference operator is *, which is the same as declaration
    // declaration: (data_type *ptr) <--X--> dereference: (*ptr) 

    assert(*ptr == x && *&x == x); // assertion error not raised!
    printf("Other ways to access x: %d %d\n", *ptr, *&x);

    // *ptr = &x is the assignment from the pointer ptr to the integer variable x
    // here, x can be accessed by *ptr, but we can also use *(&(x)) --> *&x
    // These here are the aliases of the variable x

    // furthermore,
    // *&ptr = *(&ptr) = *(address of p) = some random memory address!
    // &ptr = address of the ptr, not x!
    // *x = *(4) , which doesn't make sense!
    
    return 0;
}