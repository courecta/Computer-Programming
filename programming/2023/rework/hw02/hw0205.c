#include <stdio.h>
#include <stdint.h>

void cuboid();
void catan();

int main()
{
  int choice = 0;
  printf("Hello, welcome to Super infinity Center! What service do you want to use?\n1. Cuboid Super Infinity Exporter\n2. Catan Simple Islands Engine");
  scanf("%d", &choice);

  (choice > 1) ? catan() : cuboid();
  
  return 0;
}

void cuboid()
{
  int64_t length = 0, width = 0, height = 0, amount = 0;

  printf("Welcome to Cuboid Super Infinity Exporter!\nPlease enter Length , Width , and Height of the cuboid\n");
  printf("Length: ");
  scanf("%ld", &length);
  printf("Width: ");
  scanf("%ld", &width);
  printf("Height: ");
  scanf("%ld", &height);
  printf("How many cuboids do you want to generate?\n");
  printf("Amount: ");
  scanf("%ld", &amount);

  

  return;
}

void catan()
{
  int64_t length = 0, number_layer = 0;

  printf("Please input the length: ");
  scanf("%ld", &length);
  printf("Please input the number of layer: ");
  scanf("%ld", &number_layer);



  return;
}