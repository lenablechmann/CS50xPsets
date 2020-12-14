// http://valgrind.org/docs/manual/quick-start.html#quick-start.prepare
// help50 valgrind ./valgrind

#include <stdlib.h>

//The function f allocates enough memory for 10 integers, and stores the address in a pointer called x.
void f(void)
{
    int *x = malloc(10 * sizeof(int));
    x[10] = 0; //would have to change it to 9
}

int main(void)
{
    f();
    return 0;
}