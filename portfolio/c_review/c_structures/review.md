# Structures in C Review

> ## *Assuming int is of size 4 and char is 1*

### P1

```
#include<stdio.h>
struct st
{
    int x;
    static int y;
};

int main()
{
    printf("%d", sizeof(struct st));
    return 0;
}
```

Answer: `Compiler Error`
Explanation: In C, struct and union types cannot have static members. In C++, struct types are allowed to have static members, but union cannot have static members in C++ also.

---

### P2

```
struct node 
{ 
   int i; 
   float j; 
}; 
struct node *s[10];
```

Answer: `s is defined to be an array, each element of which is a pointer to a structure of type node`
Explanation: It defines an array, each element of which is a pointer to a structure of type node.

---

### P3

```
#include<stdio.h> 
struct st 
{ 
    int x; 
    struct st next; 
}; 
  
int main() 
{ 
    struct st temp; 
    temp.x = 10; 
    temp.next = temp; 
    printf("%d", temp.next.x); 
    return 0; 
}
```

Answer: `compiler error`
Explanation: A structure cannot contain a member of its own type because if this is allowed then it becomes impossible for compiler to know size of such struct. Although a pointer of same type can be a member because pointers of all types are of same size and compiler can calculate size of struct

---

### P4

```
union test
{
    int x;
    char arr[8];
    int y;
};

int main()
{
    printf("%d", sizeof(union test));
    return 0;
}
```

Answer: `8`
Explanation: When we declare a union, memory allocated for a union variable of the type is equal to memory needed for the largest member of it, and all members share this same memory space. In above example, "char arr[8]" is the largest member. Therefore size of union test is 8 bytes. 

---

### P5

```
union test
{
    int x;
    char arr[4];
    int y;
};

int main()
{
    union test t;
    t.x = 0;
    t.arr[1] = 'G';
    printf("%s", t.arr);
    return 0;
}
```

Answer: `Nothing is printed`
Explanation: Since x and arr[4] share the same memory, when we set x = 0, all characters of arr are set as 0. O is ASCII value of '0'. When we do "t.arr[1] = 'G'", arr[] becomes "0G00". When we print a string using "%s", the printf function starts from the first character and keeps printing till it finds a 0. Since the first character itself is 0, nothing is printed.

---

### P6

```
# include <iostream>
# include <string.h>
using namespace std;

struct Test
{
  char str[20];
};

int main()
{
  struct Test st1, st2;
  strcpy(st1.str, "GeeksQuiz");
  st2 = st1;
  st1.str[0] = 'S';
  cout << st2.str;
  return 0;
}
```

Answer: `GeeksQuiz`
Explanation: Array members are deeply copied when a struct variable is assigned to another one

---

### P7

```
#include<stdio.h>
struct Point
{
  int x, y, z;
};

int main()
{
  struct Point p1 = {.y = 0, .z = 1, .x = 2};
  printf("%d %d %d", p1.x, p1.y, p1.z);
  return 0;
}
```

Answer: `2 0 1`

---

### P8

```
/* First declaration */
struct node {
int data;
struct node * nextPtr;
};

/* Second declaration */
typedef struct node{
int data;
NODEPTR nextPtr;
} * NODEPTR;
```

Answer: `Anyone of the followings can be used to declare a node for a singly linked list. If we use the first declaration, “struct node * nodePtr;” would be used to declare pointer to a node. If we use the second declaration, “NODEPTR nodePtr;” can be used to declare pointer to a node. This is FALSE`
Explanation: The typedef usage is incorrect. Basically, we can’t use yet to be typedef-ed data type inside while applying typedef itself. Here, NODEPTR is yet to be defined (i.e. typedef-ed) and we are using NODEPTR inside the struct itself.

---

### P9

```
/* First declaration */
typedef struct node
{
 int data;
 struct node *nextPtr;
}* NODEPTR;

/* Second declaration */
struct node
{
 int data;
 struct node * nextPtr;
};
typedef struct node * NODEPTR;
```

Answer: `Anyone of the following can be used to declare a node for a singly linked list and “NODEPTR nodePtr;” can be used to declare pointer to a node using any of the above. This is TRUE`
Explanation: Yes. Both are equivalent. Either of the above declarations can be used for “NODEPTR nodePtr;”. In fact, first one is the compact form of second one.

---

### P10

```
typedef struct Student
{
 int rollno;
 int total;
} Student;

Student s1;
struct Student s2;
```

Answer: `In the following program snippet, both s1 and s2 would be variables of structure type defined as below and there won't be any compilation issue. TRUE`
Explanation: At first, it may seem that having same ‘struct tag name’ and ‘typedef name’ would cause issue here. But it’s perfectly fine for both of them having same name. s1 is defined using typedef name Student while s2 is defined using struct tag name Student.

---

### P11

```
#include "stdio.h"

int main()
{
 struct {int a[2];} arr[] = {{1},{2}};

 printf("%d %d %d %d",arr[0].a[0],arr[0].a[1],arr[1].a[0],arr[1].a[1]);

 return 0;
}
```

Answer: `No compile error and it’ll print 1 0 2 0`
Explanation: Here, struct type definition and definition of arr using that struct type has been done in the same line. This is okay as per C standard. Even initialization is also correct. The point to note is that array size of arr[] would be 2 i.e. 2 elements of this array of this struct type. This is decided due to the way it was initialized above. Here, arr[0].a[0] would be 1 and arr[1].a[0] would be 2. The remaining elements of the array would be ZERO. correct answer is E.

---

### P12

```
#include "stdio.h"

int main()
{
 struct {int a[2], b;} arr[] = {[0].a = {1}, [1].a = {2}, [0].b = 1, [1].b = 2};

 printf("%d %d %d and",arr[0].a[0],arr[0].a[1],arr[0].b);
 printf("%d %d %d\\n",arr[1].a[0],arr[1].a[1],arr[1].b);

 return 0;
}
```

Answer: `No compile error and two elements of arr[] would be defined and initialized. Output would be “1 0 1 and 2 0 2”.`
Explanation: In C, designators can be used to provide explicit initialization. For an array, elements which aren’t initialized explicitly in program are set as ZERO. That’s why correct answer is C.

---

### P3

```
#include <stdio.h>

int main()
{
    struct {
        int i;
        char c;
    } myVar = {.i = 100, .c = 'A'};
    
    printf("%d %c", myVar.i, myVar.c);
    
    return 0;
}
```

Answer: `No compile error and it’ll print 100 A.`
Explanation: As per C language, initialization of a variable of complete data type can be done at the time of definition itself. Also, struct fields/members can be initialized out of order using field name and using dot operator without myVar is ok as per C.

---

### P14

```
#include <stdio.h>

int main()
{
    union {
        int i1;
        int i2;
    } myVar = {.i2 = 100};
    
    printf("%d %d", myVar.i1, myVar.i2);
    
    return 0;
}
```

Answer: `No compile error and it’ll print “100 100”.`
Explanation: Since fields/members of union share same memory, both i1 and i2 refer to same location. Also, since both i1 and i2 are of same type, initializing one would initialize the other as well implicitly. So answer is C.

---

### P15

```
struct addr {
     char city[10];
     char street[30];
     int pin ;
};

struct {
char name[30];
int gender;
struct addr locate;
} person , *kd = &person ;
```

Answer: `Then *(kd -> name +2) can be used instead of *((*kd).name + 2 )`
Explanation: \*(kd -> name +2) = \*((*kd).name + 2 )

---

### P16

```
int main () {
    Integer x;
    return 0;
}
```

Answer: `Semantic analyzer will throw error`
Explanation: Online IDE C compiler returns error: unknown type name ‘Integer’. That is undefined behavior. Though it also should be an syntax error as it does not satisfies syntax of ANSI C program for "Integer".

Lexical analyzer: The lexical analyzer typically would not flag this as an error it just generates tokens for further analysis.
Syntax analyzer: The syntax analyzer checks the syntax of the program based on grammar rules. Since Integer is not a valid type in C, the syntax analyzer would not generate a valid parse tree, but it doesn’t typically flag the error at this point either.
Semantic analyzer: The semantic analyzer checks the meaning of the program, including type checking and variable declarations. Since Integer is not a valid type, the semantic analyzer will detect the type mismatch or undeclared type, and this phase will throw the error.
Machine dependent optimizer: By this point, all type errors have already been caught, so it will not throw an error related to Integer.
Answer:
The Semantic analyzer phase will throw the error because it checks for type correctness, and Integer is not a valid type in C.

Also official key given by GATE authority is "Semantic analyzer".

---

### P17

```
struct {
short s[5];
union {
float y;
long z;
}u;
}t;
```

Answer: `18 bytes`
Explanation: Here structure creates the minimum of array and union, but union only creates the minimum for only 'long z' which is max.
So minimum required =18.

---

### P18

```
Which of the following operators can be applied on structure variables?
i) ==
ii) =
iii) Both
iv) none
```

Answer: `both`
Explanation: Both of the above: In C, both equality comparison (==) and assignment (=) operators can be applied to structure variables, allowing for comparison and assignment of values between structures when their members support it. 

---

### P19

```
struct {int a[2];} arr[] = {1,2};
```

Answer: `No compile error and it’ll create array arr of 1 element. Each of the element of arr contain a struct field of int array of 2 elements. arr[0]. a[0] would be 1 and arr[0].a[1] would be 2.`
Explanation: Since size of array arr isn’t given explicitly, it would be decided based on the initialization here. Without any curly braces, arr is initialized sequentially i.e. arr[0].a[0] would be 1 and arr[0].a[1] would be 2. There’s no further initialization so size of arr would be 1.

---

### P20

```
#include‹stdio.h›
int main()
{
    struct site
    {
        char name[] = "GeeksQuiz";
        int no_of_pages = 200;
    };
    struct site *ptr;
    printf("%d ", ptr->no_of_pages);
    printf("%s", ptr->name);
    getchar();
    return 0;
}
```

Answer: `Compiler Error`
Explanation: When we declare a structure or union, we actually declare a new data type suitable for our purpose. So we cannot initialize values as it is not a variable declaration but a data type declaration.

---

### P21

```
#include <stdio.h>

struct Ournode{
    char x, y, z;
};

int main(){
    struct Ournode p = P{'1', '0', 'a' + 2};
    struct Ournode *q = &p;
    printf("%c %c", *((char*)q + 1), *((char*)q + 2));
    return 0;
}
```

Answer: `0, c`

---