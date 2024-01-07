#include "myMail.h"

// For the homework and problem number
static uint8_t A;
static uint8_t B;
static uint64_t length_title;

void printLine(uint8_t input) // print lines for the format, may be called if needed
{
    uint8_t lineType;
    if(input == 1) lineType = '=';
    else lineType = '-';
    for ( uint8_t i = 0 ; i < _LINENUM_ ; ++i ) printf("%c", lineType);
    printf("\n\n");
}

uint16_t getSize(char (*array)[])
{
    uint16_t i;
    for (i = 0; (*array)[i] != '\0'; ++i);
    if (i <= 0) return EXIT_FAILURE;
    return i;
}

uint8_t email_Check(char (*email)[])
{
    uint16_t localLength = getSize(email);
    uint16_t i = 0;
    bool atSign_detected = false;
    bool localInvalidity = false;

    for( ; i < localLength ; ++i)
    {
        uint8_t tmp = (uint8_t)(*email)[i];
        switch (tmp)
        {
            case 48 ... 57:
            continue;

            case 65 ... 90:
            continue;

            case 97 ... 122:
            continue;

            case 64:
            atSign_detected = true;
            break;

            default:
            localInvalidity = true;
            break;
        }
        if (atSign_detected == 1) break;
    }

    if (atSign_detected == 0) return _INVALID_EMAIL_FORMAT_;
    if (localInvalidity == 1) return _LOCAL_INVALID_;

    ++i;
    char domain[localLength];
    uint16_t j = 0;
    for( ; i < localLength ; ++i)
    {
        if ((*email)[i] == 10) break;
        domain[j] = (*email)[i];
        ++j;
    }
    domain[j] = '\0';

    if (strcmp(domain, "ntnu.edu.tw") == 0 || strcmp(domain, "gapps.ntnu.edu.tw") == 0 || strcmp(domain, "csie.ntnu.edu.tw") == 0 || strcmp(domain, "csie.cool") == 0 )
    {
        return 0;
    }
    else return _DOMAIN_INVALID_;

}

uint8_t subject_Check( char (*subject)[] )
{
    uint16_t categoryLength = getSize(subject);

    bool general_detected = false;
    bool hwp_detected = false;
    
    char general[] = "[general]";
    char hw[] = "[hw";
    char p[] = "[p";

    char *match_general = NULL;
    char *match_hw = NULL;
    char *match_p = NULL;

    match_general = strstr((*subject), general);
    match_hw = strstr((*subject), hw);
    match_p = strstr((*subject), p);

    if(match_general) general_detected = true;
    if(match_hw)
    {
        if(match_p)
        {
            uint8_t i, j;
            char A_hw[categoryLength]; 
            for( i = 0 ; (*subject)[i] != 'p' ; ++i)
            {
                A_hw[i] = (*subject)[3 + i];
            }
            A_hw[i] = '\0';

            char B_p[categoryLength];
            for( j = 0 ; (*subject)[j] != ']' ; ++j)
            {
                B_p[j] = (*subject)[7 + j];
            }
            B_p[j] = '\0';
            
            char *temp1;
            char *temp2;
            int64_t A_convert = strtol(A_hw, &temp1, 10);
            int64_t B_convert = strtol(B_p, &temp2, 10);
            
            A = A_convert;
            B = B_convert;

            if (A < 10 && B < 10) hwp_detected = true;
        }
    }

    if (general_detected == 0 && hwp_detected == 0) return _CATEGORY_INVALID_;
    if (categoryLength <= 10) return _TITLE_EMPTY_;

    uint8_t m = 0;
    char title[UCHAR_MAX];
    for(uint8_t k = 9 ; (*subject)[k] != '\0' ; ++k)
    {
        title[m] = (*subject)[k];
        ++m;
    }
    title[m] = '\0';
    length_title = (strlen(title)) - 1;

    return 0;
    
}

uint8_t content_Check( double content )
{
    if(content < 0 || content > pow(10, 19)) return _CONTENT_OUT_OF_RANGE_;
    if(content < (length_title * pow(10, 10))) return _CONTENT_TOO_LOW_;
    return 0;
}

void assignment( uint8_t email, uint8_t subject, uint8_t content )
{
    printf("Sender Address Test : ");

    bool emailPass = false;
    switch (email)
    {
        case 0:
        printf("Passed");
        emailPass = true;
        break;

        case _LOCAL_INVALID_:
        printf("Failed, local part is invalid");
        break;

        case _DOMAIN_INVALID_:
        printf("Failed, domain is unauthorized");
        break;

        case _INVALID_EMAIL_FORMAT_:
        printf("Failed, email format is invalid");
        break;

        default:
        break;

    }
    printf("\n");

    printf("Email Subject Test  : ");

    bool subjectPass = false;
    if(emailPass == 1)
    {
        switch (subject)
        {

        case 0:
        printf("Passed");
        subjectPass = true;
        break;

        case _CATEGORY_INVALID_:
        printf("Failed, no category or category is invalid");
        break;

        case _TITLE_EMPTY_:
        printf("Failed, title is empty");
        break;

        default:
        break;

        }
    }
    else printf("-");
    printf("\n");

    printf("Email Content Test  : ");

    if(subjectPass == 1)
    {
        switch (content)
        {
        
        case 0:
        printf("Passed");
        break;

        case _CONTENT_OUT_OF_RANGE_:
        printf("Failed, content score is out of range");
        break;
        
        case _CONTENT_TOO_LOW_:
        printf("Failed, content score is too low");
        break;

        default:
        break;

        }
    } else printf("-");
    printf("\n");

    printLine(0);

    if(email != 0 || subject != 0 || content != 0)
    {
        printf("Rejected\n");
        return;
    }

    printf("Assigned to ");
    if (A != 0 && B != 0)
    {
        uint8_t C = (A * B) % 5;
        switch (C) 
        {
            case 0:
            printf("Kaname Madoka\n");
            return;
            break;

            case 1:
            printf("Akemi Homura\n");
            return;
            break;

            case 2:
            printf("Miki Sayaka\n");
            return;
            break;

            case 3:
            printf("Tomoe Mami\n");
            return;
            break;

            case 4:
            printf("Sakura Kyoko\n");
            return;
            break;
        }
    } else printf("TA QB\n");

    return;
}