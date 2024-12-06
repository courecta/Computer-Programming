#include <stdio.h>
#include <string.h>

// Ancient Cipher
bool isAncientCipher(char *s1, char *s2);
void sort(int arr[], int n);

int main(){
    char S1[101], S2[101];

    while (scanf("%s", S1) == 1)
    {
        scanf("%s", S2);
        if(isAncientCipher(S1, S2)) printf("YES\n");
        else printf("NO\n");
    }
}

bool isAncientCipher(char *s1, char *s2)
{
    constexpr int ALPHABET_SIZE = 26;
    int freqs1[ALPHABET_SIZE] = {0};
    int freqs2[ALPHABET_SIZE] = {0};

    for (int i = 0; s1[i] != '\0'; i++)
    {
        freqs1[s1[i] - 'A']++;
        freqs2[s2[i] - 'A']++;
    }

    sort(freqs1, ALPHABET_SIZE);
    sort(freqs2, ALPHABET_SIZE);

    for (int i = 0; i < ALPHABET_SIZE; i++) if(freqs1[i] != freqs2[i]) return 0;
    return 1;
}

void sort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}