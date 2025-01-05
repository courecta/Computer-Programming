#include <stdio.h>

void execute_callback(void (*callback)(void));
void my_callback(void);
void your_callback(void);

int main(void){
    execute_callback(your_callback);
    execute_callback(my_callback);
}

void execute_callback(void (*callback)(void))
{
    callback();
}

void my_callback(void)
{
    printf("my callback executed!\n");
}

void your_callback(void)
{
    printf("your callback executed!\n");
}