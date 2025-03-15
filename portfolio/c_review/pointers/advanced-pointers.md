*Assume size of int and pointer are both 4 bytes*

### Q1  
<details>
<summary><i>Click here for code</i></summary>

```
void fun(int *p) 
{ 
  int q = 10; 
  p = &q; 
}     
  
int main() 
{ 
  int r = 20; 
  int *p = &r; 
  fun(p); 
  printf("%d", *p); 
  return 0; 
}
```

```
Explanation

Inside fun(), q is a copy of the pointer p. So if we change q to point something else then p remains uneffected. If we want to change a local pointer of one function inside another function, then we must pass pointer to the pointer. By passing the pointer to the pointer, we can change pointer to point to something else. See the following program as an example.

void fun(int **pptr)
{
  static int q = 10;
  *pptr = &q;
}

int main()
{
  int r = 20;
  int *p = &r;
  fun(&p);
  printf("%d", *p);
  return 0;
}
In the above example, the function fun() expects a double pointer (pointer to a pointer to an integer). Fun() modifies the value at address pptr.  The value at address pptr is pointer p as we pass address of p to fun().  In fun(), value at pptr is changed to address of q.  Therefore, pointer p of main() is changed to point to a new variable q. Also, note that the program won’t cause any out of scope problem because q is a static variable. Static variables exist in memory even after functions return. For an auto variable, we might have seen some unexpected output because auto variable may not exist in memory after functions return.
```

</details>

---

### Q2
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>

#define R 10
#define C 20

int main()
{
   int (*p)[R][C];
   printf("%d",  sizeof(*p));
   getchar();
   return 0;
}
```

```
Explanation

Output is 10*20*sizeof(int) which is “800″ for compilers with integer size as 4 bytes. When a pointer is de-referenced using *, it yields type of the object being pointed. In the present case, it is an array of array of integers. So, it prints R*C*sizeof(int).
```

</details>

---

### Q3
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
int main()
{
    int a[5] = {1,2,3,4,5};
    int *ptr = (int*)(&a+1);
    printf("%d %d", *(a+1), *(ptr-1));
    return 0;
}
```

```
Explanation

The program prints “2 5″. Since compilers convert array operations in pointers before accessing the array elements, (a+1) points to 2. The expression (&a + 1) is actually an address just after end of array ( after address of 5 ) because &a contains address of an item of size 5*integer_size and when we do (&a + 1) the pointer is incremented by 5*integer_size. ptr is type-casted to int * so when we do ptr -1, we get address of 5
```

</details>

---

### Q4
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>

char *c[] = {"GeksQuiz", "MCQ", "TEST", "QUIZ"};
char **cp[] = {c+3, c+2, c+1, c};
char ***cpp = cp;

int main()
{
    printf("%s ", **++cpp);
    printf("%s ", *--*++cpp+3);
    printf("%s ", *cpp[-2]+3);
    printf("%s ", cpp[-1][-1]+1);
    return 0;
}
```

```
Explanation

Let us first consider **++cpp. Precedence of prefix increment and de-reference is same and associativity of both of them is right to left. So the expression is evaluated as **(++cpp). So cpp points to c+2. So we get "TEST" as output. Note the de-reference operator twice. Similarly, you may try other expressions yourself with the help of precedence table.
```

</details>

---

### Q5
<details>
<summary><i>Click here for code</i></summary>

```
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void fun(char** str_ref)
{
    str_ref++;
}

int main()
{
    char *str = (void *)malloc(100*sizeof(char));
    strcpy(str, "GeeksQuiz");
    fun(&str);
    puts(str);
    free(str);
    return 0;
}
```

```
Explanation

Note that str_ref is a local variable to fun(). When we do str_ref++, it only changes the local variable str_ref. We can change str pointer using dereference operator *. For example, the following program prints "eeksQuiz"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void fun(char** str_ref)
{
    (*str_ref)++;
}

int main()
{
    char *str = (void *)malloc(100*sizeof(char));
    strcpy(str, "GeeksQuiz");
    fun(&str);
    puts(str);
    free(str);
    return 0;
}
```

</details>

---

### Q6
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
void f(char**);
int main()
{
    char *argv[] = { "ab", "cd", "ef", "gh", "ij", "kl" };
    f(argv);
    return 0;
}
void f(char **p)
{
    char *t;
    t = (p += sizeof(int))[-1];
    printf("%s\\n", t);
}
```

```
Explanation

The expression (p += sizeof(int))[-1] can be written as (p += 4)[-1] which can be written as (p = p+4)[-] which returns address p+3 which is address of fourth element in argv[].
```

</details>

---

### Q8
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i;
    int *ptr = (int *) malloc(5 * sizeof(int));

    for (i=0; i<5; i++)
        *(ptr + i) = i;

    printf("%d ", *ptr++);
    printf("%d ", (*ptr)++);
    printf("%d ", *ptr);
    printf("%d ", *++ptr);
    printf("%d ", ++*ptr);
}
```

```
Explanation

The important things to remember for handling such questions are 1) Prefix ++ and * operators have same precedence and right to left associativity. 2) Postfix ++ has higher precedence than the above two mentioned operators and  associativity is from left to right. We can apply the above two rules to guess all *ptr++ is treated as *(ptr++) *++ptr is treated as *(++ptr) ++*ptr is treated as ++(*ptr)
```

</details>

---

### Q9
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
int fun(int arr[]) {
   arr = arr+1;	
   printf("%d ", arr[0]);
}
int main(void) {
   int arr[2] = {10, 20};
   fun(arr);
   printf("%d", arr[0]);
   return 0;
}
```

```
Explanation

In C, array parameters are treated as pointers (See http://www.geeksforgeeks.org/why-c-treats-array-parameters-as-pointers/ for details). So the variable arr represents an array in main(), but a pointer in fun().
```

</details>

---

### Q10
<details>
<summary><i>Click here for code</i></summary>

```
$include <stdio.h>
int f(int x, int *py, int **ppz)
{
  int y, z;
  **ppz += 1; 
   z  = **ppz;
  *py += 2;
   y = *py;
   x += 3;
   return x + y + z;
}
 
void main()
{
   int c, *b, **a;
   c = 4;
   b = &c;
   a = &b; 
   printf( "%d", f(c,b,a));
   getchar();
}
```

```
/* Explanation for the answer */

/*below line changes value of c to 5. Note that x remains unaffected 
	by this change as x is a copy of c and address of x is different from c*/
**ppz += 1 

/* z is changed to 5*/
z = **ppz; 

/* changes c to 7, x is not changed */
*py += 2; 

/* y is changed to 7*/
y = *py; 

/* x is incremented by 3 */
x += 3; 

/* return 7 + 7 + 5*/
return x + y + z; 

```

</details>

---

### Q11
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
int main()
{ 
   unsigned int x[4][3] = {{1, 2, 3}, {4, 5, 6}, 
                           {7, 8, 9}, {10, 11, 12}};
   printf("%u, %u, %u", x+3, *(x+3), *(x+2)+3);
}
```

```
Explanation

x = 2000

Since x is considered as a pointer to an 
array of 3 integers and an integer takes 4
bytes, value of x + 3 = 2000 + 3*3*4 = 2036

The expression, *(x + 3) also prints same 
address as x is 2D array.


The expression *(x + 2) + 3 = 2000 + 2*3*4 + 3*4
                            = 2036
```

</details>

---

### Q12
<details>
<summary><i>Click here for code</i></summary>

```
# include 
int main( )
{
  static int a[] = {10, 20, 30, 40, 50};
  static int *p[] = {a, a+3, a+4, a+1, a+2};
  int **ptr = p;
  ptr++;
  printf("%d%d", ptr - p, **ptr};
}
```

```
Explanation

Step 1: Analyze the Arrays and Pointers
Array a[]: a = {10,20,30,40,50}
a[0]=10, a[1]=20, a[2]=30, a[3]=40, a[4]=50.
Pointer Array p[]: Each element in p[] is a pointer pointing to specific elements in a[]: p[0] = a, p[1] = a+3 , p[2] = a+4, p[3] = a+1, p[4] = a+2.
So:
p[0]=&a[0] (value = 10),
p[1]=&a[3] (value = 40),
p[2]=&a[4] (value = 50),
p[3]=&a[1] (value = 20),
p[4]=&a[2] (value = 30).
Pointer to Pointer ptr:
Initially, ptr = p (i.e., ptr points to the first element of p[])
Step 2: Perform the Operations
Increment ptr: ptr++: Now ptr points to p[1], which is a+3 (i.e., the address of a[3]).
Evaluate ptr−p:
ptr−p: The difference in addresses between ptr and p gives the index of ptr in p[].
Since ptrptrptr now points to p[1]p[1]p[1], ptr−p=1ptr - p = 1ptr−p=1.
Evaluate ∗∗ptr:
∗∗ptr: Dereferencing ptr twice gives the value at the address p[1].
p[1] = a + 3, and the value at a+3 is a[3] = 40.
Step 3: Final Output
The output is: 140

Explanation:

The first number (ptr−p) is 111.
The second number (∗∗ptr) is 40.
```

</details>

---

### Q13
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdlib.h>
int main()
{
 int *pInt;
 int **ppInt1;
 int **ppInt2;

 pInt = (int*)malloc(sizeof(int));
 ppInt1 = (int**)malloc(10*sizeof(int*));
 ppInt2 = (int**)malloc(10*sizeof(int*));

 free(pInt);
 free(ppInt1);
 free(*ppInt2);
 return 0;
}
```

```
Explanation

ppInt2 is pointer to pointer to int. *ppInt2 is pointer to int. So long as the argument of free() is pointer, there's no issue. That's why B) and C) both are not correct. Allocation of both ppInt1 and ppInt2 is fine as per their data type. So A) is also not correct. The correct statement is D).
```

</details>

---

### Q4
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h> 
int main()
{
 void *pVoid;
 pVoid = (void*)0;
 printf("%lu",sizeof(pVoid));
 return 0;
}
```

```
Explanation

(void *)0 is basically NULL pointer which is used for many purposes in C. Please note that no matter what is the type of pointer, each pointer holds some address and the size of every pointer is equal to sizeof(int). So D) isn't correct. An absolute address can be assigned to any pointer though it might result in issues at run time if the address is illegal. Since 0 is a legal address, assigning (void *)0 to pVoid is fine. So B) isn't correct. We aren't doing any illegal operation with pVoid here. So it'll not result in any compile/run time error. So A) isn't correct. For example, if we perform illegal operation over pVoid e.g. de-referencing of void pointer i.e. *pVoid, it'll result in error. The above program will compile/run without any issue. So C) is correct.
```

</details>

---

### Q15
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
int arr[10][10][10];
int main()
{
 arr[5][5][5] = 123;
 return 0;
}

// (i) printf("%d",arr[5][5][5]);
// (ii) printf("%d",*(*(*(arr+5)+5)+5));
// (iii) printf("%d",(*(*(arr+5)+5))[5]);
// (iv) printf("%d",*((*(arr+5))[5]+5));

```

```
Explanation

For arrays, we can convert array subscript operator [] to pointer deference operator * with proper offset. It means that arr[x] is equal to *(arr+x). Basically, these two are interchangeable. The same concept can be applied in multi-dimensional arrays as well. That's why all of the above 4 printf are referring to the same element i.e. 

arr[5][5][5]
```

</details>

---

### Q16
<details>
<summary><i>Click here for code</i></summary>

```
#include "stdio.h"

typedef int (*funPtr)(int);

int inc(int a)
{
 printf("Inside inc() %d\\n",a);
 return (a+1);
}

int main()
{

 funPtr incPtr1 = NULL, incPtr2 = NULL;

 incPtr1 = &inc; /* (1) */
 incPtr2 = inc; /* (2) */

 (*incPtr1)(5); /* (3) */
 incPtr2(5); /* (4)*/

 return 0;
}
```

```
Explanation

While assigning any function to function pointer, & is optional. Same way, while calling a function via function pointer, * is optional.
```

</details>

---

### Q17
<details>
<summary><i>Click here for code</i></summary>

```
#include <stdio.h>
int main(void)
   {
    char c[ ] = "ICRBCSIT17";
    char *p=c;
    printf("%s", c+2[p] – 6[p] – 1);
    return 0;
   }
```

```
Explanation

Given String = "ICRBCSIT17" Index of I=0, C=1, R=2, B=3 and so on. Now we are making a pointer p point to character array c. Here 2[p] = p[2] ='R' and 6[p] = p[6] ='I' 'R'-'I' = 9 and c + 2[p] – 6[p] – 1 = c + 9 - 1 = c + 8 So "17" is printed as the string.
```

</details>

---

### Q18
<details>
<summary><i>Click here for code</i></summary>

*Assuming little endian*

```
#include<stdio.h>

int main() {
    short a = 320;
    char * ptr;
    ptr = (char * ) & a;
    printf("%d", * ptr);
    return 0;
}
```

```
Explanation

Big Endian Byte Order: The most significant byte of the data is placed at the byte with the lowest address. The rest of the data is placed in order in the next three bytes in memory. Little Endian Byte Order: The least significant byte of the data is placed at the byte with the lowest address. The rest of the data is placed in order in the next three bytes in memory. a= 320, 2-byte representation of a: But as we are using little-endian format here, so it would actually stored in memory like : ptr which is a character pointer, on dereferencing will output a single byte, i.e the first byte: (01000000)2 which is equal to 64. So, the correct option is (C) 64.
```

</details>