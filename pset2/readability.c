#include <stdio.h>
#include <math.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// Should become a readability grading program.

int main(void)

{

    string s = get_string("Text:  "); // Prompts user for text input.

    // Counting letters.
    int letters = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {

        if (isalpha(s[i])) // A prettier version of:   if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))
        {
            letters++;
        }

    }

    // Counting spaces.

    int spaces = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {

        if (isspace(s[i])) // Using the library command to count spaces.
        {
            spaces++;
        }

    }

    // Counting sentences via . ! ? chars.

    int sentences = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {

        if (s[i] == '.' || s[i] == '!' || s[i] == '?') // Using normal operators to discern chars.
        {
            sentences++;
        }

    }

    // Main grade calculation
    float grade;
    grade = 0.0588 * (((float) letters / ((float) spaces + 1)) * 100) - 0.296
            * (((float) sentences / ((float) spaces + 1)) * 100) - 15.8;

    if (grade >= 1 && grade <= 16)
    {
        printf("Grade %.0f\n", round(grade)); // for normal grades.
    }

    if (grade < 1)
    {
        printf("Before Grade 1\n"); // for sub1.
    }

    if (grade > 16)
    {
        printf("Grade 16+\n"); // for 16+.
    }

}
