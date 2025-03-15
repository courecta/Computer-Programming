#include "printf.h"

int main(void){
    my_printf("%.2f\n", 3.1415);      // 3.14
    my_printf("%.0f\n", 2.6);         // 3
    my_printf("%.3f\n", -0.1234);     // -0.123
    my_printf("%f\n", 0.0 / 0.0);     // nan
    my_printf("%f\n", 1.0 / 0.0);     // inf
    my_printf("%f\n", -1.0 / 0.0);    // -inf

    my_printf("String: |%-10s| |%10s|\n", "left", "right"); // |left      | |     right|
    my_printf("Integer: |%05d| |%5d| |%-5d|\n", 42, -42, 42); // |00042| |  -42| |42   |
    my_printf("Precision: |%.5d| |%.2u|\n", 42, 1234); // |00042| |1234|

    // Floating-point width/precision
    my_printf("Float: |%10.2f| |%-10.2f| |%010.2f|\n", 3.1415, -3.1415, 3.1415);
    // Output: |      3.14| |-3.14      | |000003.14|

    // Sign flags
    my_printf("Signs: |%+d| |% d| |%+05d|\n", 42, 42, 42);
    // Output: |+42| | 42| |+0042|

    my_printf("Scientific: %e | %E\n", 3141.5, 0.0031415);
    // Output: 3.141500e+03 | 3.141500E-03

    // Hex/Octal with #
    my_printf("Hex: %#x | Octal: %#o\n", 255, 63);
    // Output: 0xff | Octal: 077

    my_printf("Dynamic: |%*.*f|\n", 10, 2, 3.1415);
    // Output: |      3.14|

    // Octal with proper alternate form
    my_printf("Hex: %#x | Octal: %#o\n", 255, 63);
    // Output: 0xff | Octal: 077

    int array[] = {1, 2, 3, 4, 5};
    int size = sizeof(array) / sizeof(array[0]);
    for(int i = 0; i < size; i++) my_printf("%d ", array[i]);
    my_printf("\n");

    return 0;
}

int my_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int count = 0;

    for(int i = 0; format[i] != '\0'; i++)
    {
        if(format[i] == '%')
        {
            i++; // iterate past the % sign
            // Parse flags
            int left_justify = 0, zero_pad = 0, precision = -1, width = 0;
            int force_sign = 0, space_sign = 0, alt_form = 0;
            // flag parsing loop:
            while (format[i] == '-' || format[i] == '+' || format[i] == ' ' || format[i] == '0' || format[i] == '#')
            {
                if(format[i] == '+')
                {
                    force_sign = 1;
                } else if(format[i] == ' ')
                {
                    space_sign = 1;
                } else if(format[i] == '#')
                {
                    alt_form = 1;
                }

                if(format[i] == '-')
                {
                    left_justify = 1;
                } else if(format[i] == '0')
                {
                    zero_pad = 1;
                }
                i++;
            }
            // Parse width
            if(format[i] == '*')
            {
                width = va_arg(args, int);
                i++;
                if(width < 0)
                {
                    width = -width;
                    left_justify = 1;
                }
            } else if(format[i] >= '0' && format[i] <= '9')
            {
                width = 0;
                while(format[i] >= '0' && format[i] <= '9')
                {
                    width = width * 10 + (format[i] - '0');
                    i++;
                }
            }
            // Parse precision
            if(format[i] == '.')
            {
                i++;
                if(format[i] == '*')
                {
                    precision = va_arg(args, int);
                    i++;
                } else
                {
                precision = 0;
                while(format[i] >= '0' && format[i] <= '9')
                    {
                        precision = precision * 10 + (format[i] - '0');
                        i++;
                    }
                }
            }
            char specifier = format[i];
            switch(specifier)
            {
                case 's':
                {
                    const char *str_arg = va_arg(args, const char*);
                    if(!str_arg) str_arg = "(null)";
                    int str_len = 0;
                    // Calculate length (respect precision as max characters)
                    while(str_arg[str_len] != '\0' && (precision == -1 || str_len < precision)) str_len++;
                    // Calculate padding
                    int padding = width - str_len;
                    if(padding < 0) padding = 0;
                    // Left padding (right-aligned)
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if(my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    // Print the string (up to str_len)
                    for(int j = 0; j < str_len; j++)
                    {
                        if(my_fputc(str_arg[j]) == EOF) return -1;
                        count++;
                    }
                    // Right padding (left-aligned)
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if(my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                case 'd':
                case 'i':
                case 'u':
                {
                    int num = (specifier == 'u') ? va_arg(args, unsigned int) : va_arg(args, int);
                    char buffer[50];
                    int len = int_to_str(num, buffer, precision, force_sign, space_sign);
                    // Determine padding and zero-pad
                    int padding = width - len;
                    if(padding < 0) padding = 0;
                    char fill = (zero_pad && !left_justify && precision == -1) ? '0' : ' ';
                    // Sign handling (for negative/positive)
                    if(fill == '0' && (buffer[0] == '-' || buffer[0] == '+'))
                    {
                        my_fputc(buffer[0]);
                        count++;
                        len--;
                        padding = width - (len + 1);
                        memmove(buffer, buffer + 1, len + 1); // Remove sign from buffer
                    }
                    // Left padding
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(fill) == EOF) return -1;
                            count++;
                        }
                    }
                    // Print the number
                    if(my_fputs(buffer) == EOF) return -1;
                    count += len;
                    // Right padding
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                case 'f':
                {
                    double num = va_arg(args, double);
                    char buffer[50];
                    int len = float_to_str(num, buffer, (precision == -1) ? 6 : precision);
                    int padding = width - len;
                    if(padding < 0) padding = 0;
                    char fill = (zero_pad && !left_justify) ? '0' : ' ';
                
                    // Handle zero padding with sign
                    if(fill == '0' && (buffer[0] == '-' || buffer[0] == '+'))
                    {
                        my_fputc(buffer[0]);
                        count++;
                        memmove(buffer, buffer + 1, len);
                        len--;
                        padding--;
                    }
                
                    // Left padding
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if(my_fputc(fill) == EOF) return -1;
                            count++;
                        }
                    }

                    // Print the float
                    if(my_fputs(buffer) == EOF) return -1;
                    count += len;

                    // Right padding
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if(my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                case 'x':
                case 'X':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[20];
                    int len = uint_to_hex_str(num, buffer, (specifier == 'X'));
                    int prefix_len = 0;
                    if(alt_form && num != 0)
                    {
                        // Reverse to original order, add "0x", then re-reverse digits
                        reverse(buffer, len);
                        memmove(buffer + 2, buffer, len);
                        buffer[0] = '0';
                        buffer[1] = (specifier == 'X') ? 'X' : 'x';
                        len += 2;
                        reverse(buffer + 2, len - 2); // Re-reverse the hex digits
                    }
                    
                    // Determine padding and zero-pad
                    int padding = width - len;
                    if(padding < 0) padding = 0;
                    char fill = (zero_pad && !left_justify && precision == -1) ? '0' : ' ';
                    // Sign handling (for negative/positive)
                    if(fill == '0' && (buffer[0] == '-' || buffer[0] == '+'))
                    {
                        my_fputc(buffer[0]);
                        count++;
                        len--;
                        padding--;
                        memmove(buffer, buffer + 1, len + 1); // Remove sign from buffer
                    }
                    // Left padding
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(fill) == EOF) return -1;
                            count++;
                        }
                    }
                    // Print the number
                    if(my_fputs(buffer) == EOF) return -1;
                    count += len;
                    // Right padding
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                case 'o':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[20];
                    int len = uint_to_oct_str(num, buffer);
                    if(alt_form && num != 0)
                    {
                        reverse(buffer, len); // Reverse to original order
                        memmove(buffer + 1, buffer, len);
                        buffer[0] = '0';
                        len += 1;
                        reverse(buffer + 1, len - 1); // Re-reverse the octal digits
                    }
                    
                    // Determine padding and zero-pad
                    int padding = width - len;
                    if(padding < 0) padding = 0;
                    char fill = (zero_pad && !left_justify && precision == -1) ? '0' : ' ';
                    // Sign handling (for negative/positive)
                    if(fill == '0' && (buffer[0] == '-' || buffer[0] == '+'))
                    {
                        my_fputc(buffer[0]);
                        count++;
                        len--;
                        padding--;
                        memmove(buffer, buffer + 1, len + 1); // Remove sign from buffer
                    }
                    // Left padding
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(fill) == EOF) return -1;
                            count++;
                        }
                    }
                    // Print the number
                    if(my_fputs(buffer) == EOF) return -1;
                    count += len;
                    // Right padding
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                case 'e':
                case 'E':
                {
                    double num = va_arg(args, double);
                    char buffer[50];
                    int len = float_to_exp_str(num, buffer, precision, (specifier == 'E'));
                    // Handle padding/alignment (similar to %f)
                    int padding = width - len;
                    if(padding < 0) padding = 0;
                    char fill = (zero_pad && !left_justify && precision == -1) ? '0' : ' ';

                    // Left padding
                    if(!left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(fill) == EOF) return -1;
                            count++;
                        }
                    }

                    // Print the number
                    if(my_fputs(buffer) == EOF) return -1;
                    count += len;

                    // Right padding
                    if(left_justify)
                    {
                        for(int j = 0; j < padding; j++)
                        {
                            if (my_fputc(' ') == EOF) return -1;
                            count++;
                        }
                    }
                    break;
                }

                default:
                {
                    if(my_fputc('%') == -1 || my_fputc(specifier) == -1)
                    {
                        va_end(args);
                        return -1;
                    }
                    count += 2;
                }
            }

        } else 
        {
            if(my_fputc(format[i]) == EOF)
            {
                va_end(args);
                return -1;
            }
            count++;
        }
    }

    va_end(args);
    return count;
}