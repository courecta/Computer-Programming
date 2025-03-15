#include "mysplit.h"

int main(void){
    char **ppStrs = NULL;
    int32_t number = mysplit(&ppStrs , "  ", " ");

    for( int32_t i = 0 ; i < number ; i++ ) printf( "%d) %s\n", i, ppStrs[i] );

    // New Test Case 1: Empty input string.
    number = mysplit(&ppStrs, "", " ");
    printf("\nTest: Empty input string\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0 ; i < number ; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Test Case 2: No delimiter match.
    number = mysplit(&ppStrs, "hello", " ");
    printf("\nTest: No delimiter match\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0 ; i < number ; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Test Case 3: Input string with only delimiters.
    number = mysplit(&ppStrs, "   ", " ");
    printf("\nTest: Input string with only delimiters\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0 ; i < number ; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Test Case 4: Invalid delimiter (empty string).
    number = mysplit(&ppStrs, "hello", "");
    printf("\nTest: Invalid delimiter (empty string)\n");
    if(number == -1)
        printf("Error: Invalid delimiter\n");

    // New Complex Test Case 5: Text with multi-character delimiter in-between.
    // Input: "The quick---brown---fox" with delimiter "---"
    // Expected:
    // 0) The quick
    // 1) brown
    // 2) fox
    number = mysplit(&ppStrs, "The quick---brown---fox", "---");
    printf("\nTest: Complex Test Case 5\nExpected:\n0) The quick\n1) brown\n2) fox\nActual:\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0; i < number; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Complex Test Case 6: Multiple occurrences of delimiter.
    // Input: "hello...world...again" with delimiter "..."
    // Expected:
    // 0) hello
    // 1) world
    // 2) again
    number = mysplit(&ppStrs, "hello...world...again", "...");
    printf("\nTest: Complex Test Case 6\nExpected:\n0) hello\n1) world\n2) again\nActual:\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0; i < number; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Complex Test Case 7: String starting and ending with delimiters.
    // Input: "##start##middle##end##" with delimiter "##"
    // Expected:
    // 0) (empty)
    // 1) start
    // 2) middle
    // 3) end
    // 4) (empty)
    number = mysplit(&ppStrs, "##start##middle##end##", "##");
    printf("\nTest: Complex Test Case 7\nExpected:\n0) \"\"\n1) start\n2) middle\n3) end\n4) \"\"\nActual:\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0; i < number; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    // New Complex Test Case 8: Consecutive delimiters within text.
    // Input: "a,b,,c" with delimiter ","
    // Expected:
    // 0) a
    // 1) b
    // 2) (empty)
    // 3) c
    number = mysplit(&ppStrs, "a,b,,c", ",");
    printf("\nTest: Complex Test Case 8\nExpected:\n0) a\n1) b\n2) \"\"\n3) c\nActual:\n");
    if(number == -1)
        printf("Error: Invalid input\n");
    else
        for(int32_t i = 0; i < number; i++) printf("%d) \"%s\"\n", i, ppStrs[i]);

    return 0;
}