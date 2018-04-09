#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// declaring the function that will take in the text that the user want to cipher and the key and than the function will output the ciphered text.
char *change_letters(char *text, int key);

int main(int argc, char *argv[])
{
    // if the user types in exactly one parameter beside the program, the program will work.
    if (argc == 2)
    {
        char *text = get_string("Enter the text that you want ciphered: ");
        int key;
        // if the key is bigger then the english alphabets count then set the key to be the reminder when the first parameter is divided by 26, 26 being the count of the english alphabet.
        if (atoi(argv[1]) > 26)
        {
            key = atoi(argv[1]) % 26;
        }
        // else set the key to the first parameter.
        else
        {
            key = atoi(argv[1]);
        }
        char *ciphered_text = change_letters(text, key);
        printf("ciphertext: %s\n", ciphered_text);
    }
    // else the program outputs some text to the user and returns 1.
    else
    {
        printf("please enter a valid key as a parameter for the program to work.\n");
        return 1;
    }
}

// this function takes in the text and the key and outputs the ciphered text.
char *change_letters(char *text, int key)
{

    char *resulted_text = malloc(strlen(text) + 1);
    // loop over the text and cipher every letter.
    for (int i = 0; i < strlen(text); i++)
    {
        // check to see if the letter is lower case letter.
        if (text[i] < 123 && text[i] > 90)
        {
            int ciphered_letter = text[i] + key;
            // if the letter after ciphering is still a valid letter then set the i'th letter in the resulted_text array to the ciphered_letter.
            if (ciphered_letter < 123)
            {
                resulted_text[i] = ciphered_letter;
            }
            // else then start again from the letter a.
            else
            {
                int key_after_looping = ciphered_letter - 123;
                resulted_text[i] = 97 + key_after_looping;
            }
        }
        // else check to see if the letter is a punctuation mark. If so return the letter as it is.
        else if (text[i] < 65)
        {
            resulted_text[i] = text[i];
        }
        // else the letter is in Upper Case.
        else
        {
            if (text[i] + key < 91)
            {
                resulted_text[i] = text[i] + key;
            }
            else
            {
                int key_after_looping = text[i]  + key - 91;
                resulted_text[i] = 65 + key_after_looping;
            }
        }
    }
    return (resulted_text);
}
