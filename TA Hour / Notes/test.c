#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

     
int main()    
{    
    //Initialize array     
    int32_t array[6] = {0};
    array[ rand() % 6 ]++;
        
    //Calculate length of array arr    
    int length = sizeof(array)/sizeof(array[0]);    
        
    //Array fr will store frequencies of element    
    int fr[length];    
    int visited = -1;    
        
    for(int i = 0; i < length; i++){    
        int count = 1;    
        for(int j = i+1; j < length; j++){    
            if(array[i] == array[j]){    
                count++;    
                //To avoid counting same element again    
                fr[j] = visited;    
            }    
        }    
        if(fr[i] != visited)    
            fr[i] = count;    
    }    
        
    //Displays the frequency of each element present in array    
    printf("---------------------\n");    
    printf(" Element | Frequency\n");    
    printf("---------------------\n");    
    for(int i = 0; i < length; i++){    
        if(fr[i] != visited){    
            printf("    %d", array[i]);    
            printf("    |  ");    
            printf("  %d\n", fr[i]);    
        }    
    }    
    printf("---------------------\n");    
    return 0;    
}