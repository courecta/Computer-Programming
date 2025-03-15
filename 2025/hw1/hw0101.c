#include "mystring.h"

int main(void){

	const char *str = "hello, world";
    
    	// Normal cases
    	printf("%s\n", mystrchr(str, 'o')); 	// Expected: "o, world"
    	printf("%s\n", mystrchr(str, 'w'));	// Expected: "world"
    
    	// Edge Cases
    	printf("%s\n", mystrchr(str, '\0'));	// Expected: "" (empty string)
    	printf("%p\n", mystrchr(str, 'z'));	// Expected: NULL (not found)
    	printf("%p\n", mystrchr(NULL, 'a'));	// Expected: NULL (null pointer)
	
		// Normal cases
    	printf("%s\n", mystrrchr(str, 'o'));  	// Expected: "orld"
    	printf("%s\n", mystrrchr(str, 'l'));  	// Expected: "ld"
    
    	// Edge Cases
    	printf("%s\n", mystrrchr(str, '\0'));  	// Expected: "" (empty string)
    	printf("%p\n", mystrrchr(str, 'z'));   	// Expected: NULL (not found)
    	printf("%p\n", mystrrchr(NULL, 'a'));  	// Expected: NULL (null pointer)
	
	printf("%zu\n", mystrspn("abcde123", "abcde")); // Expected: 5
    	printf("%zu\n", mystrspn("hello", "lh"));       // Expected: 1
    	printf("%zu\n", mystrspn("12345", "123456789"));// Expected: 5
    	printf("%zu\n", mystrspn("abcdef", ""));        // Expected: 0 (empty `accept`)
    	printf("%zu\n", mystrspn("", "abc"));          	// Expected: 0 (empty `s`)
    	printf("%zu\n", mystrspn(NULL, "abc"));        	// Expected: 0 (NULL `s`)
    	printf("%zu\n", mystrspn("abcdef", NULL));     	// Expected: 0 (NULL `accept`)

	printf("%zu\n", mystrcspn("hello, world", "lo"));	// Expected: 2 (stops at 'l')
    	printf("%zu\n", mystrcspn("abcdef", "xyz"));       	// Expected: 6 (no match found)
    	printf("%zu\n", mystrcspn("abcdef", "c"));        	// Expected: 2 (stops at 'c')
    	printf("%zu\n", mystrcspn("abcdef", ""));          	// Expected: 6 (empty reject, no match)
    	printf("%zu\n", mystrcspn("", "abc"));             	// Expected: 0 (empty `s`)
    	printf("%zu\n", mystrcspn(NULL, "abc"));           	// Expected: 0 (NULL `s`)
    	printf("%zu\n", mystrcspn("abcdef", NULL));        	// Expected: 0 (NULL `reject`)

	printf("%s\n", mystrpbrk("hello, world", "lo"));	// Expected: "llo, world"

	char *result = mystrpbrk("abcdef", "xyz");
	if(result) 
    		printf("%s\n", result);  
	else 
    		printf("NULL\n");  // Print "NULL" safely if no match found

	printf("%s\n", mystrpbrk("abcdef", "cde"));       	// Expected: "cdef"
    	printf("%p\n", mystrpbrk("abcdef", ""));           	// Expected: NULL (empty accept)
    	printf("%p\n", mystrpbrk("", "abc"));              	// Expected: NULL (empty s)
    	printf("%p\n", mystrpbrk(NULL, "abc"));            	// Expected: NULL (NULL `s`)
    	printf("%p\n", mystrpbrk("abcdef", NULL));         	// Expected: NULL (NULL `accept`)

	printf("%s\n", mystrstr("hello world", "world"));	// Expected: "world"
    	printf("%s\n", mystrstr("abcdef", "def"));         	// Expected: "def"
    	printf("%p\n", mystrstr("abcdef", "xyz"));         	// Expected: NULL (not found)
    	printf("%s\n", mystrstr("abcdef", ""));            	// Expected: "abcdef" (empty needle)
    	printf("%p\n", mystrstr("", "abc"));               	// Expected: NULL (empty haystack)
    	printf("%p\n", mystrstr(NULL, "abc"));             	// Expected: NULL (NULL haystack)
    	printf("%p\n", mystrstr("abcdef", NULL));          	// Expected: NULL (NULL needle)

	char string[] = "hello, world! How are you?";
    	char *token = mystrtok(string, " ,!?");

    	while (token) {
        	printf("%s\n", token);
        	token = mystrtok(NULL, " ,!?");
    	}

    	// Edge Cases
    	char empty[] = "";
    	printf("%p\n", mystrtok(empty, " ,!?"));  // Expected: NULL (empty string)

    	char only_delim[] = "   ,,,!!!";
    	printf("%p\n", mystrtok(only_delim, " ,!?"));  // Expected: NULL (only delimiters)

    	printf("%p\n", mystrtok(NULL, " ,!?"));  // Expected: NULL (no more tokens)

	// Additional Edge Cases Testing
	printf("\n--- Additional Edge Cases ---\n");

	// Test mystrchr and mystrrchr with repeated characters
	const char *multi = "aaabbbccc";
	printf("mystrchr (first 'a'): %s\n", mystrchr(multi, 'a'));
	printf("mystrrchr (last 'a'): %s\n", mystrrchr(multi, 'a'));

	// Test mystrtok with consecutive delimiters
	char multi_delim[] = ",,hello,,world,,";
	char *tokenMulti = mystrtok(multi_delim, ",");
	while(tokenMulti)
	{
		printf("token: %s\n", tokenMulti);
		tokenMulti = mystrtok(NULL, ",");
	}

	// Test mystrstr when needle is longer than haystack
	printf("mystrstr with longer needle: %p\n", mystrstr("short", "a very long needle"));

	// Test mystrcspn when first char is in reject
	printf("mystrcspn starting with reject: %zu\n", mystrcspn("xyz", "x"));

	// Test mystrspn when both s and accept are empty
	printf("mystrspn with both empty: %zu\n", mystrspn("", ""));

	return 0;
}
