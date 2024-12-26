#include <stdio.h>

int main() {

    int input = 0;
    double q1 = 1.0, q2 = 0.1, q3 = 0.2, q4 = 0.3, q5 = 0.4, q6 = 0.0, current_state = q1;
    while(1)
    {
        scanf("%d", &input);
        if(input == -1) break;
        if(current_state == q1)
        {
            if(input == 0) current_state = q2;
            else if (input == 1) current_state = q3;
            else current_state = q6;
        }
        else if(current_state == q2)
        {
            if(input == 1) continue;
            else if(input == 2 || input == 3) current_state = q4;
            else if(input == 4) current_state = q1;
            else current_state = q6;
        }
        else if(current_state == q3)
        {
            if(input == 0) continue;
            else if(input == 4) current_state = q5;
            else if(input == 2) current_state = q1;
            else current_state = q6;
        }
        else if(current_state == q4)
        {
            if(input == 2 || input == 3) continue;
            else if(input == 4) current_state = q1;
            else current_state = q6;
        }
        else if(current_state == q5)
        {
            if(input == 4) continue;
            else if(input == 2) current_state = q1;
            else current_state = q6;
        }
        else if(current_state == q6) continue;
    }

    if(current_state == q1) printf("this sequence is accepted.\n");
    else printf("this sequence is rejected.\n");

    return 0;
}