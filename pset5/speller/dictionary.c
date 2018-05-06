// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define WORD_MAX 55
#define HASH_SIZE 500000

typedef struct node {
    char val[WORD_MAX];
    struct node * next;
} linked_list;

// create a counter to help with counting the number of words in a dictionary.
int dictionary_counter = 0;

// create the hash table and set it to NULL.
linked_list *hash_table[HASH_SIZE] = {NULL};

// defining the hash function.
// this hash function's name is djb2 and it's done by Dan Bernstein.
// you can find the algrothim at http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c = 0;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    if (hash > HASH_SIZE)
    {
        hash = hash % HASH_SIZE;
    }
    return hash;
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char temp[strlen(word) + 1];
    int length = strlen(word);
    for(int i = 0; i < length; i++)
        temp[i] = tolower(word[i]);
    temp[length] = '\0';

    int index = hash(temp);

    // if hashtable is empty at index, return false
    if (hash_table[index] == NULL)
    {
        return false;
    }
    // create cursor to compare to word
    linked_list *cursor = hash_table[index];
    
    // if empty at index iterate till cursor is NULL
    while (cursor != NULL)
    {
        if (strcmp(temp, cursor->val) == 0)
        {
            return true;
        }
        else {
            cursor = cursor->next;
        }
    }
    
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char buffer[WORD_MAX];

    // opening the dictionary file to read from.
    FILE *file = fopen(dictionary, "r");

    // error handling for if the dictionary file couldn't be opened.
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return 2;
    }
    while (fgets(buffer, WORD_MAX, file) != NULL)
    {
        // increment the dictionary_counter
        dictionary_counter++;

        // removing the \n from the buffer and changing it to all lowercase letters.
        strtok(buffer, "\n");
        for (int i = 0; i < strlen(buffer); i++)
        {
            buffer[i] = tolower(buffer[i]);
        }

        int index = hash(buffer);

        linked_list *n1;
        n1 = malloc(sizeof(linked_list));
        strcpy(n1->val, buffer);
        // n1->val = buffer;
        n1->next = NULL;
        if (hash_table[index] == NULL )
        {
            hash_table[index] = n1;
        }
        else
        {
            n1->next = hash_table[index];
            hash_table[index] = n1;
        }
    }
    
    // close file
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return false;
}
