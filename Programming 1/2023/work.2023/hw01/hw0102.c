#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// This code seeks to achieve multiplcation through single to double digit positive integers

int main(){

int32_t firstNumber, secondNumber;

// Input Machine - checks if the input is valid and calculates the values to prepare for calculation

    printf("Please enter the first number: ");
    int32_t check1 = scanf("%d", &firstNumber);
  	if (0 > firstNumber || firstNumber >= 100 || check1 != 1){
      printf("ERROR\nYou have entered an \x1b[31minvalid input"
           "\x1b[0m, such as a negative integer, non-integer, or a string of letters, Please enter a positive integer between 0 and 99.\n Now run the file again by entering \"./hw0102\", thank you.\n\n");
    return 1;
    }

    printf("Please enter the second number: ");
    int32_t check2 = scanf("%d", &secondNumber);
  	if (0 > secondNumber || secondNumber >= 100 || check2 != 1){
      printf("ERROR\nYou have entered an \x1b[31minvalid input"
           "\x1b[0m, such as a negative integer, non-integer, or a string of letters, Please enter a positive integer between 0 and 99.\n Now run the file again by entering \"./hw0102\", thank you.\n\n");
    return 1;
    }
  
// Initial Process Format - this part takes the Input and formats it into the input display format

	if (check1 == 1 && check2 == 1){
	int32_t ftd = firstNumber / 10, fod = firstNumber % 10, std = secondNumber / 10, sod = secondNumber % 10, prod = firstNumber * secondNumber;;
	bool simple1 = false, simple2 = false, simple = false;
	
	if (0 < firstNumber && firstNumber <= 9){
	simple1 = true;
	}
	if (0 < secondNumber && secondNumber <= 9){
	simple2 = true;
	}
	if (simple1 == 1 && simple2 == 1){
	simple = true;
	}
	
	if (simple1 == 1 && secondNumber == 0){
	printf("  %d\n", firstNumber);
	}
	else if (firstNumber == 0 && simple2 == 1){
	printf("  %d\n", firstNumber);
	}
	else if (firstNumber == 0 && simple2 == 0){
	printf("\t  %d\n", firstNumber);
	}
	else if (simple1 == 1 && simple2 == 0){
    	printf("\t  %d\n", firstNumber);
	}
	else if (simple1 == 0 && firstNumber != 0){
    	printf("\t%d %d\n", ftd, fod);
  	}
	
	if (simple2 == 1 && firstNumber == 0){
	printf("*)%d\n", secondNumber);
	}
	else if (secondNumber == 0 && simple1 == 1){
	printf("*)%d\n", secondNumber);
	}
	else if (secondNumber == 0 && simple1 == 0){
	printf("*)\t  %d\n", secondNumber);
	}
	else if (simple1 == 0 && simple2 == 1){
    	printf("*)\t  %d\n", secondNumber);
	}
  	else if (simple2 == 0 && secondNumber != 0){
    	printf("*)\t%d %d\n", std, sod);
  	}
  	
	if (firstNumber >= 10 || secondNumber >= 10){
    	printf("-----------\n");
   	}
   	else if (firstNumber == 0 && simple2 == 1){
   	printf("---\n");
   	}
	else if (secondNumber == 0 && simple1 == 1){
	printf("---\n");
	}
	else if (simple == 1){
    	printf("---\n");
	}
	
	
// Process Machine - the main part of the code that calculates the individual values for each place value. This is then applied to different output formats for different cases. Edge cases have been calculated (hopefully).

  int32_t a, b, c, d, e, f, aTens, bTens, cTens, dTens, eTens, fTens, aOnes, bOnes, cOnes, dOnes, eOnes, fOnes;

  a = fod * sod, aTens = a / 10, aOnes = a % 10;
  b = (ftd * sod) + aTens, bTens = b / 10, bOnes = b % 10;
  c = bTens;
  d = fod * std, dTens = d / 10, dOnes = d % 10;
  e = (ftd * std) + dTens, eTens = e / 10, eOnes = e % 10;
  f = eTens;

  if (firstNumber == 0 && simple2 == 1){
  printf("  0\n");
  return 0;
  }
  else if (firstNumber == 0 && simple2 == 0){
  printf("\t  0\n");
  return 0;
  }
  else if (secondNumber == 0 && simple1 == 1){
  printf("  0\n");
  return 0;
  }
  else if (secondNumber == 0 && simple1 == 0){
  printf("\t  0\n");
  return 0;
  }
  
  if (simple == 1){
  	if (prod >= 10){
  	printf(" %d\n", prod);
  	}
  	else{
  	printf("  %d\n", prod);
  	}
  return 0;
  }
  
  else if (c == 0 && bOnes != 0 && simple == 0){
  printf("        %d %d\n", bOnes, aOnes);
  }
  else if (c == 0 && bOnes == 0 && simple == 0){
  printf("\t  %d\n", aOnes);
  }
  else if (simple == 0) {
  printf("      %d %d %d\n", c, bOnes, aOnes);
  }
  
  if (secondNumber < 10){
  return 0;
  }
  else if (f == 0 && eOnes != 0){
  printf("      %d %d  \n", eOnes, dOnes);
  } 
  else if (f == 0 && eOnes == 0){
  printf("        %d  \n", dOnes);
  }
  else if (simple == 0){
  printf("    %d %d %d \n", f, eOnes, dOnes);
  }
  
    printf("-----------\n");
  
// Output Format - formats the output for the final product of the multiplication process

if (prod >= 1000){
int32_t prodQuad_1 = prod / 1000, prodQuad_2 = (prod / 100) % 10, prodQuad_3 = (prod / 10) % 10, prodQuad_4 = prod % 10;
printf("    %d %d %d %d\n", prodQuad_1, prodQuad_2, prodQuad_3, prodQuad_4);
}
else if (prod >= 100 && prod < 1000){
int32_t prodTriple_1 = prod / 100, prodTriple_2 = (prod / 10) % 10, prodTriple_3 = prod % 10;
printf("      %d %d %d \n", prodTriple_1, prodTriple_2, prodTriple_3);
}
else if (prod >= 10 && prod < 100){
int32_t prodDouble_1 = prod / 10, prodDouble_2 = prod % 10;
printf("\t%d %d\n", prodDouble_1, prodDouble_2);
}
else if (prod != 0){
printf("  %d\n", prod);
}

}

  return 0;
}
