#include <stdio.h>
#include <string.h>
#define MAX (1024)

int luhns_algorithm(const char buffer[MAX]);

int main(void){
    char buffer[MAX] = {'\0'};
    do
    {
        printf("Number: ");
        scanf("%s", buffer);
    } while(luhns_algorithm(buffer));
    int start = ((buffer[0] - '0') * 10) + ((buffer[1]) - '0');
    if(buffer[0] == '4') printf("Visa\n");
    else if(start == 34 || start == 37) printf("American Express\n");
    else if(start >= 51 && start <= 55) printf("MasterCard\n");
    return 0;
}

int luhns_algorithm(const char buffer[MAX])
{
    int sum = 0, num = 0;
    int size = strlen(buffer) - 1;
    for(int i = size; i >= 0 ; )
    {
        sum += buffer[i] - '0';
        i--;
        num = (buffer[i] - '0') * 2;
        if(num > 9) sum += (num / 10) + (num % 10);
        else sum += num;
        i--;
    }
    printf("%d\n", sum);
    return sum % 10;
}