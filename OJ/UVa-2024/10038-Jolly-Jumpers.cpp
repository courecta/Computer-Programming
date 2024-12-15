#include <stdio.h>

void min_sort(int *array, int n);

int main()
{
    int n = 0;
    while(scanf("%d", &n) == 1)
    {
        int array[n] = { }, diff[n-1] = { }, jolly = 1;
        for(int i = 0 ; i < n ; i++)
            scanf("%d", &array[i]);
        if(n == 1)
        {
            printf("Jolly\n");
            continue;
        }
        for(int i = 0 ; i < n - 1 ; i++) diff[i] = (array[i] > array[i+1]) ? array[i] - array[i+1] : array[i+1] - array[i];
        min_sort(diff, n - 1);
        for(int i = 0 ; i < n - 1 ; i++)
        {
            if(diff[i] != i + 1)
            {
                jolly = 0;
                break;
            }
        }
        if(jolly)
            printf("Jolly\n");
        else
            printf("Not jolly\n");
    }
}

void min_sort(int *array, int n)
{
    int temp = 0;
    for(int i = 0 ; i < n - 1 ; i++)
    {
        for(int j = i + 1 ; j < n ; j++)
        {
            if(array[i] > array[j])
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}