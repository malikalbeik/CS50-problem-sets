#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int number;
    // ask for the number till the user inputs a valid number.
    do
    {
        number = get_int("Enter an int: ");
    }
    while (number > 23 || number < 0);
    // Do this on every line
    for (int i = 0; i < number; i++)
    {
        // prints the initial spaces to make it look like a half pyramid.
        for (int k = 0; k < number - i - 1; k++)
        {
            printf(" ");
        }
        printf("#");
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}