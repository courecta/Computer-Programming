#include "trebuchet.h"

int main(){
  FILE *fptr;
  long sum = 0;
  fptr = fopen("text.txt", "r");
  char input_buffer[UINT8_MAX];

  if(fptr != NULL)
  {
    while(fgets(input_buffer, UINT8_MAX, fptr))
    {
      sum += extract(input_buffer);
    }
    printf("%ld\n", sum);
  } else printf("error\n");  

  fclose(fptr);
  
  return 0;
}

