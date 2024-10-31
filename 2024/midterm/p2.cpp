#include <stdio.h>

// double pointer approach using i and j

int main(){

    // initialize variables
    int input = 0;
    int div = 10;
    int temp = 0;
    int temp_div = 0;
    int i = 0;
    int j = 0;
    int array[9];

    // sentinel input loop
    while(1)
    {
        // take user input
        scanf("%d", &input);

        // sentinel check
        if(input == -1) break;

        if(input < 10)
        {
            printf("YES\n");
            continue;
        }

        // clean array for every loop use
        for(int k = 0 ; k < 9 ; k++)
        {
            array[k] = 0;
        }

        // set starting variables for judgement
        i = 0;
        j = 0;
        div = 10;
        temp_div = 1;
        while(div < input * 10)
        {
            temp = input % div;
            array[i] = temp / temp_div;
            div *= 10;
            temp_div *= 10;
            i++;
        }
        // set pointer
        while(array[j] != 0)
        {
            j++;
        }
        j--;
        i = 0;
        int end = j;
        while(1)
        {
            if(array[i] != array[j])
            {
                printf("NO\n");
                break;
            }
            if((i == j) || (i == end && j == 0))
            {
                printf("YES\n");
                break;
            }
            i++;
            j--;
        }
    }

    return 0;
}