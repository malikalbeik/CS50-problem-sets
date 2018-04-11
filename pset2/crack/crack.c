#include <stdio.h>
#define _GNU_SOURCE
#include <crypt.h>
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// definding the brute_force function so that we can use it in main.
void brute_force(char *buffer, int index, int max_length, char *hashed_password);

int main(int argc, char *argv[])
{
    // if the user types exactly 1 argument beside the program then execute the code.
    if (argc == 2)
    {
        int length = 5;
        char *buffer = malloc(length + 1);
        for (int i = 1; i <= length; i++)
        {
            brute_force(buffer, 0, i, argv[1]);
        }
        free(buffer); // emptying the buffer
    }
    // else print an error and return 1.
    else
    {
        printf("please enter a valid hashed password.\n");
        return 1;
    }
}

// this function takes in the buffer, index, the maximum length of the password, and the hashedpassword.
// what it does is it determines the salt and then loops to try out evey possible combination,
// encrypts them with the same salt and checks if the resulted hash equals the original hash if so then it prints the password and exits returning 0.
void brute_force(char *buffer, int index, int max_length, char *hashed_password)
{
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz""ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int alphabetSize = sizeof(alphabet) - 1;
    char *salt = malloc(3);
    strncpy(salt, hashed_password, 2);

    for (int i = 0; i < alphabetSize; i++)
    {
        buffer[index] = alphabet[i];
        if (index == max_length - 1)
        {
            char *hashed_buffer = crypt(buffer, salt);
            if (strcmp(hashed_buffer, hashed_password) == 0)
            {
                printf("%s\n", buffer);
                exit(0);
            }
        }
        else
        {
            brute_force(buffer, index + 1, max_length, hashed_password);
        }
    }
}