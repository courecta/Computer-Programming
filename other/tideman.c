#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }


    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for(int i = 0; i < candidate_count; i++){

        if(strcmp(name, candidates[i]) == 0){

            ranks[rank] = i;

            return true;

        }

    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //
    for(int i = 0; i < candidate_count; i++){

        for(int j = i + 1; j < candidate_count; j++){

            preferences[ranks[i]][ranks[j]]++;

        }

    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for(int i = 0; i < candidate_count; i++){

        for(int j = i + 1; j < candidate_count; j++){

            if(preferences[i][j] > preferences[j][i]){

                //Add pair struct to pairs array
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;

            } else if(preferences[i][j] < preferences[j][i]){

                //Add pair struct to pairs array
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;

            }

        }

    }
    return;
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    for(int i = 0; i < pair_count - 1;  i++){

        for(int j = 0; j < pair_count - i - 1; j++){

            if(preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j+1].winner][pairs[j+1].loser]){

                pair temp = pairs[j];
                pairs[j] = pairs[j+1];
                pairs[j+1] = temp;

            }

        }

    }
    return;
}

bool cycle_pairs(int endCase, int start){
    
    if (endCase == start){
        
        return true;
        
    }
    
    for(int i = 0; i < candidate_count; i++){
        
        //check if each position in array is true/false
        //if true go onto next check
        if(locked[endCase][i]){
            //recursively check with current candidate as new end argument 
            //this loops until false is returned or base case is triggered, thus creating cycle and returning true
            if(cycle_pairs(i, start)){
                 
                 return true;
                 
            }
        }
  
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for(int i = 0; i < pair_count; i++){
        
        if (!cycle_pairs(pairs[i].loser, pairs[i].winner)){
            
            locked[pairs[i].winner][pairs[i].loser] = true;
            
        }
        
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
    // TODO
    for(int i = 0; i < candidate_count; i++){
        
        int falseCount = 0;
        
        for(int j = 0; j < candidate_count; j++){
            
            if(locked[j][i] == false){
                falseCount++;
            }
            
        }
        
        if(falseCount == candidate_count){
            printf("%s\n", candidates[i]);
            return;
        }
        
    }
    return;
}
