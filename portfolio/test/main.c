#include "myge.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_TESTS 5

void print_score(int32_t score, int failed_tests[]){
    printf("%d, WA: ", score);
    // printf("Failed Test Cases: ");
    for (int i = 0; i < NUM_TESTS; i++) {
        if (failed_tests[i] != 0) {
            printf("%d ", i + 1);
        }
    }
    printf("\n");
    fflush(stdout);
}

int main() {

    int32_t score = 0;
    int32_t failed_tests[NUM_TESTS];
    for(int i = 0; i < NUM_TESTS; i++){
        failed_tests[i] = -1;
    }

    // Test 1~4: n = 3
    int32_t n = 3;

    // Flatten the matrix into a single array
    int32_t A[4][9] = {
        {   1, 1, 1,
            1, 2, 4,
            1, 3, 9 },
        {   1483,53,-1,
            1616,74562,43,
            563,-5231,79345},
        {   1, 1, 1, 
            0, 0, 0, 
            1, 2, 3 },
        {   1, 2, 3, 
            2, 4, 6, 
            3, 6, 9 }
    };
    int32_t y[4][3] = {
        {6, 17, 34}, 
        {1586, 150869, 228136},
        {6, 0, 14},
        {14, 28, 42}};
    int32_t x_answer[4][3] = {
        {1, 2, 3},
        {1, 2, 3},
        {-1, -1, -1},
        {-1, -1, -1}
    };
    int32_t return_answer[4] = {1, 1, 0, 2};


    for(int t = 0; t < 4; ++t){
        int32_t *x = NULL;

        int32_t result = gaussian_elimination(n, A[t], y[t], &x);
        
        if(result != return_answer[t]){
            printf("Test %d: Wrong return value\n", t+1);
            failed_tests[t] = 1;
            print_score(score, failed_tests);
            continue;
        }
        if(result == 0 || result == 2){
            score += 4;
            printf("Test %d: All Correct\n", t+1);
            failed_tests[t] = 0;
        }
        else{
            for(int i = 0; i < n; ++i){
                if(x[i] != x_answer[t][i]){
                    failed_tests[t] = 2;
                    break;
                }
            }
            if(failed_tests[t] == -1){
                score += 4;
                printf("Test %d: All Correct\n", t+1);
                failed_tests[t] = 0;
            }
            else{
                score += 2;
                printf("Test %d: Wrong answer\n", t+1);
                printf("Your answer: ");
                for(int i = 0; i < n; ++i){
                    printf("%d ", x[i]);
                }
                printf("\n");
                printf("Correct answer: ");
                for(int i = 0; i < n; ++i){
                    printf("%d ", x_answer[t][i]);
                }
                printf("\n");
            }
        }

        print_score(score, failed_tests);
    }

    n = 4;
    int32_t B[16]= {
        1, 2, 3, 4, 
        5, 6, 7, 8,
        9, -10, -11, 12,
        13, -14, -15, -16
    };
    int32_t yB[4] = {30,  70, 4, -124};
    int32_t xB_answer[4] = {1, 2, 3, 4};
    int32_t Breturn_answer = {1};
    int32_t *x = NULL;

    int32_t result = gaussian_elimination(n, B, yB, &x);


    if(result != Breturn_answer){
        printf("Test 5: Wrong return value\n");
        failed_tests[4] = 1;
        print_score(score, failed_tests);
        return 0;
    }
    if(result == 0 || result == 2){
            score += 4;
        printf("Test 5: All Correct\n");
        failed_tests[4] = 0;
    }
    else{
        for(int i = 0; i < n; ++i){
            if(x[i] != xB_answer[i]){
                failed_tests[4] = 2;
                break;
            }
        }
        if(failed_tests[4] == -1){
            score += 4;
            printf("Test 5: All Correct\n");
            failed_tests[4] = 0;
        }
        else{
            score += 2;
            printf("Test 5: Wrong answer\n");
            printf("Your answer: ");
            for(int i = 0; i < n; ++i){
                printf("%d ", x[i]);
            }
            printf("\n");
            printf("Correct answer: ");
            for(int i = 0; i < n; ++i){
                printf("%d ", xB_answer[i]);
            }
            printf("\n");
        }
    }

    print_score(score, failed_tests);

    return 0;
}