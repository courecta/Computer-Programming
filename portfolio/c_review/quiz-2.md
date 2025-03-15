### Q1: a true statement about static functions in C -- `Static functions are restricted to the files where they are declared`
---
### Q2: In C, is it mandatory to declare a function before use? -- `No, it is optional`
---
### Q3: What keyword is used to come out of a loop for that iteration only? -- `continue`
---
### Q4: Correct ways to declare a function prototype include,

```
int fun(int var1, int var2); // 1
int fun(int, int);           // 2
fun(int, int);               // 3
```

### -- `Only 1 and 2`
---
### Q5: Does C support dynamic scoping? -- `No`
---
### Q6: In C, parameters are always `passed by value`

> ### *the only way to pass by reference is to explicity pass pointer values*
---
### Q7: In C, if a function, `void fun();` is declared as a prototype with an empty parameter list, it is? -- `a function that can be called with any number of paramters of any type`
---
### Q8: Assuming int is 4 bytes, how does the following run?

```
#include <stdio.h>

int main(void){
    printf("Hi!\n");
    main();
    return 0;
}
```

### -- `"Hi!", would be printed until stack overflow happens for this program`
---
### Q9: What is the best statement for the `break` and `continue` statements in C? -- `both can be used in for, while, and do-while loops, but only break can be used for switch statements`
---
### Q10: Determine the output of the following,

```
#include <stdio.h>

int main(void){
    int i = 9;
    for(; i; )
    {
        printf("Hmmm...);
        i--;
    }
}
```

### -- `Hmmm... will be printed 9 times`