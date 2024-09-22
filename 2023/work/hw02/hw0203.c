#include <stdio.h>
#include <stdint.h>

int main () {

  int32_t number = 0, start = 0, odd, even, loopDone; // Process data
  int32_t enterS0odd = 0, enterS1odd = 0, enterS2odd = 0, enterS3odd = 0, enterS4odd, enterS5odd = 0, enterS6odd = 0; // odd enter heads
  int32_t enterS0even = 0, enterS1even = 0, enterS2even = 0, enterS3even = 0, enterS4even = 0, enterS5even = 0, enterS6even = 0; // even enter heads
  int32_t leaveS0odd = 0, leaveS0even = 0, leaveS1odd = 0, leaveS1even = 0, leaveS2odd = 0, leaveS2even = 0, leaveS3odd = 0, leaveS3even = 0, leaveS4odd = 0, leaveS4even = 0, leaveS5odd = 0, leaveS5even = 0, leaveS6odd = 0, leaveS6even = 0;
  // leave head states
  int32_t S0 = 0, S1 = 0, S2 = 0, S3 = 0, S4 = 0, S5 = 0, S6 = 0; // Final states

// ------------------------------------------------------------------------
    
do { // Loop for inputting integers and checking for the stop code 0
  if (loopDone == 0){
  printf("Please enter an Integer: ");
  scanf("%d", &number);
  }
    while (number == 0){
        if (number == 0){ // checks for the stop codes and halts the read
      printf("Possible States: ");
      
      if (S0 >= 1){ // According to the calculated states, will output accordingly
        printf("S0");
      }
      if (S0 != 0){
      if (S1 != 0 || S2 != 0 || S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      }
      if (S1 >= 1){
        printf("S1");
      }
      if (S1 != 0){
        if (S2 != 0 || S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      }
      if (S2 >= 1){
        printf("S2");
      }
      if (S2 != 0){
        if (S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      }
      if (S3 >= 1){
        printf("S3");
      }
      if (S3 != 0){
        if (S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      }
      if (S4 >= 1){
        printf("S4");
      }
      if (S4 != 0){
        if (S5 != 0 || S6 != 0){
        printf(", ");
      }
      }
      if (S5 >= 1){
        printf("S5");
      }
      if (S5 != 0 && S6 != 0){
        printf(", ");
      }
      if (S6 >= 1){
        printf("S6");
      }

      printf("\n");
      return 0;
      }
    }
        
    if (even == 1){
      even -= 1;
    }
    else if (odd == 1){
      odd -= 1;
    }

    if (number % 2 == 0) {
          even += 1;
          loopDone += 1;
          start += 1;
    }
    else {
          odd += 1;
          loopDone += 1;
          start += 1;
    }

    if (start == 1){// the game begins and the machine initializes into S0
      start += 1;
      if (even == 1){
        enterS3even += 1;
      }
      if (odd == 1){
        enterS1odd += 1;
        enterS2odd += 1;
      }
    }

// ----------------------------------------------------------------------------------------
    if (loopDone == 1){
    if (S0 >= 1){ // Evaluates the state of S0
      if (even == 1){
        S0 -= 1;
        leaveS0even += 1;
      }
      else if (odd == 1){
        S0 -= 1;
        leaveS0odd += 1;
      }
    }

    if (S1 >= 1){ // Evaluates the state of S1
      if (even == 1){
        S1 -= 1;
        leaveS1even += 1;
      }
      else if (odd == 1){
        S1 -= 1;
        leaveS1odd += 1;
      }
    }

    if (S2 >= 1){ // Evaluates the state of S2
      if (even == 1){
        S2 -= 1;
        leaveS2even += 1;
      }
      else if (odd == 1){
        S2 -= 1;
        leaveS2odd += 1;
      }
    }
    
    if (S3 >= 1){ // Evaluates the state of S3
      if (even == 1){
        S3 -= 1;
        leaveS3even += 1;
      }
      else if (odd == 1){
        S3 -= 1;
        leaveS3odd += 1;
      }
    }

    if (S4 >= 1){ // Evaluates the state of S4
      if (even == 1){
        S4 -= 1;
        leaveS4even += 1;
      }
      else if (odd == 1){
        S4 -= 1;
        leaveS4odd += 1;
      }
    }

    if (S5 >= 1){ // Evaluates the state of S5
      if (even == 1){
        S5 -= 1;
        leaveS5even += 1;
      }
      else if (odd == 1){
        S5 -= 1;
        leaveS5odd += 1;
      }
    }

    if (S6 >= 1){ // Evaluates the state of S6
      if (even == 1){
        S6 -= 1;
        leaveS6even += 1;
      }
      else if (odd == 1){
        S6 -= 1;
        leaveS6odd += 1;
      }
    }

    if (leaveS0even >= 1){ // processes the head transfer from S0 to S3
      enterS0even -= 1;
      enterS3even += 1;
    }

    if (leaveS0odd >= 1){ // processes the head transfer from S0 to S1 and S2
      enterS0odd -= 1;
      enterS1odd += 1;
      enterS2odd += 1;
    }

    if (leaveS1even >= 1){ // processes the head transfer from S1 to S4 
      leaveS1even -= 1;
      enterS4even += 1;
    }

    if (leaveS1odd >= 1){ // processes the head transfer from S1 to S2
      leaveS1odd -= 1;
      enterS2odd += 1;
    }

    if (leaveS2even >= 1){ // processes the head transfer from S2 to S5
      leaveS2even -= 1;
      enterS5even += 1;
    }

    if (leaveS2odd >= 1){ // processes the head transfer from S2 to S3
      leaveS2odd -= 1;
      enterS3odd += 1;
    }

    if (leaveS3even >= 1){ // processes the head transfer from S3 to S0
      leaveS3even -= 1;
      enterS0even += 1;
    }

    if (leaveS3odd >= 1){ // processes the head transfer from S3 to S5
      leaveS3odd -= 1;
      enterS5odd += 1;
    }

    if (leaveS4even >= 1){ // processes the head transfer from S4 to S2 and S6
      leaveS4even -= 1;
      enterS2even += 1;
      enterS6even += 1;
    }

    if (leaveS4odd >= 1){ // processes the head transfer from S4 to S5
      leaveS4odd -= 1;
      enterS5odd += 1;
    }

    if (leaveS5even >= 1){ // processes the head transfer from S5 to S0
      leaveS5even -= 1;
      enterS0even += 1;
    }

    if (leaveS5odd >= 1){ // processes the head transfer from S5 to S6
      leaveS5odd -= 1;
      enterS6odd += 1;
    }

    if (leaveS6even >= 1){ // processes the head transfer from S6 to S1
      leaveS6even -= 1;
      enterS1even += 1;
    }

    if (leaveS6odd >= 1){ // processes the loop in S6
      leaveS6odd -= 1;
      enterS6odd += 1;
    }


    if (enterS0even >= 1 && even == 1){ // Judges if the input to the S0 state head entrance is even
      S0 += 1;
      enterS0even = 0;
    }

    if (enterS1odd >= 1 && odd == 1){ // Judges if the input to the S1 state head entrance is odd
      S1 += 1;
      enterS1odd = 0;
    }

    if (enterS1even >= 1 && even == 1){ // Judges if the input to the S1 state head entrance is even
      S1 += 1;
      enterS1even = 0;
    }

    if (enterS2even >= 1 && even == 1){ // Judges if the input to the S2 state head entrance is even
      S2 += 1;
      enterS2even = 0;
    }

    if (enterS2odd >= 1 && odd == 1){ // Judges if the input to the S2 state head entrance is odd
      S2 += 1;
      enterS2odd = 0;
    }

    if (enterS3even >= 1 && even == 1){ // Judges if the input to the S3 state head entrance is even
      S3 += 1;
      enterS3even = 0;
    }

    if (enterS3odd >= 1 && odd == 1){ // Judges if the input to the S3 state head entrance is odd
      S3 += 1;
      enterS3odd = 0;
    }

    if (enterS4even >= 1 && even == 1){ // Judges if the input to the S4 state head entrance is even
      S4 += 1;
      enterS4even = 0;
    }

    if (enterS4odd >= 1 && odd == 1){ // Judges if the input to the S4 state head entrance is odd
      S4 += 1;
      enterS4odd = 0;
    }

    if (enterS5even >= 1 && even == 1){ // Judges if the input to the S5 state head entrance is even
      S5 += 1;
      enterS5even = 0;
    }

    if (enterS5odd >= 1 && odd == 1){ // Judges if the input to the S5 state head entrance is odd
      S5 += 1;
      enterS5odd = 0;
    }

    if (enterS6even >= 1 && even == 1){ // Judges if the input to the S6 state head entrance is even
      S6 += 1;
      enterS6even = 0;
    }

    if (enterS6odd >= 1 && odd == 1){ // Judges if the input to the S6 state head entrance is odd
      S6 += 1;
      enterS6odd = 0;
    }

  loopDone -= 1;
    }

} while (number != 0);

  return 0;
}