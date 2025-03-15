#include "mymixed.h"

// Helper function to find the greatest common divisor (GCD)
int32_t gcd(int32_t a, int32_t b)
{
    a = (a < 0) ? -a : a;  // Make sure we work with positive values
    b = (b < 0) ? -b : b;
    while(b != 0)
    {
        int32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Fixed normalization function:
// This version converts the mixed number to an improper fraction using absolute values,
// reduces the fraction, and converts back using proper floor division for negatives.
void normalize(sMixedNumber *pNumber)
{
    if (pNumber->denominator < 0) {
        pNumber->numerator = -pNumber->numerator;
        pNumber->denominator = -pNumber->denominator;
    }    

    // Handle invalid denominator.
    if(pNumber->denominator == 0)
    {
        pNumber->integer = 0;
        pNumber->numerator = 0;
        pNumber->denominator = 1;
        return;
    }
    
    // If numerator is zero, no fraction part remains.
    if(pNumber->numerator == 0)
    {
        // The integer part remains as is, but we set the denominator to 1.
        pNumber->denominator = 1;
        return;
    }
    
    // Determine if the number is negative.
    int negative = (pNumber->integer < 0 || (pNumber->integer == 0 && pNumber->numerator < 0));
    
    // Use absolute values to compute the improper fraction.
    int64_t absInteger   = (pNumber->integer < 0) ? -(int64_t)pNumber->integer : pNumber->integer;
    int64_t absNumerator = (pNumber->numerator < 0) ? -(int64_t)pNumber->numerator : pNumber->numerator;
    int64_t improper   = absInteger * pNumber->denominator + absNumerator;
    
    // Reduce the improper fraction.
    int32_t divisor = gcd((int32_t)improper, pNumber->denominator);
    improper /= divisor;
    int32_t denom = pNumber->denominator / divisor;
    
    // Convert the reduced improper fraction back to mixed form.
    if(!negative)
    {
        pNumber->integer    = (int32_t)(improper / denom);
        pNumber->numerator  = (int32_t)(improper % denom);
        pNumber->denominator = denom;
    }
    else
    {
        // For a negative value, we want to display the minus sign once.
        // Compute the absolute whole number and remainder.
        int32_t whole = (int32_t)(improper / denom);
        int32_t rem   = (int32_t)(improper % denom);
        if(whole == 0)
        {
            // When the absolute value is less than 1, keep integer part as 0
            // and let numerator carry the negative sign.
            pNumber->integer    = 0;
            pNumber->numerator  = -rem;
            pNumber->denominator = denom;
        }
        else
        {
            // Represent as -whole and a positive fraction.
            pNumber->integer    = -whole;
            pNumber->numerator  = rem;
            pNumber->denominator = denom;
        }
    }
    
    // Ensure that if there is no fractional remainder, denominator is 1.
    if(pNumber->numerator == 0)
        pNumber->denominator = 1;
}

int32_t mixed_print(sMixedNumber *pNumber)
{
    if(pNumber == NULL)
        return -1;
    
    // Create a copy and normalize it
    sMixedNumber normalized = *pNumber;
    normalize(&normalized);
    
    // Special case: zero
    if(normalized.integer == 0 && normalized.numerator == 0)
    {
        printf("0");
        return 0;
    }

    // Handle sign - only one negative sign should appear
    int32_t isNegative = 0;
    if(normalized.integer < 0 || (normalized.integer == 0 && normalized.numerator < 0))
    {
        isNegative = 1;
        printf("-");
        normalized.integer = (normalized.integer < 0) ? -normalized.integer : normalized.integer;
        normalized.numerator = (normalized.numerator < 0) ? -normalized.numerator : normalized.numerator;
    }
    
    // Print integer part if not zero
    if(normalized.integer != 0)
    {
        printf("%d", normalized.integer);
    }
    
    // Print fraction part if numerator is not zero
    if(normalized.numerator != 0)
    {
        printf("\\frac{%d}{%d}", normalized.numerator, normalized.denominator);
    }
    
    return 0;
}

int32_t mixed_input(sMixedNumber *pNumber, const char *str)
{
    if (pNumber == NULL || str == NULL)
        return -1;

    // Initialize the mixed number
    pNumber->integer = 0;
    pNumber->numerator = 0;
    pNumber->denominator = 1;
    
    // Temporary storage for parsing
    char *copy = strdup(str);
    if(copy == NULL)
        return -1;  // Memory allocation failed
    
    // Check if the input is negative
    int32_t isNegative = 0;
    char *ptr = copy;
    if(*ptr == '-')
    {
        isNegative = 1;
        ptr++;
    }
    
    // Parse different formats
    if(strstr(ptr, "\\frac{") != NULL)
    {
        // Format like "\frac{a}{b}" or "n\\frac{a}{b}"
        
        // Try to extract the integer part if it exists
        char *fracPart = strstr(ptr, "\\frac{");
        if(fracPart != ptr)
        {
            // There might be an integer part
            char intPart[32] = {0};
            strncpy(intPart, ptr, fracPart - ptr);
            pNumber->integer = atoi(intPart);
        }
        
        // Extract numerator
        char *numStart = strstr(ptr, "{") + 1;
        char *numEnd = strstr(numStart, "}");
        if(numStart && numEnd && numEnd > numStart)
        {
            char num[32] = {0};
            strncpy(num, numStart, numEnd - numStart);
            pNumber->numerator = atoi(num);
        } else
        {
            free(copy);
            return -1;
        }
        
        // Extract denominator
        char *denomStart = strstr(numEnd, "{") + 1;
        char *denomEnd = strstr(denomStart, "}");
        if(denomStart && denomEnd && denomEnd > denomStart)
        {
            char denom[32] = {0};
            strncpy(denom, denomStart, denomEnd - denomStart);
            pNumber->denominator = atoi(denom);
            if(pNumber->denominator == 0)
            {
                free(copy);
                return -1;  // Invalid denominator
            }
        } else
        {
            free(copy);
            return -1;
        }
    } else
    {
        // Simple integer format
        pNumber->integer = atoi(ptr);
    }
    
    // Apply negative sign if needed
    if(isNegative)
    {
        if(pNumber->integer != 0)
        {
            pNumber->integer = -pNumber->integer;
        }
        else
        {
            pNumber->numerator = -pNumber->numerator;
        }
    }
    
    free(copy);
    normalize(pNumber);
    return 0;
}

// Convert a mixed number to an improper fraction with proper handling of signs
int64_t toImproperNum(const sMixedNumber *num) {
    int64_t sign = (num->integer < 0 || (num->integer == 0 && num->numerator < 0)) ? -1 : 1;
    int64_t absInt = (num->integer < 0) ? -(int64_t)num->integer : num->integer;
    int64_t absNum = (num->numerator < 0) ? -(int64_t)num->numerator : num->numerator;
    
    return sign * (absInt * num->denominator + absNum);
}

int32_t mixed_add(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if(pNumber == NULL)
        return -1;
    
    // Check for invalid denominators
    if(r1.denominator == 0 || r2.denominator == 0)
        return -1;
    
    // Convert to improper fractions for addition
    int64_t num1 = toImproperNum(&r1);
    int64_t denom1 = r1.denominator;
    
    int64_t num2 = toImproperNum(&r2);
    int64_t denom2 = r2.denominator;
    
    // Add fractions: a/b + c/d = (ad + bc) / bd
    pNumber->numerator = (int32_t)(num1 * denom2 + num2 * denom1);
    pNumber->denominator = (int32_t)(denom1 * denom2);
    pNumber->integer = 0;
    
    // Normalize the result
    normalize(pNumber);
    return 0;
}

int32_t mixed_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if(pNumber == NULL)
        return -1;
    
    if(r1.denominator == 0 || r2.denominator == 0)
        return -1;
    
    int64_t num1 = toImproperNum(&r1);
    int64_t denom1 = r1.denominator;
    
    int64_t num2 = toImproperNum(&r2);
    int64_t denom2 = r2.denominator;
    
    pNumber->numerator = (int32_t)(num1 * denom2 - num2 * denom1);
    pNumber->denominator = (int32_t)(denom1 * denom2);
    pNumber->integer = 0;
    
    normalize(pNumber);
    return 0;
}

int32_t mixed_mul(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if(pNumber == NULL)
        return -1;
    
    if(r1.denominator == 0 || r2.denominator == 0)
        return -1;
    
    int64_t num1 = toImproperNum(&r1);
    int64_t denom1 = r1.denominator;
    
    int64_t num2 = toImproperNum(&r2);
    int64_t denom2 = r2.denominator;
    
    pNumber->numerator = (int32_t)(num1 * num2);
    pNumber->denominator = (int32_t)(denom1 * denom2);
    pNumber->integer = 0;
    
    normalize(pNumber);
    return 0;
}

int32_t mixed_div(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2)
{
    if(pNumber == NULL)
        return -1;
    
    if(r1.denominator == 0 || r2.denominator == 0)
        return -1;
    
    int64_t num1 = toImproperNum(&r1);
    int64_t denom1 = r1.denominator;
    
    int64_t num2 = toImproperNum(&r2);
    int64_t denom2 = r2.denominator;
    
    if(num2 == 0)
        return -1;
    
    pNumber->numerator = (int32_t)(num1 * denom2);
    pNumber->denominator = (int32_t)(denom1 * num2);
    pNumber->integer = 0;
    
    normalize(pNumber);
    return 0;
}

int32_t mixed_compare(const sMixedNumber r1, const sMixedNumber r2)
{
    if(r1.denominator == 0 || r2.denominator == 0)
        return -2;
    
    int64_t num1 = (int64_t)r1.integer * r1.denominator + r1.numerator;
    int64_t num2 = (int64_t)r2.integer * r2.denominator + r2.numerator;
    
    int64_t comparison = num1 * r2.denominator - num2 * r1.denominator;
    
    if(comparison > 0) return 1;
    if(comparison < 0) return -1;
    return 0;
}