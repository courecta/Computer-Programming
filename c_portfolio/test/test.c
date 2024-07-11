#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main(){
    
    int l, w, h, n;
    scanf("%d %d %d %d", &l, &w, &h, &n);
    
    int length = 2 * l;
    int x_line = length + (w - 1);
    int y_line = h + (w - 1);

    bool print = false;

    int max_line = (n * (length + w - 2));
    int temp_max = max_line;

    int box_perLine = 81 / (length + w);

    for(int i = 0 ; i < y_line ; ++i)
    {
        for(int k = 0 ; k < n ; ++k)
        {
            for(int j = 0 ; j < x_line ; ++j)
            {
                print = false;
                // printing the initial spaces
                if(j < (w - 1 - i) && i < (w - 1))
                {
                    printf(" ");
                    max_line--;
                }

                // printing '#' symbols

                // diagonals
                if((i == (w - j - 1)) || (i == (x_line - j - 1)) || ((i + (h - 1)) == (x_line - j - 1))) print = true;

                //horizontals
                if((i == 0 && j >= (w - 1)) || (j < length && (i == w - 1)) || (j < length && (i == ((h - 1) + (w - 1))))) print = true;

                //verticals
                if((i >= (w - 1) && j == 0) || (i >= (w - 1) && j == (length - 1)) || (i <= (w - 1) && j == ((w - 1)+ (length - 1)))) print = true;

                // printing the colored faces


                if(print)
                {
                    printf("#");
                    max_line--;
                }
            }

            // printing the after spaces

            if(i >= (w - 1))
            {
            int space_bottom = i - (w - 2);  
            while(space_bottom)
            {
                printf(" ");
                max_line--;
            }
            printf(" ");
            } else printf(" ");

            if(temp_max - max_line >= 80)
                {
                    printf("\n");
                    temp_max -= max_line;
                }
        }
    }
    return 0;
}