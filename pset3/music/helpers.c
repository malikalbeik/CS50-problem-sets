// Helper functions for music


#include <cs50.h>
// #include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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
    float frequency;
    int octave;

    if (strlen(note) == 2)
    {
        octave = note[1] - '0';
    }
    else
    {
        octave = note[2] - '0';
    }

    // Depending on the letter calculate the frequency.
    switch (note[0])
    {
        case 'A'  :
            frequency = 440.0 * pow(2.0, octave - 4.0);
            break;

        case 'B'  :
            frequency = 440.0 * pow(2.0, 2.0 / 12.0) * pow(2.0, octave - 4.0);
            break;

        case 'C'  :
            frequency = 440.0 * pow(2.0, 3.0 / 12.0) * pow(2.0, octave - 5.0);
            break;

        case 'D'  :
            frequency = 440.0 * pow(2.0, 5.0 / 12.0) * pow(2.0, octave - 5.0);
            break;

        case 'E'  :
            frequency = 440.0 * pow(2.0, 7.0 / 12.0) * pow(2.0, octave - 5.0);
            break;

        case 'F'  :
            frequency = 440.0 * pow(2.0, 8.0 / 12.0) * pow(2.0, octave - 5.0);
            break;

        case 'G'  :
            frequency = 440.0 * pow(2.0, 10.0 / 12.0) * pow(2.0, octave - 5.0);
            break;
    }
    // if the second char is # or b than do the proper calculations to reduce or increase the frequency.
    switch (note[1])
    {
        case '#':
            frequency *= pow(2.0, (1.0 / 12.0));
            break;
        case 'b':
            frequency /= pow(2.0, (1.0 / 12.0));
            break;
    }
    // At the end return the frequency.
    return round(frequency);
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