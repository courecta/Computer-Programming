#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main (){
  int32_t x1, x2, y1, y2;
  float m, b, xint, yint, a;
  bool validPass = false;

// Checks for valid input of integers and not letter strings or symbols

  printf("Please enter the point A (x, y): ");
  int32_t coord1 = scanf("%d, %d", &x1, &y1);
  
  if (coord1 != 2){
  printf("Please input a valid integer and don't forget to inlcude a comma!\n");
  return 1;
  }
  
  printf("Please enter the point B (x, y): ");
  int32_t coord2 = scanf("%d, %d", &x2, &y2);

  if (coord2 != 2){
  printf("Please input a valid integer and don't forget to inlcude a comma!\n");
  return 1;
  }
  else{
  validPass = true;
  }

// These 2 below are for testing cases of asymptotes

  if (x2 - x1 == 0){
    printf("Area : 0\n");
    return 1;
  }

	if (y2 - y1 == 0){
    printf("Area : Infinite\n");
    return 1;
  }
  if (validPass == 1){
    m = (y2 - y1)/(x2 - x1);
    b = y1 - (m * x1);
  }

// testing for cases of invalid areas

	if (b == 0 && validPass == 1){
	printf("Area : Infinite\n\n"
	"Please do not input any equivalent equations of the line y = x!\n"	
	);
	return 1;
	}

  xint = b;
  yint = -(b)/m;

  a = (xint * yint)/2;

// My aim is for this code to be able to return any valid area relative to the x/y or -x/-y lines, thus the valid output area needs to be unsigned/absolute valued

  if (a < 0){
    a = a*-1;
  }

  printf("Area : %.2f\n", a);

  return 0;
}
