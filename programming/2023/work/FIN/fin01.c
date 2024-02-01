#include <stdio.h>
#include "frac.h"

#define NUM_TESTS 20

void print_score(int32_t score, int failed_tests[]){
    printf("%d, WA: ", score);
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
    int32_t x, y;
    int failed_tests[NUM_TESTS] = {}; // Array to track failed tests
    for(int i = 0; i < NUM_TESTS; i++){
        failed_tests[i] = -1;
    }

    int add_testx1[5] = {1, 0, 3, 87, 2147483647-87};
    int add_testy1[5] = {2, 1, 4, 1073741819, 100};
    int add_testx2[5] = {1, 1, -5, 2147482164, -100};
    int add_testy2[5] = {3, 3, 6, 2147483638, 2147483647-56};
    int add_ansx[5] = {5, 1, -1, 1073741169, 0};
    int add_ansy[5] = {6, 3, 12, 1073741819, 0};
    int add_ansisvalid[5] = {0, 0, 0, 0, -1};
    
    for(int i = 0; i < 5; i++){
        int isvalid = frac_add(&x, &y, add_testx1[i], add_testy1[i], add_testx2[i], add_testy2[i]);
        if(isvalid != add_ansisvalid[i]){
            failed_tests[i] = 1;
            continue;
        }
        if(isvalid == -1 || (x == add_ansx[i] && y == add_ansy[i])){
            score++;
            failed_tests[i] = 0;
        }
        else{
            failed_tests[i] = 1;
        }
    }

    int del_testx1[5] = {3, 0, -1, 321, 1};
    int del_testy1[5] = {4, 1, 2, 73, 0};
    int del_testx2[5] = {1, 1, -1, 36482, 1};
    int del_testy2[5] = {2, 3, 4, 675835, 2};
    int del_ansx[5] = {1, -1, -1, 12604697, 0};
    int del_ansy[5] = {4, 3, 4, 2902115, 0};
    int del_ansisvalid[5] = {0, 0, 0, 0, -1};

    for(int i = 0; i < 5; i++){
        int isvalid = frac_del(&x, &y, del_testx1[i], del_testy1[i], del_testx2[i], del_testy2[i]);
        if(isvalid != del_ansisvalid[i]){
            failed_tests[i+5] = 1;
            continue;
        }
        if(isvalid == -1 || (x == del_ansx[i] && y == del_ansy[i])){
            score++;
            failed_tests[i+5] = 0;
        }
        else{
            failed_tests[i+5] = 1;
        }
    }

    int mul_testx1[5] = {1, 0, 2147483647-1483, 1073741824, 1};
    int mul_testy1[5] = {2, 1, (2147483647-9)/2, 1, 0};
    int mul_testx2[5] = {2, 1, 2147483647-9, 4, 1};
    int mul_testy2[5] = {3, 3, 2147483647-1483, 1, 2};
    int mul_ansx[5] = {1, 0, 2, -1, -1};
    int mul_ansy[5] = {3, 1, 1, 0, -1};
    int mul_ansisvalid[5] = {0, 0, 0, -1, -1};

    for(int i = 0; i < 5; i++){
        int isvalid = frac_mul(&x, &y, mul_testx1[i], mul_testy1[i], mul_testx2[i], mul_testy2[i]);
        if(isvalid != mul_ansisvalid[i]){
            failed_tests[i+10] = 1;
            continue;
        }
        if(isvalid == -1 || (x == mul_ansx[i] && y == mul_ansy[i])){
            score++;
            failed_tests[i+10] = 0;
        }
        else{
            failed_tests[i+10] = 1;
        }
    }

    int div_testx1[5] = {1, 0, -1, 1, 2147483647};
    int div_testy1[5] = {2, 1, 2, 2, 1};
    int div_testx2[5] = {1, 1, 1, 0, 1};
    int div_testy2[5] = {3, 3, 4, 1, 2147483647};
    int div_ansx[5] = {3, 0, -2, 0, 0};
    int div_ansy[5] = {2, 1, 1, 0, 0};
    int div_ansisvalid[5] = {0, 0, 0, -1, -1};

    for(int i = 0; i < 5; i++){
        int isvalid = frac_div(&x, &y, div_testx1[i], div_testy1[i], div_testx2[i], div_testy2[i]);
        if(isvalid != div_ansisvalid[i]){
            failed_tests[i+15] = 1;
            continue;
        }
        if(isvalid == -1 || (x == div_ansx[i] && y == div_ansy[i])){
            score++;
            failed_tests[i+15] = 0;
        }
        else{
            failed_tests[i+15] = 1;
        }
    }

    print_score(score, failed_tests);

    return 0;
}