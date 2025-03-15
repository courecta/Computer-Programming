#include <stdio.h>

#pragma pack(1)

struct abc {
	char a;
	int b;
	char c;
} var; // by struct_padding, we know it is 8 bytes 

// However, because of the packing we enabled, it is now "packed" to its real size!

int main(void){
	printf("%d\n", sizeof(var)); // sure enough, its 6 bytes now!
	return 0;
}
