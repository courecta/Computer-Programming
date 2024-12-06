#include <stdio.h>
#include <string.h>

constexpr int MAX_NAME_LENGTH = 12;

struct person{
    char name[MAX_NAME_LENGTH + 1];
    int balance;
};

int findPersonIndex(struct person group[], const char* name, int size);

int main() {
    int n = 0, t = 0;
    while(scanf("%d", &n) != EOF)
    {
        struct person group[10];
        for(int i = 0; i < n; i++)
        {
            scanf("%s", group[i].name);
            group[i].balance = 0;
        }
        for(int i = 0 ; i < n ; i++)
        {
            char giver[MAX_NAME_LENGTH + 1] = {0}, receiver[MAX_NAME_LENGTH + 1] = {0};
            int price, num_of_receivers;
            scanf("%s", giver);
            int giver_index = findPersonIndex(group, giver, n);

            scanf("%d %d", &price, &num_of_receivers);
            int each_given = (num_of_receivers == 0) ? 0 : price / num_of_receivers;
            group[giver_index].balance -= each_given * num_of_receivers;

            for(int j = 0 ; j < num_of_receivers ; j++)
            {
                scanf("%s", receiver);
                int receiver_index = findPersonIndex(group, receiver, n);
                group[receiver_index].balance += each_given;
            }
        }
        if(t) printf("\n");
        t++;
        for(int i = 0 ; i < n ; i++) printf("%s %d\n", group[i].name, group[i].balance);
    }
}

int findPersonIndex(struct person group[], const char* name, int size)
{
    for(int i = 0 ; i < size ; i++) if(strcmp(group[i].name, name) == 0) {return i;} return -1;
}