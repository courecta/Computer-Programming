#include <stdio.h>

int main(){

    int counter = 0, grade = 0, max = 0, min = 0;
    double total = 0.0, average = 0.0, pass_rate = 0.0;

    printf("Please input a score (type -1 to end)...> ");
    scanf("%d", &grade);
    max = grade;
    min = grade;

    while(grade != -1)
    {
        total += grade;
        counter++;

        if(max <= grade) max = grade;
        if(min >= grade) min = grade;
        if(grade >= 60) pass_rate += 1.0;

        printf("Please input a score (type -1 to end)...> ");
        scanf("%d", &grade);
    }

    if(counter != 0)
    {
        average = (double)total / (double)counter;
        pass_rate /= (double)counter;
        printf("\nAnalysis\n---------------------\nMax: %d\nMin: %d\nAvg: %.2lf\nPass rate: %.0lf%%\n", max, min, average, (pass_rate * 100.0));

    } else printf("No grades were submitted.\n");

    return 0;
}