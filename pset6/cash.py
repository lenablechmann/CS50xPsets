# calculates the coins needed
from cs50 import get_float

# Makes sure the user input float is positive
while True:
    dollars = get_float("Enter change: ")
    if dollars > 0:
        break

# Rounds up dollars
cents = round((dollars * 100))

coins = 0

while cents >= 25:
    cents -= 25
    coins += 1

while cents >= 10 and cents < 25:
    cents -= 10
    coins += 1

while cents >= 5 and cents < 10:
    cents -= 5
    coins += 1

while cents >= 1 and cents < 5:
    cents -= 1
    coins += 1

print(f"{coins}")