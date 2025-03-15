### Q1: Sizeof operator returns size in -- `bytes`
---
### Q2: Correct inline declaration -- `int a, b, c;`
---
### Q3: What does the `printf()` function return? -- `Number of characters printed on the screen`
---
### Q4: ASCII decimal ranges of A to Z? -- `65 - 90`
> ###  *lowercase is from `97 = 122`*
---
### Q5: Size of an integer? -- `Depends on the machine, on a 64-bit machine, it is 32 bytes`
---
### Q6: Consider, the following and which of the following are correct?

1) `int 39 = 1;`
2) `int var_39 = 2;`
3) `int _ = 3`

### -- `Both <2> and <3> are valid`
---
### Q7: Consider `int var;` and `extern int var`, what is a correct statement we can derive about them? -- `Statement 1 declares and defines a variable, statement 2 just declares a variable`
---
### Q8: Predict the output of the following,

```
#include <stdio.h>
int var = 5;

int main(void){
    int var = var;
    printf("%d", var);
}
```

### -- `Garbage value`
---
### Q9: Predict the output of the following,

```
#include <stdio.h>

int main(void){
    {
        int var = 10;
    }
    {printf("%d", var);}
}
```

### -- `Compiler error`
---
### Q10: Predict the output of the following,

```
#include <stdio.h>

int main(void){
    unsigned int var = 10;
    printf("%d", ~var);
}
```

### -- `-11`