#include "printf.h"

void format_exponent(int exponent, char *exp_buffer, int uppercase)
{
    char sign = (exponent < 0) ? '-' : '+';
    int exp_abs = (exponent < 0) ? -exponent : exponent;

    // Convert exponent to string without using stdlib
    char digits[20];
    int len = int_to_str(exp_abs, digits, -1, 0, 0);

    // Pad with leading zeros if needed (minimum 2 digits)
    if(len < 2)
    {
        // Shift digits to the right and pad with zeros
        for(int i = len; i >= 0; i--)
        {
            digits[i + (2 - len)] = digits[i];
        }
        for(int i = 0; i < 2 - len; i++)
        {
            digits[i] = '0';
        }
        len = 2;
    }

    // Build the exponent part (e.g., "e+03")
    exp_buffer[0] = uppercase ? 'E' : 'e';
    exp_buffer[1] = sign;
    for(int i = 0; i < len; i++)
    {
        exp_buffer[2 + i] = digits[i];
    }
    exp_buffer[2 + len] = '\0';
}

int float_to_exp_str(double num, char *buffer, int precision, int uppercase) {
    // Handle special cases (nan, inf)
    if(isnan(num) || isinf(num))
    {
        if(isnan(num))
        {
            buffer[0] = uppercase ? 'N' : 'n';
            buffer[1] = uppercase ? 'A' : 'a';
            buffer[2] = uppercase ? 'N' : 'n';
            return 3;
        }
        if(isinf(num))
        {
            if(num < 0)
            {
                buffer[0] = '-';
                buffer[1] = uppercase ? 'I' : 'i';
                buffer[2] = uppercase ? 'N' : 'n';
                buffer[3] = uppercase ? 'F' : 'f';
                return 4;
            } else
            {
                buffer[0] = uppercase ? 'I' : 'i';
                buffer[1] = uppercase ? 'N' : 'n';
                buffer[2] = uppercase ? 'F' : 'f';
                return 3;
            }
        }
    }

    if(precision == -1) precision = 6;  // Default precision
    
    // Handle zero specially
    if(num == 0.0)
    {
        buffer[0] = '0';
        buffer[1] = '.';
        for(int i = 0; i < precision; i++) buffer[2 + i] = '0';
        char exp[6];
        format_exponent(0, exp, uppercase);
        strcat(buffer + 2 + precision, exp);
        return 2 + precision + strlen(exp);
    }

    // Get sign
    int sign = (num < 0);
    num = fabs(num);

    // Calculate exponent and normalize
    int exp = (int)floor(log10(num));
    double normalized = num / pow(10.0, exp);

    // Convert normalized number to string with precision
    char sig_str[50];
    float_to_str(normalized, sig_str, precision);

    // Build final string
    int pos = 0;
    if(sign) buffer[pos++] = '-';
    
    // Copy significant digits
    for(int i = 0; sig_str[i]; i++)
    {
        if(sig_str[i] != '-') buffer[pos++] = sig_str[i];
    }

    // Add exponent
    char exp_str[10];
    format_exponent(exp, exp_str, uppercase);
    strcpy(buffer + pos, exp_str);
    
    return pos + strlen(exp_str);
}

// Fix octal conversion
int uint_to_oct_str(unsigned int num, char *buffer)
{
    if(num == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return 1;
    }

    int i = 0;
    unsigned int temp = num;
    while(temp > 0) {
        buffer[i++] = '0' + (temp & 7);
        temp >>= 3;
    }
    buffer[i] = '\0';
    reverse(buffer, i);
    return i;
}

int uint_to_hex_str(unsigned int num, char *buffer, int uppercase)
{
    const char *hex_digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int i = 0;
    if (num == 0)
    {
        buffer[i++] = '0';
    } else
    {
        while(num > 0)
        {
            buffer[i++] = hex_digits[num % 16];
            num /= 16;
        }
    }
    buffer[i] = '\0';
    reverse(buffer, i);
    return i;
}

int uint_to_str(unsigned int num, char *buffer)
{
    int i = 0;
    if(num == 0)
    {
        buffer[i++] = '0';
    } else
    {
        while(num > 0)
        {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
    }
    buffer[i] = '\0';
    reverse(buffer, i);
    return i;
}

void reverse(char *str, int length)
{
    int start = 0;
    int end = length - 1;
    while(start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int int_to_str(int num, char *buffer, int precision, int force_sign, int space_sign)
{
    int i = 0, is_negative = 0;
    unsigned int unum;

    if(num < 0)
    {
        is_negative = 1;
        unum = (unsigned int)(-num);
    } else
    {
        unum = (unsigned int)num;
    }

    // Calculate digits and leading zeros
    int digits = 0;
    unsigned int temp = unum;
    do
    {
        temp /= 10;
        digits++;
    } while(temp > 0);

    int leading_zeros = (precision > digits) ? (precision - digits) : 0;
    int total_digits = leading_zeros + (unum == 0 && precision != 0 ? 1 : digits);

    // Process digits (store in reverse order)
    char digits_buffer[50];
    int digits_i = 0;
    if(unum == 0 && precision != 0)
    {
        digits_buffer[digits_i++] = '0';
    } else
    {
        while(unum > 0)
        {
            digits_buffer[digits_i++] = (unum % 10) + '0';
            unum /= 10;
        }
    }
    // Add leading zeros
    for(int j = 0; j < leading_zeros; j++)
    {
        digits_buffer[digits_i++] = '0';
    }
    reverse(digits_buffer, digits_i); // Correct digit order

    // Add sign
    if(is_negative)
    {
        buffer[i++] = '-';
    } else if(force_sign)
    {
        buffer[i++] = '+';
    } else if(space_sign)
    {
        buffer[i++] = ' ';
    }

    // Copy digits to buffer
    memcpy(buffer + i, digits_buffer, digits_i);
    i += digits_i;

    buffer[i] = '\0';
    return i;
}

int float_to_str(double num, char *buffer, int precision) {
    // Handle NaN and Infinity
    if(isnan(num))
    {
        strcpy(buffer, "nan");
        return 3;
    }
    if(isinf(num))
    {
        strcpy(buffer, num < 0 ? "-inf" : "inf");
        return num < 0 ? 4 : 3;
    }

    int is_negative = (num < 0);
    if(is_negative) num = -num;

    // Calculate rounding factor (e.g., 0.5 / 10^precision for proper rounding)
    double rounding = 0.5 / pow(10, precision);
    num += rounding;

    // Split into integer and fractional parts
    long long integer_part = (long long)num;
    double fractional_part = num - integer_part;

    // Convert integer part to string
    char int_buffer[50];
    int int_len = int_to_str(integer_part, int_buffer, -1, 0, 0);

    // Convert fractional part to string with precision
    char frac_buffer[50] = {0};
    if(precision > 0)
    {
        unsigned long long frac = (unsigned long long)(fractional_part * pow(10, precision));
        int frac_len = int_to_str(frac, frac_buffer, precision, 0, 0);

        // Pad with leading zeros if fractional part is shorter than precision
        if(frac_len < precision)
        {
            memmove(frac_buffer + (precision - frac_len), frac_buffer, frac_len + 1);
            memset(frac_buffer, '0', precision - frac_len);
        }
    }

    // Build the final string
    int total_len = 0;
    if(is_negative)
    {
        buffer[total_len++] = '-';
    }
    strcpy(buffer + total_len, int_buffer);
    total_len += int_len;

    if(precision > 0)
    {
        buffer[total_len++] = '.';
        strncpy(buffer + total_len, frac_buffer, precision);
        total_len += precision;
    }
    buffer[total_len] = '\0';

    return total_len;
}

int my_fputc(char c)
{
    ssize_t bytes_written = write(1, &c, 1); // 1 = stdout file descriptor
    return (bytes_written == 1) ? 1 : EOF;
}

int my_fputs(const char *str)
{
    int total = 0;
    for(size_t i = 0; str[i] != '\0'; i++)
    {
        int ret = my_fputc(str[i]);
        if(ret == EOF) return EOF;
        total += ret;
    }
    return total;
}