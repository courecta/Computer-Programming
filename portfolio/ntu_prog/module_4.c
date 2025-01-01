/* add your value() based on this code */
#include <stdio.h>

int value(int type, int width, int height, int length);
int gcd(int a, int b);

int main(){

	int type, width, height, length;
	scanf("%d%d%d%d", &type, &width, &height, &length);
	printf("%d\n", value(type, width, height, length));
	
    return 0;
}

int value(int type, int width, int height, int length)
{
    switch(type)
    {
        case 79:
        type = 30;
        break;

        case 47:
        type = 10;
        break;

        case 29:
        type = 4;
        break;

        case 82:
        type = 5;
        break;

        case 26:
        type = 3;
        break;

        case 22:
        type = 9;
        break;

        default:
        return -1;
        break;
    }
    if(width < 1 || height < 1 || length < 1) return -2;
    int k = gcd(width, gcd(height, length));
    int num = (width / k) * (height / k) * (length / k);
    int volume = k * k * k;
    int value_per_cube = volume * volume * type;
    
    return num * value_per_cube;
}

int gcd(int a, int b)
{
     int temp = 0;
    while(b != 0)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}