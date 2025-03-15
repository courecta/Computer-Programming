#include <stdio.h>

struct abc {
	int x;
	int y;
};

int main(void){
	struct abc a = {0, 1};
	struct abc *ptr = &a;

	printf("%d %d\n", ptr->x, ptr->y); // here, something like ptr->x is equivalent to (*ptr).x
	return 0;
}
