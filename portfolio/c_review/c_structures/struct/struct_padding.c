#include <stdio.h>

// assuming size of int is 4 bytes, char is 1 byte

struct abc {
	char a; // 1 byte
	char b; // 1 byte
	int c; 	// 4 bytes 
} var;

// total has to be 6 bytes, right?
// actually, no!
// Why? Because of structure padding
//
// The processor does not read 1 byte at a time from memory
// it reads 1 word at a time
// What does this mean?
//
// For example, having a 32-bit processor means that it accesses 4 bytes at a time
// that is to say, to a 32-bit processor, one word is 4 bytes
// equivalently, a 64-bit processor accesses 8 bytes at a time, so a "word" is 8 bytes
//

// so in the above struct, for a 32-bit architecture, in 1 CPU cycle, it can access char a
// and char b with no issue since they are both within 4 bytes. But it cannot fully access
// the entire integer. So on the first cycle, the integer's first 2 bytes are retrieved,
// then on the 2nd cycle the last 2 bytes are retrieved
//
// this is an unecessary wastage of CPU cycles, so we use padding
//
//         +===============================+
// before: | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
//         +===============================+
//           a   b {       c       }
//
//         [    cycle 1   ][    cycle 2    ]
//                  (3 | 4)
//         +===============================+
// after:  | 1 | 2 | empty | 5 | 6 | 7 | 8 |
//         +===============================+
//	     a   b         {       c       }
//
// so, total = 1 byte + 1 byte + 2 bytes + 4 bytes = 8 bytes

struct new_abc {
	char a;
	int b;
	char c;
};

// Well, hold on, with a different ordering of the structs, its a different size?
// Yes! this is because the padding will be different, like such,
//
//         [    cycle 1    ][    cycle 2   ][     cycle 3     ]
//              (2 | 3)
//         +==================================================+
// after:  | 1 | empty | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
//         +==================================================+
//	     a         {       b       } c
// 
// so, with 3 words accessed, by our earlier logic, then this would be a total of 12 bytes!

int main(void){
	struct abc var;
	printf("%d\n", sizeof(var)); // it really is 8 bytes! (for my WSL Arch Linux)
	
	struct new_abc var1;
	printf("%d\n", sizeof(var1)); // here, its 12 bytes!

	return 0;
}
