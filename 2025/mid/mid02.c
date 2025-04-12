#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEAMS 1000
#define MAX_LINE_LENGTH 1000
#define MAX_TEAM_NAME_LENGTH 100
#define MAX_COUNTRY_LENGTH 10

typedef struct {
    int position;
    char club[MAX_TEAM_NAME_LENGTH];
    char country[MAX_COUNTRY_LENGTH];
    int participated;
    int titles;
    int played;
    int win;
    int draw;
    int loss;
    int goals_for;
    int goals_against;
    int pts;
    int goal_diff;
    double win_rate; // Calculated field: Win/Played
} Team;

// Function to compare teams based on titles
int compareTitles(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->titles != teamB->titles) {
        return teamB->titles - teamA->titles;
    }
    return teamB->pts - teamA->pts; // If titles are equal, compare by points
}

// Function to compare teams based on played games
int comparePlayed(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->played != teamB->played) {
        return teamB->played - teamA->played;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on wins
int compareWins(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->win != teamB->win) {
        return teamB->win - teamA->win;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on draws
int compareDraws(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->draw != teamB->draw) {
        return teamB->draw - teamA->draw;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on losses
int compareLosses(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->loss != teamB->loss) {
        return teamB->loss - teamA->loss;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on goals for
int compareGoalsFor(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->goals_for != teamB->goals_for) {
        return teamB->goals_for - teamA->goals_for;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on goals against
int compareGoalsAgainst(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->goals_against != teamB->goals_against) {
        return teamB->goals_against - teamA->goals_against;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on goal difference
int compareGoalDiff(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->goal_diff != teamB->goal_diff) {
        return teamB->goal_diff - teamA->goal_diff;
    }
    return teamB->pts - teamA->pts;
}

// Function to compare teams based on winning rate
int compareWinRate(const void *a, const void *b) {
    const Team *teamA = (const Team *)a;
    const Team *teamB = (const Team *)b;

    if (teamA->win_rate > teamB->win_rate) return -1;
    if (teamA->win_rate < teamB->win_rate) return 1;
    return teamB->pts - teamA->pts;
}

// Function to read and parse the CSV file
int readCSV(const char *filename, Team teams[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    // Skip the header line
    fgets(line, MAX_LINE_LENGTH, file);

    int count = 0;
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && count < MAX_TEAMS) {
        char *token;
        token = strtok(line, ",");
        teams[count].position = atoi(token);

        token = strtok(NULL, ",");
        strncpy(teams[count].club, token, MAX_TEAM_NAME_LENGTH - 1);
        teams[count].club[MAX_TEAM_NAME_LENGTH - 1] = '\0';

        token = strtok(NULL, ",");
        strncpy(teams[count].country, token, MAX_COUNTRY_LENGTH - 1);
        teams[count].country[MAX_COUNTRY_LENGTH - 1] = '\0';

        token = strtok(NULL, ",");
        teams[count].participated = atoi(token);

        token = strtok(NULL, ",");
        teams[count].titles = atoi(token);

        token = strtok(NULL, ",");
        teams[count].played = atoi(token);

        token = strtok(NULL, ",");
        teams[count].win = atoi(token);

        token = strtok(NULL, ",");
        teams[count].draw = atoi(token);

        token = strtok(NULL, ",");
        teams[count].loss = atoi(token);

        token = strtok(NULL, ",");
        teams[count].goals_for = atoi(token);

        token = strtok(NULL, ",");
        teams[count].goals_against = atoi(token);

        token = strtok(NULL, ",");
        teams[count].pts = atoi(token);

        token = strtok(NULL, ",");
        teams[count].goal_diff = atoi(token);

        // Calculate winning rate
        teams[count].win_rate = (double)teams[count].win / teams[count].played;

        count++;
    }

    fclose(file);
    return count;
}

// Function to display teams
void displayTeams(Team teams[], int count, int limit) {
    if (limit > count) limit = count;
    for (int i = 0; i < limit; i++) {
        printf("%s\n", teams[i].club);
    }
}

int main() {
    Team teams[MAX_TEAMS];
    char filename[100];
    int choice, num_teams;

    printf("Please enter the record data: ");
    scanf("%99s", filename);

    num_teams = readCSV(filename, teams);
    if (num_teams == -1) {
        return 1; // Exit if file not found
    }

    while (1) {
        printf("---\n");
        printf("00) Exit\n");
        printf("01) Win the most titles , Top 10\n");
        printf("02) Play the most games , Top 10\n");
        printf("03) Win the most games , Top 10\n");
        printf("04) Draw the most games , Top 10\n");
        printf("05) Lose the most games , Top 10\n");
        printf("06) Goals for: Top 10\n");
        printf("07) Goals for: Bottom 10\n");
        printf("08) Goals against: Top 10\n");
        printf("09) Goals against: Bottom 10\n");
        printf("10) Goals diff: Top 10\n");
        printf("11) Goals diff: Bottom 10\n");
        printf("12) Winning Rate: Top 10\n");
        printf("13) Winning Rate: Bottom 10\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            break; // Exit the program
        } else if (choice < 0 || choice > 13) {
            continue; // Invalid choice, ask again
        }

        // Create a copy of teams for sorting
        Team sorted_teams[MAX_TEAMS];
        memcpy(sorted_teams, teams, sizeof(Team) * num_teams);

        switch (choice) {
            case 1: // Titles
                qsort(sorted_teams, num_teams, sizeof(Team), compareTitles);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 2: // Games played
                qsort(sorted_teams, num_teams, sizeof(Team), comparePlayed);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 3: // Wins
                qsort(sorted_teams, num_teams, sizeof(Team), compareWins);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 4: // Draws
                qsort(sorted_teams, num_teams, sizeof(Team), compareDraws);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 5: // Losses
                qsort(sorted_teams, num_teams, sizeof(Team), compareLosses);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 6: // Goals for - top 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalsFor);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 7: // Goals for - bottom 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalsFor);
                displayTeams(&sorted_teams[num_teams - 10], num_teams, 10);
                break;
            case 8: // Goals against - top 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalsAgainst);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 9: // Goals against - bottom 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalsAgainst);
                displayTeams(&sorted_teams[num_teams - 10], num_teams, 10);
                break;
            case 10: // Goal differential - top 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalDiff);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 11: // Goal differential - bottom 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareGoalDiff);
                displayTeams(&sorted_teams[num_teams - 10], num_teams, 10);
                break;
            case 12: // Winning rate - top 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareWinRate);
                displayTeams(sorted_teams, num_teams, 10);
                break;
            case 13: // Winning rate - bottom 10
                qsort(sorted_teams, num_teams, sizeof(Team), compareWinRate);
                displayTeams(&sorted_teams[num_teams - 10], num_teams, 10);
                break;
        }
    }

    return 0;
}