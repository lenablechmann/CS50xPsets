#include <stdio.h>
#include <math.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// The Caesar cipher.

int main(int argc, string argv[])

{
    // Checking if user used 1 command line argument or nah.
    if (argc != 2) // argc has to be at least 1, 2 is used if you use 1 command line argument, 3 for 2 command line arguments.
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Checking if argv[1] is actually a number.
    string s = argv[1];
    for (int i = 0, length = strlen(s); i < length; i++) // Looping through the string to check if truly number.
    {
        if (!isdigit(s[i]))
        {
            printf("Usage: ./caesar key\n"); // Prints this if isn't a number.
            return 1;
        }
    }

    int key;
    key = atoi(s);
    printf("Success\n%i\n", key); // Prints out the key as an int.


    // Getting a string from the user.
    string text = get_string("plaintext: ");

    printf("ciphertext:  ");
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]) && islower(text[i]))
        {
            printf("%c", (((text[i]) + key - 97) % 26) + 97); // Cipher for lower case letter.
        }

        if (isalpha(text[i]) && isupper(text[i]))
        {
            printf("%c", (((text[i]) + key - 65) % 26) + 65); // Cipher for upper case letter.
        }

        if (!isalpha(text[i]))
        {
            printf("%c", text[i]); // Conserves the rest.
        }

    }
    printf("\n");
    return 0;
}