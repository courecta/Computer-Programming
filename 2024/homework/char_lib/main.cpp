#include "mychar.h"

int main(){
    char c;
    scanf("%c", &c);
    printf("islower(): %d\n", islower2(c));
    printf("isupper(): %d\n", isupper2(c));
    printf("isalpha(): %d\n", ialpha2(c));
    printf("isdigit(): %d\n", isdigit2(c));
    printf("toupper(): %c\n", toupper2(c));
    printf("tolower(): %c\n", tolower2(c));
}