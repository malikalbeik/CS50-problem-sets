// Helper functions for music

#include <cs50.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int denominator = fraction[0] - '0';
    int numerator = fraction[2] - '0';
    int multiply_by = 8 / numerator;
    return (denominator * multiply_by);
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // TODO
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    if (s[0] == '\0')
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

