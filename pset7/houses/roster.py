from sys import argv, exit
import sys
import re
from cs50 import SQL

# Prints out an ordered roster

# Checking user CLI.
if len(argv) != 2:
    print("python roster.py houseName")
    exit(1)
# Did the user input a real housename? Case insensitive
user_input = argv[1].lower()
if user_input not in ['gryffindor', 'hufflepuff', 'ravenclaw', 'slytherin']:
    print("Wrong house name.")
    exit(1)

# query db for all students in that house
# db.execute("SELECT") # returns a dict of python dicts
# SELECT first, middle, last, birth FROM "students" WHERE house LIKE "Gryffindor" ORDER BY last, first;
db = SQL("sqlite:///students.db")
result = db.execute('SELECT first, middle, last, birth FROM students WHERE house LIKE "{}" ORDER BY last, first;' .format(user_input))

for row in result:
    # Printing results for when middle name is NULL:
    if not row['middle']:
            print('{} {}, born {}' .format(row['first'], row['last'], row['birth']))
    else:

        print('{} {} {}, born {}' .format(row['first'], row['middle'], row['last'], row['birth']))


