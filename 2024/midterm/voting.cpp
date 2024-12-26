#include<stdio.h>

int main()
{
    int T = 0, N = 0, max_value = 0, max_vote = 0;;
    scanf("%d%d", &T, &N);
    int values[10] = {0}, votes[10] = {0};
    for (int t = 0 ; t < T ; t++)
    {
        for(int i = 0 ; i < N ; i++)
        {
            scanf("%d", &values[i]);
            if(values[i] > max_value)
                max_value = values[i];
        }
        for(int i = 0 ; i < N ; i++)
        {
            if (values[i] == max_value)
                votes[i]++;
        }
    }
    for(int i = 0; i < N; i++)
    {
        if(votes[i] > max_vote)
            max_vote = votes[i];    
    }
    for(int i = 0; i < N; i++)
    {
        if (votes[i] == max_vote)
            printf("%d ", i + 1);
    }
}