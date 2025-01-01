#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_ABBREVIATIONS 10000
#define MAX_WORD_LENGTH 127

int is_excluded(const char *word);
void abbreviate(const char *word, char *current_abbreviation);
void finalize_abbreviation(char abbreviations[][MAX_WORD_LENGTH + 1], char *current_abbreviation, int *abbreviation_count);

int main() {
    char word[MAX_WORD_LENGTH + 1];
    char current_abbreviation[MAX_WORD_LENGTH + 1] = "";  // Tracks the current abbreviation
    char abbreviations[MAX_ABBREVIATIONS][MAX_WORD_LENGTH + 1];  // Stores all abbreviations
    int abbreviation_count = 0;

    while(scanf("%127s", word) != EOF)
    {
        int len = strlen(word);
        int ends_with_period = (word[len - 1] == '.');
        if(ends_with_period) word[len - 1] = '\0';
        abbreviate(word, current_abbreviation);
        if (ends_with_period) finalize_abbreviation(abbreviations, current_abbreviation, &abbreviation_count);
    }

    // Output all abbreviations
    for (int i = 0; i < abbreviation_count; i++)
    {
        if (i > 0) printf(" ");
        printf("%s", abbreviations[i]);
    }
    printf("\n");
    return 0;
}

int is_excluded(const char *word) {
    return (strcmp(word, "at") == 0 || strcmp(word, "of") == 0 || strcmp(word, "and") == 0 || strcmp(word, "the") == 0);
}

// Handles individual words and adds the first letter to the abbreviation
void abbreviate(const char *word, char *current_abbreviation) {
    if (!is_excluded(word))
    {
        size_t len = strlen(current_abbreviation);
        current_abbreviation[len] = toupper(word[0]);  // Add uppercase first letter
        current_abbreviation[len + 1] = '\0';          // Null-terminate
    }
}

// Finalizes the current abbreviation and resets for the next sequence
void finalize_abbreviation(char abbreviations[][MAX_WORD_LENGTH + 1], char *current_abbreviation, int *abbreviation_count)
{
    if (strlen(current_abbreviation) > 0) {
        strcpy(abbreviations[(*abbreviation_count)++], current_abbreviation);
        current_abbreviation[0] = '\0';
    }
}

/*
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main ()
{
   char str[128];
   int flag = 0;

   while ( scanf ( "%s", str ) != EOF ) {
      if ( flag ) {
         printf ( " " );
         flag = 0;
      }

      if ( strchr ( str, '.' ) )
         flag = 1;
      if ( !strcmp ( str, "and" ) || !strcmp ( str, "at" ) ||
           !strcmp ( str, "of" ) || !strcmp ( str, "the" ) )
         continue;

      printf ( "%c", toupper ( str[0] ) );
   }

   return 0;
}
*/