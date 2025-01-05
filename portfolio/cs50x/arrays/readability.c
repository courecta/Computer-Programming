#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define MAX (1024)

double letter_per_hundred(const char* text);
double count_per_hundred(const char* text);

int main(void){
    char text[MAX] = {' '};
    printf("Text: ");
    fgets(text, MAX, stdin);
    double index = 0.0588 * letter_per_hundred(text) - 0.296 * count_per_hundred(text) - 15.8;
    // printf("Real score: %lf\n", index);
    double grade = round(index);
    if(grade < 1.0) printf("Before Grade 1\n");
    else if(grade > 16.0) printf("Grade 16+\n");
    else printf("Grade %.0lf\n", grade);
    return 0;
}

double letter_per_hundred(const char* text)
{
    int letters = 0;
    int words = 0;
    int i = 0;
    int in_word = 0;

    while (text[i]) {
        if (isalpha(text[i])) {
            letters++;
            if (!in_word) {
                words++;
                in_word = 1;
            }
        } else if (isspace(text[i]) || ispunct(text[i])) {
            in_word = 0;
        }
        i++;
    }

    return words ? (double)(letters * 100) / words : 0;
}

double count_per_hundred(const char* text)
{
    int sentences = 0;
    int words = 0;
    int i = 0;
    int in_word = 0;

    while (text[i]) {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentences++;
        }
        if (isalpha(text[i])) {
            if (!in_word) {
                words++;
                in_word = 1;
            }
        } else if (isspace(text[i]) || ispunct(text[i])) {
            in_word = 0;
        }
        i++;
    }

    return words ? (double)(sentences * 100) / words : 0;
}