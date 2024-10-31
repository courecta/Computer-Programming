#include <stdio.h>

int rule_1_follow(int input);
int rule_2_follow(int input);
int rule_3_follow(int input);

int main(){

    int input = 0;
    int rule_1 = 0, rule_2 = 0, rule_3 = 0;

    while(1)
    {
        scanf("%d", &input);
        rule_1 = rule_1_follow(input);
        rule_2 = rule_2_follow(input);
        rule_3 = rule_3_follow(input);
        if(input == -1) break;
        
    }

    if(!((rule_1) && (rule_2) && (rule_3)))
    {
        if(!(rule_1)) printf("Rule 1 ");
        if(!(rule_2)) printf("Rule 2 ");
        if(!(rule_3)) printf("Rule 3 ");

        printf("not followed!\n");
    } else printf("SUCCESS!\n");

    return 0;
}

int rule_1_follow(int input)
{
    static double q1 = 0.1;
    static double q2 = 0.2;
    static double q3 = 0.3;
    static double q4 = 0.4;
    static double q5 = 0.5;
    static double q6 = 0.6;
    static double q7 = 1.0;
    static double current_state = 0.1;

    if(input == -1)
    {
        if((int)current_state) return 1;
        else return 0;
    }

    if(current_state == q1)
    {
        if(input == 1) current_state = q3;
        else if(input == 3) current_state = q2;
    }
    else if(current_state == q2)
    {
        if(input == 3) current_state = q2;
        if(input == 2 || input == 1) current_state = q3;
        else current_state = q1;
    }
    else if(current_state == q3)
    {
        if(input == 6) current_state = q4;
    }
    else if(current_state == q4)
    {
        if(input == 6) current_state = q4;
        else if(input == 5) current_state = q5;
        else current_state = q3;
    }
    else if(current_state == q5)
    {
        if(input == 9) current_state = q7;
        else if(input == 7) current_state = q6;
    }
    else if(current_state == q6)
    {
        if(input == 7) current_state = q6;
        else if(input == 8 || input == 9) current_state = q7;
        else current_state = q5;
    }
    else if(current_state == q7) current_state = q7;

    return 0;
}

int rule_2_follow(int input)
{
    static double q1 = 0.1;
    static double q2 = 0.2;
    static double q3 = 0.3;
    static double q4 = 0.4;
    static double q5 = 1.0;
    static double current_state = 0.1;

    if(input == -1)
    {
        if((int)current_state) return 1;
        else return 0;
    }

    if(current_state == q1)
    {
        if(input == 7) current_state = q2;
    }
    else if(current_state == q2)
    {
        if(input == 3) current_state = q5;
        else if(input == 5) current_state = q4;
        else if(input == 7) current_state = q3;
    }
    else if(current_state == q3)
    {
        if(input == 3) current_state = q5;
        else if(input == 5) current_state = q4;
        else if(input == 7) current_state = q1;
    }
    else if(current_state == q4)
    {
        if(input == 3) current_state = q5;
        else if(input == 5) current_state = q1;
    }
    else if(current_state == q5) current_state = q5;

    return 0;
}

int rule_3_follow(int input)
{
    static double q1 = 0.1;
    static double q2 = 0.2;
    static double q3 = 0.3;
    static double q4 = 0.4;
    static double q5 = 0.5;
    static double q6 = 1.0;
    static double current_state = 0.1;

    if(input == -1)
    {
        if((int)current_state) return 1;
        else return 0;
    }

    if(current_state == q1)
    {
        if(input == 4) current_state = q2;
    }
    else if(current_state == q2)
    {
        if(input == 6) current_state = q3;
    }
    else if(current_state == q3)
    {
        if(input == 6) current_state = q4;
        else current_state = q2;
    }
    else if(current_state == q4)
    {
        if(input == 6) current_state = q5;
        else current_state = q2;
    }
    else if(current_state == q5)
    {
        if(input == 8) current_state = q6;
    }
    else if(current_state == q6) current_state = q6;

    return 0;
}