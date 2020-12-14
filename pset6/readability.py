# Computes the approximate grade level needed to comprehend some text.

from cs50 import get_string
import re

# Prompts user for text input.
text = get_string("Enter your text: ")

# Counting letters and words.
# Found here https://stackoverflow.com/questions/24878174/how-to-count-digits-letters-spaces-for-a-string-in-python
letters = sum(c.isalpha() for c in text)
words  = sum(c.isspace() for c in text) + 1

# Counting sentences via . ! ? chars.
sentences = text.count('.') + text.count('!') + text.count('?')

# Main grade (float) calculation using the given formula
grade = round(0.0588 * ((float(letters) / (float(words))) * 100.0) - 0.296 * ((float(sentences) / (float(words))) * 100.0) - 15.8)

if grade > 0 and grade < 17:
    print(f"Grade {grade}")
elif grade < 1:
    print ("Before Grade 1")
else:
    print("Grade 16+")