#include <stdio.h>
#define N 12

int * get_evens();

int main() {
    int *a;
    int k;

    a = get_evens(); /* get first 5 even numbers */
    for (k = 0; k < N; k++)
      printf("%d\n", a[k]); 

    return 0;
}

int * get_evens () {
   static int nums[N];
   int k;
   int even = 0;

   for (k = 0; k < N; k++) {
        nums[k] = even += 2;
   }

    return (nums);
}
