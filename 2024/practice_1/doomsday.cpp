#include <stdio.h>

int main(){

    const char* days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    int cases(0), m(0), d(0);
    int pointer = 0;

    scanf("%d", &cases);

    while(cases--)
    {
        scanf("%d%d", &m, &d);
        pointer = 0;

        switch(m)
        {
            case 1:
            for(int i = 10 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 2:
            for(int i = 21 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 3:
            for(int i = 7 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 4:
            for(int i = 4 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 5:
            for(int i = 9 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 6:
            for(int i = 6 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 7:
            for(int i = 11 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 8:
            for(int i = 8 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 9:
            for(int i = 5 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 10:
            for(int i = 10 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 11:
            for(int i = 7 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;

            case 12:
            for(int i = 12 ;  ; )
            {
                if(d > i)
                {
                    pointer++;
                    i++;
                } else if(d < i)
                {
                    pointer--;
                    i--;
                } else break;
                
                if(pointer > 6) pointer = 0;
                else if(pointer < 0) pointer = 6;
            }
            break;
        }

        printf("%s\n", days[pointer]);

    }

}