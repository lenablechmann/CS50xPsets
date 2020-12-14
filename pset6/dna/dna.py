# Calculates STR counts from a DNA sequence and compares to a DNA database
# to find a person that matches

# for parsing csv, and for argv:
import csv
from sys import argv, exit
import sys
import re

# gets 2 argv from user: first database.csv, second a text file with the DNA sequence
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# read text file into a string:
with open(sys.argv[2], 'r', newline = '') as txt_file:
    suspect_reader = txt_file.readlines()
    for line in suspect_reader:
        suspect_dna = line

# parse first line of csv to use it for sequence search
with open(sys.argv[1], 'r', newline = '') as csv_file:
    csv_reader = csv.DictReader(csv_file)

    # parsing the first line of the csv into a string.
    nucleobases = csv_reader.fieldnames

    # getting rid of first word via pop() and invisibly closing the file
    nucleobases.pop(0)

# {} creates a dictionary
suspect_sequences_storage = {}

# goes through the nucleobases "row" we got from the csv and stores them as keys in the dict
# sequences will be values (zeroed at the start). [ for indexing
for name in nucleobases:
    suspect_sequences_storage[name] = 0

# Finding the longest substring in the text file, now turned string
for key in nucleobases:
    key_length = len(key)
    counter = 0
    conseq_seq_max = 0
    position = 0

    # Using while loop to manually increment steps as needed
    while position < (len(suspect_dna)):
        # Counting the repeating substring
        if suspect_dna[position : position + key_length] == key:
            counter += 1
            position += key_length

        # resetting counter if the repeat has ended
        elif suspect_dna[position : position + key_length] != key:
            if counter > conseq_seq_max:
                conseq_seq_max = counter
            counter = 0
            position += 1
    # storing our values in the special dictionary
    suspect_sequences_storage[key] += conseq_seq_max

# testing if the special dictionary works via print
# print(suspect_sequences_storage)


# store csv (not the first row) as a dictionary
# https://towardsdatascience.com/15-things-you-should-know-about-dictionaries-in-python-44c55e75405c
# compare your simple dictionary to each "row" of that csv dictionary

with open(sys.argv[1], 'r', newline = '') as csv_file:
    csv_reader = csv.DictReader(csv_file)
    sequences = int(len(suspect_sequences_storage))
    

    # comparing counts in the database to the suspect sequence counts
    for name in csv_reader:
        # counting matches and always resetting
        count = 0
        for number in suspect_sequences_storage:
            
            suspect = int(suspect_sequences_storage[number])
            rowling_char = int(name[number])
            
            if suspect == rowling_char:
                count += 1
            else:
                continue
        # printing the find
        if count == sequences:
            print(name["name"])
            exit()

print("No match")