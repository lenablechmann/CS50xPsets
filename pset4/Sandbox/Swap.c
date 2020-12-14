#include <stdio.h>

// Swap function takes in pointers
void swap(int *a, int *b);

int main(void)
{
    int x = 1;
    int y = 2;

    printf("x is %i, y is %i\n", x, y);
    //Passing addresses of x and y
    swap(&x, &y);
    printf("x is %i, y is %i\n", x, y);
}


void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}