#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// declaring the functions
int is_alphabetical(char *text_to_check);
char *change_letters(char *text, char *key_word);

int main(int argc, char *argv[])
{
    // check to see if the user entered the right parameter.
    if (argc == 2 && is_alphabetical(argv[1]))
    {
        char *text = get_string("Enter the text that you want ciphered: ");
        char *key_word = argv[1];

        char *ciphered_text = change_letters(text, key_word);
        printf("ciphertext: %s\n", ciphered_text);
    }
    // else the program outputs some text to the user and returns 1.
    else
    {
        printf("please enter a valid key as a parameter for the program to work.\n");
        return 1;
    }
}

// this function takes in the key_word and checks if all the letters in the word are alphabetic.
int is_alphabetical(char *text_to_check)
{
    for (int i = 0; i < strlen(text_to_check); i++)
    {
        if (!isalpha(text_to_check[i]) && !(text_to_check[i] == ' '))
        {
            return false;
        }
    }
    return true;
}

// this function takes in the text and the key_word and outputs the ciphered text.
char *change_letters(char *text, char *key_word)
{

    char *resulted_text = malloc(strlen(text) + 1);
    int punctuation_count = 0;
    // loop over the text and cipher every letter.
    for (int i = 0; i < strlen(text); i++)
    {
        char key_letter = key_word[(i - punctuation_count) % strlen(key_word)];
        int key;
        if (key_letter < 123 && key_letter > 96)
        {
            key = key_letter - 97;
        }
        else if (key_letter < 91 && key_letter > 64)
        {
            key = key_letter - 65;
        }
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
        // else check to see if the letter is a punctuation mark or a space. If so return the letter as it is.
        else if (text[i] < 65 || text[i] == 32)
        {
            resulted_text[i] = text[i];
            punctuation_count++;
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
