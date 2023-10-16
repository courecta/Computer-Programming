#include <stdio.h>
#include <stdint.h>

int main () {

  int32_t number, start, die, pay, odd, even, initial, play = 0; // Process data
  int16_t enterS0odd, enterS1odd, enterS2odd, enterS3odd, enterS4odd, enterS5odd, enterS6odd; // odd enter heads
  int16_t enterS0even, enterS1even, enterS2even, enterS3even, enterS4even, enterS5even, enterS6even; // even enter heads
  int16_t leaveS0odd, leaveS0even, leaveS1odd, leaveS1even, leaveS2odd, leaveS2even, leaveS3odd, leaveS3even, leaveS4odd, leaveS4even, leaveS5odd, leaveS5even, leaveS6odd, leaveS6even;
  // leave head states
  int32_t S0, S1, S2, S3, S4, S5, S6; // Final states

// ------------------------------------------------------------------------
    do{ // Loop for inputting integers and checking for the stop code 0
        printf("Please enter an Integer: ");
        scanf("%d", &number);
        if (number == 0){
          die += 1;
          break;
        }
        
        start++;
        pay++;

        if (start >= 1){
          even = 0;
          odd = 0;
        }

    } while (number != 0 && play == 0);

      while (pay == 1){ // Checks for the even or odd integer and resets counter
        if (number % 2 == 0) {
            even += 1;
        }
        else {
          odd += 1;
        }
        play++;
        pay--;
      }

// ------------------------------------------------------------------------------
    while (play == 1){
    if (start == 1){// the game begins and the machine initializes into S0
      start++;
      if (even == 1){
        enterS0even++;
      }
      if (odd == 1){
        enterS0odd++;
      }
      play--;
      continue;
    }

// ----------------------------------------------------------------------------------------

    if (enterS0odd >= 1) { // Special case only to handle first case of odd
          leaveS0odd++;
    }

    if (enterS0even >= 1){ // Judges if the input to the S0 state head entrance is even
      if (leaveS3even >= 1){
        leaveS3even--;
      }
      if (leaveS5even >= 1){
        leaveS5even--;// 
      }
      leaveS0even++;
    }

    if (leaveS0even >= 1){ // processes the head transfer from S0 to S3
      enterS0even--;
      enterS3even++;
    }

    if (leaveS0odd >= 1){ // processes the head transfer from S0 to S1 and S2
      enterS0odd--;
      enterS1odd++;
      enterS2odd++;
    }

    if (leaveS1even >= 1){ // processes the head transfer from S1 to S4 
      leaveS1even--;
      enterS4even++;
    }

    if (leaveS1odd >= 1){ // processes the head transfer from S1 to S2
      leaveS1odd--;
      enterS2odd++;
    }

    if (leaveS2even >= 1){ // processes the head transfer from S2 to S5
      leaveS2even--;
      enterS5even++;
    }

    if (leaveS2odd >= 1){ // processes the head transfer from S2 to S3
      leaveS2odd--;
      enterS3odd++;
    }

    if (leaveS3even >= 1){ // processes the head transfer from S3 to S0
      leaveS3even--;
      enterS0even++;
    }

    if (leaveS3odd >= 1){ // processes the head transfer from S3 to S5
      leaveS3odd--;
      enterS5odd++;
    }

    if (leaveS4even >= 1){ // processes the head transfer from S4 to S2 and S6
      leaveS4even--;
      enterS2even++;
      enterS6even++;
    }

    if (leaveS4odd >= 1){ // processes the head transfer from S4 to S5
      leaveS4odd--;
      enterS5odd++;
    }

    if (leaveS5even >= 1){ // processes the head transfer from S5 to S0
      leaveS5even--;
      enterS0even++;
    }

    if (leaveS5odd >= 1){ // processes the head transfer from S5 to S6
      leaveS5odd--;
      enterS6odd++;
    }

    if (leaveS6even >= 1){ // processes the head transfer from S6 to S1
      leaveS6even--;
      enterS1even++;
    }

    if (leaveS6odd){ // processes the loop in S6
      leaveS6odd--;
      enterS6odd++;
    }

      S0 = enterS0even + enterS0odd;
      S1 = enterS1even + enterS1odd;
      S2 = enterS2even + enterS2odd;
      S3 = enterS3even + enterS3odd;
      S4 = enterS4even + enterS4odd;
      S5 = enterS5even + enterS5odd;
      S6 = enterS6even + enterS6odd;
      
      play--;
    }

// ----------------------------------------------------------------------------------
    
    if (number == 0 && die == 1 && play == 0){ // checks for the stop codes and halts the read
      printf("Possible States: ");
      
      if (S0 >= 1){ // According to the calculated states, will output accordingly
        printf("S0");
      }
      if (S1 != 0 || S2 != 0 || S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      if (S1 >= 1){
        printf("S1");
      }
      if (S2 != 0 || S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      if (S2 >= 1){
        printf("S2");
      }
      if (S3 != 0 || S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      if (S3 >= 1){
        printf("S3");
      }
      if (S4 != 0 || S5 != 0 || S6 != 0){
        printf(", ");
      }
      if (S4 >= 1){
        printf("S4");
      }
      if (S5 != 0 || S6 != 0){
        printf(", ");
      }
      if (S5 >= 1){
        printf("S5");
      }
      if (S6 != 0){
        printf(", ");
      }
      if (S6 >= 1){
        printf("S6");
      }

      printf("\n");
      return 0;
    }

  return 0;
}