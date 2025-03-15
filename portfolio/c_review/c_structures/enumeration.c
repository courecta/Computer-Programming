#include <stdio.h>

// Fact 1: Enums can be declared in the local scope with names that are automatically initialized by the compiler 
// Fact 2: two or more names can have the same value
// Fact 3: We can assign values in any order, unassigned names are given an incremented value since the last assigned name 
// Fact 4: Only integers are allowed
// Fact 5: All enum constants must be unique in their scope

int main(void){
    
    enum Bool {
        False = 0,
        True
    } var;

    var = True;
    printf("%d\n", var);
    return 0;
}