#include "test.h"

uint8_t email_Check(char (*email)[]);

int main()
{
    char email[UCHAR_MAX - 1];
    printf("Please enter the sender address      > ");
    fgets(email, UCHAR_MAX - 1, stdin);
    uint8_t emailCheck_Result = email_Check(&email);

}

uint8_t email_Check(char (*email)[])
{

    uint16_t i = 0;
    for( ; i < UCHAR_MAX - 1 ; ++i)
    {
        uint8_t tmp = (*email)[i];
        switch (tmp)
        {
            case 48 ... 57:
            continue;
            break;

            case 65 ... 90:
            continue;
            break;

            case 97 ... 122:
            continue;
            break;

            case 64:
            break;

            default:
            return 1;
            break;
        } 
    }

    

}