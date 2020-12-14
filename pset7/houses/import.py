# A python script that imports csv into a SQL db.

import csv
from sys import argv, exit
import sys
import re
import cs50


# Checks if the right amount of user input arguments
if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

# Checks if the second arg really is a file with a csv extension
# https://stackoverflow.com/questions/37233018/check-for-valid-file-extension
user_input = argv[1]
if not user_input.endswith('.csv'):
    print("CSV file is needed.")
    exit(1)

# Opens up the SQL database
# Used this video to learn how to transfer csv into sqlite https://youtu.be/x8i9haDHR0s
db = cs50.SQL("sqlite:///students.db")

# Open csvfile
with open(argv[1], 'r', newline = '') as csv_file:

    # Putting the csv file into lists
    csv_reader = csv.reader(csv_file, delimiter=',')

    # Parsing the new lists into nicely trimmed strings and inserting em into the db.
    for row in csv_reader:
        # Counting the substrings. If no middle name elements = 4, otherwise = 5
        elements = len((str(row)).split(' '))

        # Splitting if no middle name, row needs to be converted to string so that split works.
        if elements == 4:
            first, last, house, birth = (str(row)).split()

            # Deleting all the unnecessary chars off the string via regex
            first = re.sub('[^A-Za-z0-9]+', '', first)
            last = re.sub('[^A-Za-z0-9]+', '', last)
            house = re.sub('[^A-Za-z0-9]+', '', house)
            birth = re.sub('[^A-Za-z0-9]+', '', birth)
            if last == 'FinchFletchley':
                last = 'Finch-Fletchley'
            # Inserting the newly created strings into the SQL db
            db.execute("INSERT INTO students (first, last, house, birth) VALUES(?, ?, ?, ?)", first, last, house, birth)

        # Splitting if middle name exists.
        elif elements == 5:
            first, middle, last, house, birth = (str(row)).split()

            # Deleting all the unnecessary chars off the string via regex
            first = re.sub('[^A-Za-z0-9]+', '', first)
            middle = re.sub('[^A-Za-z0-9]+', '', middle)
            last = re.sub('[^A-Za-z0-9]+', '', last)
            house = re.sub('[^A-Za-z0-9]+', '', house)
            birth = re.sub('[^A-Za-z0-9]+', '', birth)



            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", first, middle, last, house, birth)
