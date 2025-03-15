#include <stdio.h>

struct abc{
	int x;
	int y;
	int z;
};

int main(void){
	struct abc a = {.y = 20, .x = 10, .z = 30};
	printf("%d %d %d\n", a.x, a.y, a.z);
	return 0;
}
