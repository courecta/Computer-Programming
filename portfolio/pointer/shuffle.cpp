#include <iostream>
#include <ctime>
#define DECK_SIZE 52

// Fisher-Yates / Knuth shuffle algorithm

void shuffle(int* deck, int size);
void printDeck(int deck[], int size);
void swap(int* a, int* b);

int main(){
    srand(time(NULL));
    int deck[DECK_SIZE];
    for(int i = 0 ; i < DECK_SIZE ; i++) deck[i] = i;
    int size = sizeof(deck)/sizeof(deck[0]);
    
    std::cout << "Original Deck top 13 cards: ";
    printDeck(deck, 13);
    std::cout << "Shuffled Deck top 13 cards: ";
    shuffle(deck, size);
    printDeck(deck, 13);
    std::cout << std::endl;

    return 0;
}

void shuffle(int* array, int size)
{
    int j;
    for(int i = size - 1 ; i > 0 ; i--)
    {
        j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}


void printDeck(int deck[], int size)
{
    for(int i = 0 ; i < size ; i++) std::cout << deck[i] << " ";
    std::cout << std::endl;
}

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}