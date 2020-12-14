#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    /*
    // replace getsring with
    char *s = NULL;
    printf("s: ");
    scanf("%s", s);
    printf("s: %s\n", s);
    and string with char*/

    char s[5];
    //char *s = NULL; //something might be up here
    //get_string("s: ");
    printf("s: ");
    scanf("%s", s);
    //printf("s: %s\n", s);

    char *t = malloc(strlen(s) + 1);

    for (int i = 0, n = strlen(s); i < n + 1; i++)
    {
        t[i] = s[i];
    }

    t[0] = toupper(t[0]);

    printf("s is: %s\n", s);
    printf("t is: %s\n", t);
    free(t); //to free up all that memory
}