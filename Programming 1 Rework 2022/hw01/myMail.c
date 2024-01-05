#include "myMail.h"

void printLine(uint8_t input) // print lines for the format, may be called if needed
{
    input = 1 ? lineType = '=' : lineType = '-';
    for ( uint8_t i = 0 ; i < _LINENUM_ ; ++i ) printf("%c", lineType);
    printf("\n");
}

uint8_t email_Check(char (*email)[])
{
    uint16_t i = 0;
    for( ; i < strlen((*email)) ; ++i)
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
            return _LOCAL_INVALID_;
            break;
        } 
    }
    ++i;
    char domain[UCHAR_MAX - 1];
    for( ; i < strlen((*email)) ; ++i)
    {
        domain[i] = (*email)[i];
    }

    

}

uint8_t subject_Check( char (*subject)[] )
{

}

uint8_t content_Check( double content )
{

}

void assignment( uint8_t email, uint8_t subject, uint8_t content )
{

}