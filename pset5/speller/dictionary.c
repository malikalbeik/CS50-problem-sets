// Implements a dictionary's functionality

#include <stdbool.h>

#include "dictionary.h"

#define SIZE 20


// defining the linked list's node.
typedef struct node {
    int val;
    struct node * next;
} node_t;

// defining the hash function.
// this hash function's name is djb2 and it's done by Dan Bernstein.
// you can find the algrothim at http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // TODO
    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
