#include <stdio.h>
#include <string.h>

// Cool Word
int isUnique(int occurrences[], int size);

int main(){
    int n = 0, cases = 1, cool = 0, ans = 0;

    while(scanf("%d", &n) == 1)
    {
        ans = 0;
        for(int i = 0 ; i < n ; i++)
        {
            char str[31] = { };
            scanf("%30s", str);
            int occurrences[26] = {0};
            int size = strlen(str);
            for(int j = 0 ; j < size ; j++)
            {
                occurrences[str[j] - 'a']++;
            }
            if(isUnique(occurrences, 26) && size != 1) ans++;
        }
        printf("Case %d: %d\n", cases++, ans);
    }    

}

int isUnique(int occurrences[], int size)
{
    for(int i = 0 ; i < size ; i++)
    {
        for(int j = i + 1 ; j < size ; j++)
        {
            if(occurrences[i] == occurrences[j] && occurrences[i] != 0) return 0;
        }
    }
    return 1;
}