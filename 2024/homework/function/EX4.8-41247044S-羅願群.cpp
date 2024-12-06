#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
game flow
------------
display monster status
prompt player action
if single attack --> has 20% chance to miss
if group attack --> prompt which alive monster to attack
display damage done to monster
monster action --> heal other monster if not, attacks player
if combo hit --> hurt by 3 points
pause and wait for user input --> clear screen and loop-over
---
*) On linux, using read, it only accepts enter
*) ANSI escape codes may be unsupported, oh well
*/

void pause();
int all_clear(int num, int monsters[]);
int player_death(int health);
int generate_random(int chance);

int main(){

    // initialize variables
    constexpr int numMonsters = 5;
    constexpr int monsterHealth = 10;
    constexpr int player_hitChance = 80;
    constexpr int monster_comboChance = 10;
    int playerHealth = 30, playerChoice = 0, choice = 0, attack_id = 0, clear = 0, die = 0, healed = 0;
    int monster[numMonsters] = {0};
    for(int i = 0 ; i < numMonsters ; i++) monster[i] = monsterHealth;
    srand(time(NULL));

    // game loop
    while(1)
    {
        printf("\033[H\033[J");
        // update game status
        printf("***Monster Hunting Game***\n\n");

        // show monster's status
        for(int i = 0 ; i < numMonsters ; i++)
        {
            printf("Monster %d ", i + 1);
            if(monster[i] > 0)
            {
                printf("(%d)", monster[i]);
                for(int j = 0 ; j < monster[i] ; j++) printf("*");
                printf("\n");
            } 
            else printf("-DEAD-\n");
        }
        printf("\n");

        // print player's health
        printf("Player's life:(%d)", playerHealth);
        for(int i = 0 ; i < playerHealth ; i++) printf("*");
        printf("\n");

        // Player's turn
        printf("\nPlayer's turn: (1) single attack (7P) (2) group attack (2P)...> ");
        scanf("%d", &choice);
        choice -= 1;

        // player's attack
        if(choice)
        {
            for(int i = 0; i < numMonsters ; i++) monster[i] = (monster[i] > 2) ? monster[i] - 2 : 0;
            printf("you hit all monsters by 2 points!\n");
        }
        else
        {
            printf("which monster do you want to attack?...> ");
            scanf("%d", &attack_id);
            if(generate_random(player_hitChance))
            {
                monster[attack_id - 1] = (monster[attack_id - 1] > 7) ? monster[attack_id - 1] - 7 : 0;
                printf("you hit monster %d by 7 points!\n", attack_id);
            } else printf("\033[1;31mMissed! You do not hit monster %d.\033[0m\n", attack_id);
        }

        pause();

        // monsters action turn
        for(int i = 0 ; i < numMonsters ; i++)
        {
            if(monster[i] <= 0) continue;
            healed = 1;
            for(int j = 0 ; j < numMonsters ; j++)
            {
                if(monster[j] <= 3 && monster[j] > 0)
                {
                    monster[j] += 1;
                    printf("Monster %d heals monster %d by 1 point.\n", i + 1, j + 1);
                    healed -= 1;
                }
                if(!healed) break;
            }
            if(healed)
            {
                if(generate_random(monster_comboChance))
                {
                    playerHealth = (playerHealth > 3) ? playerHealth - 3 : 0;
                    printf("\033[1;33mCombo hit! Monster %d hits you by 3 points!\033[0m\n", i + 1);
                }
                else
                {
                    playerHealth = (playerHealth > 1) ? playerHealth - 1 : 0;
                    printf("Monster %d hits you by 1 point!\n", i + 1); 
                }
            }
        }

        // check break loop conditions
        clear = all_clear(numMonsters, monster);
        die = player_death(playerHealth);
        if(clear) printf("\033[1;32myou have killed ALL monsters!\033[0m\n");
        else if(die) printf("\033[1;31mOh no, you are killed.\033[0m\n");
        pause();
        if((clear) || (die)) break;
    }

    return 0;
}

void pause()
{
    #ifdef _WIN32
        system("pause");
    #else
        system("read -p 'Please press any key to continue...'");
    #endif
}

int all_clear(int num, int monsters[])
{
    for(int i = 0 ; i < num ; i++) if(monsters[i] > 0) return 0;
    return 1;
}

int player_death(int health)
{
    if(health > 0) return 0;
    else return 1;
}

int generate_random(int chance)
{
    return (rand() % 100) < chance;
}