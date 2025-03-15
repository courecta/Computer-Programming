#include "mymixed.h"

void print_test_header(const char* title) {
    printf("\n=== %s ===\n", title);
}

int main(void) {
    sMixedNumber num1, num2, result;
    
    print_test_header("BASIC TESTS");
    // Original tests
    mixed_input(&num1, "2\\frac{1}{3}");
    printf("Number 1: ");
    mixed_print(&num1);
    printf("\n");
    
    mixed_input(&num2, "-\\frac{2}{3}");
    printf("Number 2: ");
    mixed_print(&num2);
    printf("\n");
    
    printf("Addition: ");
    mixed_add(&result, num1, num2);
    mixed_print(&result);
    printf(" (Expected: 1\\frac{2}{3})\n");
    
    printf("Subtraction: ");
    mixed_sub(&result, num1, num2);
    mixed_print(&result);
    printf(" (Expected: 3)\n");
    
    printf("Multiplication: ");
    mixed_mul(&result, num1, num2);
    mixed_print(&result);
    printf(" (Expected: -1\\frac{5}{9})\n");
    
    printf("Division: ");
    mixed_div(&result, num1, num2);
    mixed_print(&result);
    printf(" (Expected: -3\\frac{1}{2})\n");
    
    printf("Comparison result: %d (Expected: 1)\n", mixed_compare(num1, num2));
    
    // Edge cases
    print_test_header("EDGE CASES");
    
    // Zero tests
    mixed_input(&num1, "0");
    mixed_input(&num2, "\\frac{0}{5}");
    printf("Zero as integer: ");
    mixed_print(&num1);
    printf("\nZero as fraction: ");
    mixed_print(&num2);
    printf("\nComparing zeros: %d (Expected: 0)\n", mixed_compare(num1, num2));
    
    // Zero integer part
    mixed_input(&num1, "0\\frac{5}{7}");
    mixed_input(&num2, "\\frac{5}{7}");
    printf("Mixed with zero integer: ");
    mixed_print(&num1);
    printf("\nJust fraction: ");
    mixed_print(&num2);
    printf("\nComparing: %d (Expected: 0)\n", mixed_compare(num1, num2));
    
    // Large numbers
    print_test_header("LARGE NUMBERS");
    mixed_input(&num1, "999\\frac{999}{1000}");
    mixed_input(&num2, "1000");
    printf("Num1: ");
    mixed_print(&num1);
    printf("\nNum2: ");
    mixed_print(&num2);
    printf("\n");
    
    mixed_sub(&result, num2, num1);
    printf("Num2 - Num1: ");
    mixed_print(&result);
    printf(" (Expected: \\frac{1}{1000})\n");
    
    // Negative fractions
    print_test_header("NEGATIVE FORMS");
    mixed_input(&num1, "-5\\frac{1}{4}");
    mixed_input(&num2, "-\\frac{3}{4}");
    printf("Num1: ");
    mixed_print(&num1);
    printf("\nNum2: ");
    mixed_print(&num2);
    printf("\n");
    
    mixed_add(&result, num1, num2);
    printf("Addition: ");
    mixed_print(&result);
    printf(" (Expected: -6)\n");
    
    // Complex arithmetic
    print_test_header("COMPLEX ARITHMETIC");
    mixed_input(&num1, "7\\frac{3}{12}");  // Should normalize to 7\\frac{1}{4}
    mixed_input(&num2, "2\\frac{8}{16}");  // Should normalize to 2\\frac{1}{2}
    printf("Num1 (reduced): ");
    mixed_print(&num1);
    printf("\nNum2 (reduced): ");
    mixed_print(&num2);
    printf("\n");
    
    mixed_add(&result, num1, num2);
    printf("Addition: ");
    mixed_print(&result);
    printf(" (Expected: 9\\frac{3}{4})\n");
    
    mixed_mul(&result, num1, num2);
    printf("Multiplication: ");
    mixed_print(&result);
    printf(" (Expected: 18\\frac{1}{8})\n");
    
    // Mixed signs
    print_test_header("MIXED SIGNS");
    mixed_input(&num1, "3\\frac{1}{5}");
    mixed_input(&num2, "-1\\frac{3}{10}");
    printf("Num1: ");
    mixed_print(&num1);
    printf("\nNum2: ");
    mixed_print(&num2);
    printf("\n");
    
    mixed_add(&result, num1, num2);
    printf("Addition: ");
    mixed_print(&result);
    printf(" (Expected: 1\\frac{9}{10})\n");
    
    mixed_mul(&result, num1, num2);
    printf("Multiplication: ");
    mixed_print(&result);
    printf(" (Expected: -4\\frac{11}{50})\n");
    
    // Close values
    print_test_header("CLOSE VALUES");
    mixed_input(&num1, "5\\frac{49}{100}");
    mixed_input(&num2, "5\\frac{51}{100}");
    printf("Comparing: %d (Expected: -1)\n", mixed_compare(num1, num2));
    
    return 0;
}