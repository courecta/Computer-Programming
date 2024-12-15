#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52
#define SUITS 4
#define RANKS 13

typedef struct Card
{
    char* suit;
    char* rank;
} Card;

void initailizeDeck(Card* deck, char* suits[], char* ranks[]);
void shuffleDeck(Card* deck);
void printDeck(const Card* deck, const int size);

int main(){
    // initialize random seed
    srand(time(NULL));

    // initialize cards
    char* suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    char* ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    Card deck[DECK_SIZE];
    
    // initialize deck
    initailizeDeck(deck, suits, ranks);
    printf("Original Deck top 13 cards: \n");
    printDeck(deck, 13);

    // shuffle deck
    shuffleDeck(deck);
    printf("Shuffled Deck top 13 cards: \n");
    printDeck(deck, 13);

    return 0;
}

void initailizeDeck(Card* deck, char* suits[], char* ranks[])
{
    int i = 0;
    for(int j = 0 ; j < SUITS ; j++)
    {
        for(int k = 0 ; k < RANKS ; k++)
        {
            deck[i].suit = suits[j];
            deck[i].rank = ranks[k];
            i++;
        }
    }
}

void shuffleDeck(Card* deck)
{
    for(int i = DECK_SIZE - 1 ; i > 0 ; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}
void printDeck(const Card* deck, const int size)
{
    for(int i = 0 ; i < size ; i++)
        printf("%s of %s\n", deck[i].rank, deck[i].suit);
    printf("\n");
}