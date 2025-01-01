#include <stdio.h>

// CTRL + d for EOF signal

void shuffle(int *deck[]);
void print(int *deck[]);

int main()
{
    int card[10000];
    int *deck[10000];
    int index = 0;
    while(scanf("%d", &(card[index])) != EOF)
    {
      deck[index] = &(card[index]);
      index++;
    }
    deck[index] = NULL;
    shuffle(deck);
    print(deck);  
    return 0;
}

void shuffle(int *deck[])
{
    int size = 0;
    while(deck[size] != NULL) size++;
    if(deck[0] == NULL || deck[1] == NULL) return;
    int* left_hand[5000] = {0};
    int* right_hand[5000] = {0};
    int j = 0, k = (size % 2) ? ((size / 2) + 1) : (size / 2);
    left_hand[0] = deck[j];
    right_hand[0] = deck[k];
    for(int i = 0; j < k; i++)
    {
        left_hand[i] = deck[j++];
    }
    for(int i = 0; k < size; i++)
    {
        right_hand[i] = deck[k++];
    }
    j = 0;
    k = 0;
    for(int i = 0; ;)
    {
        deck[i] = left_hand[j];
        i++;
        j++;
        if(deck[i] == NULL) break;
        deck[i] = right_hand[k];
        i++;
        k++;
        if(deck[i] == NULL) break;
    }
}

void print(int *deck[])
{
    for(int i = 0; deck[i] != NULL; i++)
    {
        printf("%d", *(deck[i]));
        if(deck[i + 1] != NULL) printf(" ");
    }
    printf("\n");
}

/*
void shuffle ( int *deck[] )
{
	int i = 0;
	int buf[10000];
	while ( deck[i] ) {
		buf[i] = *deck[i];
		i++;
	}

	int sec_s = i / 2 + i % 2;
	int fir = 0, sec = sec_s, deck_p = 0;

	while ( deck[deck_p] ) {
		*deck[deck_p++] = buf[fir++];
		if ( buf[sec] )
			*deck[deck_p++] = buf[sec++];
	}
}

void print ( int *deck[] )
{
	int i = 0;
	while ( deck[i] ) {
		if ( i ) printf ( " " );
		printf ( "%d", *deck[i++] );
	}
}
*/