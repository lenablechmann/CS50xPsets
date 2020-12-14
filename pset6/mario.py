from cs50 import get_int

# Prompts the user until they give a good int
while True:
    height = get_int("Positive Integer (1-8): ")
    if height > 0 and height < 9:
        break

# a bit of a brute force huh, but works, so whatever
for i in range(height):
    for j in range(height - i - 1):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print()a program that computes the approximate grade level needed to comprehend some text