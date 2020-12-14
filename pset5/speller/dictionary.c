// Implements a dictionary's functionality

#include <stdbool.h>
//adding all the libraries:
#include <ctype.h>
#include <string.h>
#include <strings.h> //for the specific strcmp
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1; // Gotta change it to 114473 later when I start using the real dictionary

// Hash table
node *table[N] = {0};

// My word count variable, global cause can't change function defs
unsigned int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // hash the word
    unsigned int hash_index;
    hash_index = hash(word);

    // access the table at the hash_index
    node *search_pointer = table[hash_index];
    // traverse the linked list until you've found the word, return true
    while (search_pointer)
    {
        if (strcasecmp(search_pointer -> word, word) == 0)
        return true;
        search_pointer = search_pointer -> next;
    }
    // If it's gotten to NULL, returns false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Source: https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn/
    // Though I've heard djb2 is better eh
    unsigned int hash_index = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash_index = (hash_index << 2) ^ word[i];
    }
    return hash_index % N;

}


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char tmp_word[LENGTH + 1] = {0};
    // debug50 ./speller dictionaries/test texts/test.txt
    // open up dictionary with fopen, check for NULL
    FILE *dict;
    dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        unload();
        printf("Failed to allocate memory for the dictionary.\n");
        return false;
    }
    // read strings from file one at a time with fscanf
    while(fscanf(dict, "%s", tmp_word) != EOF)
    {
        // create a new node for each word (use malloc)
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            printf("Failed to allocate memory for the node.\n");
            return false;
        }

        // use the hash function in the word, atm only gives a zero
        unsigned int hash_index = hash(tmp_word);

        // insert node into hash table like how you'd normally do it with a linked list
        strcpy(new_node -> word, tmp_word);
        new_node -> next = table[hash_index];
        table[hash_index] = new_node;
        // Adding word count for the size function
        word_count++;
    }

    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // using the global variable to save time a bit
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *current_node, *previous_node;
    unsigned int count_buckets = 0;

    // iterate from 0 to N-1 to traverse the array, then delete each list
    for (unsigned int bucket = 0; bucket < N; bucket++)
    {
        // Using a trailing pointer to keep track of the node I want to delete
        current_node = table[bucket];
        previous_node = NULL;

        while (current_node)
        {
            // advancing in the list to the next node
            previous_node = current_node;
            current_node = current_node -> next;
            free(previous_node);

        }
        count_buckets++;
    }

    if (count_buckets == N)
        return true;
    else
        return false;
}
