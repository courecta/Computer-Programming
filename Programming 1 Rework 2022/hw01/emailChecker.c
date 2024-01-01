#include "myMail.h"

int main(){

    char email[UCHAR_MAX - 1];
    printf("Please enter the sender address      > ");
    fgets(email, UCHAR_MAX - 1, stdin);
    uint8_t emailCheck_Result = email_Check(&email);
    
    char subject[UCHAR_MAX]
    printf("Please enter the email subject       > ");
    fgets(subject, UCHAR_MAX, stdin);
    uint8_t subjectCheck_Result = subject_Check(&subject);
    
    double content = 0;
    printf("Please enter the email content score > ");
    scanf("%lf", &content);
    uint8_t contentCheck_Result = content_Check(content);

    printLine(1);

    assignment(emailCheck_Result, subjectCheck_Result, contentCheck_Result);

    return 0;
}