#include <stdio.h>
#include <string.h>
#include <ctype.h>

int ConvertToDigit(const char str[]);

int main() {
    int n;
    scanf("%d", &n);
    getchar();
    while(n--)
    {
        char line[256];
        fgets(line, sizeof(line), stdin);

        char *token = strtok(line, " \n");
        while (token != NULL) {
            int digit = ConvertToDigit(token);
            if (digit != -1) {
                printf("%d", digit);
            }
            token = strtok(NULL, " \n");
        }
        printf("\n");
    }
}

int ConvertToDigit(const char str[])
{
    const char* digits[] = {
        "zero", "one", "two", "three", "four",
        "five", "six", "seven", "eight", "nine"
    };

    for (int i = 0; i < 10; i++)
    {
        if (strcasecmp(str, digits[i]) == 0)
            return i;
    }
    return -1;
}