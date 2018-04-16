// Helper functions for music

#include <cs50.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // TODO
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // TODO
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // if the first char of s is \0 than the given string is empty return true.
    if (s[0] == '\0')
    {
	return (true);
    }
    // else the string is not a rest return false.
    else
    {
	return (false);
    }
}
