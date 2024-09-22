#include <stdio.h>
#include <stdint.h>

void print_cuboid();
void print_catan();

int main()
{
  int choice = 0;
  printf("Hello, welcome to Super infinity Center! What service do you want to use?\n1. Cuboid Super Infinity Exporter\n2. Catan Simple Islands Engine");
  scanf("%d", &choice);

  (choice > 1) ? print_catan() : print_cuboid();
  
  return 0;
}

void print_cuboid()
{
  int64_t length = 0, w = 0, h = 0, n = 0;

  printf("Welcome to Cuboid Super Infinity Exporter!\nPlease enter Length , Width , and Height of the cuboid\n");
  printf("Length: ");
  scanf("%ld", &length);
  printf("Width: ");
  scanf("%ld", &w);
  printf("Height: ");
  scanf("%ld", &h);
  printf("How many cuboids do you want to generate?\n");
  printf("Amount: ");
  scanf("%ld", &n);

    length *= 2;
    int64_t x_line = length + (w - 1);
    int64_t y_line = h + (w - 1);

    if(length < 8 || w < 4 || h < 4 || x_line > 80)
    {
        printf("Error! Incorrect input!\n");
        return;
    }

    int box_perLine = 81 / (length + w);

    while(n > 0)
    {
        if(n < box_perLine) box_perLine = n;
        n -= box_perLine;

        for(int i = 0 ; i < y_line ; ++i)
        {
            for(int k = 0 ; k < box_perLine ; ++k)
            {
                for(int j = 0 ; j < x_line ; ++j)
                {
                    // printing edges

                    // diagonals
                    if((j + i == w - 1) || (j + i == length + w - 2 && i <= w - 1) || (j + i == h - 1 + length + w - 2))
                    {
                        printf("#");
                        continue;
                    }
                    //horizontals
                    if((i == 0 && j >= (w - 1)) || (j < length && (i == w - 1)) || (j < length && (i == ((h - 1) + (w - 1)))))
                    {
                        printf("#");
                        continue;
                    }
                    //verticals
                    if((i >= (w - 1) && j == 0) || (i >= (w - 1) && j == (length - 1)) || (i < h && j == ((w - 1)+ (length - 1))))
                    {
                        printf("#");
                        continue;
                    }
                    // printing the colored faces

                    // top area
                    if((i > 0) && (i < w-1) && (j + i > w - 1) && (j+i < length + w - 2))
                    {
                        printf("\033[32;42m \033[0m");
                    }
                    // front area
                    else if((i > w - 1) && (i < w +h - 2) && (j > 0) && (j < length - 1))
                    {
                        printf("\033[44m \033[0m");
                    }
                    // right area
                    else if((j > length - 1) && (j < w + length - 2) && (j + i > length + w - 2) && (j + i < h - 1 + length + w - 2))
                    {
                        printf("\033[33;43m \033[0m");
                    } else printf(" ");

                }
                if(k != box_perLine - 1) printf(" ");
            }
            printf("\n");
        }
        if(n != 0) printf("\n");
    }
  return;
}

void print_catan()
{
  int64_t length = 0, number_layer = 0;

  printf("Please input the length: ");
  scanf("%ld", &length);
  printf("Please input the number of layer: ");
  scanf("%ld", &number_layer);



  return;
}