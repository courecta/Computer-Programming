#include <stdio.h>
#include <string.h>
#include <ctype.h>

void cipher(const char* text, const char* key);

int main(int argc, char* argv[]){

    if(argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return -1;
    }
    if(strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return -1;
    }
    char text[256] = {' '};
    printf("plaintext: ");
    fgets(text, 256, stdin);
    printf("ciphertext: ");
    cipher(text, argv[1]);
    return 0;
}

void cipher(const char* text, const char* key)
{
    char c;
    int size = strlen(text);
    for(int i = 0; i < size; i++)
    {
        if(islower(text[i]))
        {
            c = key[(toupper(text[i]) - 'A')];
            printf("%c", tolower(c));    
        }
        else
        {
            c = key[(text[i] - 'A')];
            printf("%C", c);
        }
    }
    printf("\n");
}