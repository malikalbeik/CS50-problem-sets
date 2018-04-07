#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // initializing variables that we be used later.
    int sumOfOthers = 0;
    int sumOfFirst = 0;
    long cardnumber = get_long("Enter a long: ");
    int number_length = log10(cardnumber) + 1;
    long long int dividByForOthers = 10;
    long long int dividBy = 1;

    // a loop that loops over the card numbers and adds the numbers together to calculate if the number is a valid card number.
    for (int i = 1; i <= number_length ; i++)
    {
        if (cardnumber / dividByForOthers != 0)
        {
            dividByForOthers = pow(10, (i + (i - 1)));
        }
        if (cardnumber / dividBy != 0)
        {
            dividBy = pow(10, (i + (i - 2)));
        }
        int otherNumMultiplication = ((cardnumber / dividByForOthers) % 10) * 2;

        // if the other number's multiplication is bigger than 9 then store the first digit in a integer and the second digit in another integer and then add them together in the sumOfOthers.
        if (otherNumMultiplication > 9)
        {
            int firstNum = otherNumMultiplication % 10;
            int secondNum = otherNumMultiplication / 10 % 10;
            sumOfOthers += firstNum + secondNum;
        }
        // else if the number's multiplication is less than 10 then just add the multiplication to the sumOfOthers.
        else
        {
            sumOfOthers += otherNumMultiplication;
        }
        // setting the sum of the first numbers
        sumOfFirst += (cardnumber / dividBy) % 10;
    }
    int sumOfAll = sumOfFirst + sumOfOthers;

    // if the last digit of the sum is 0 than further check if the card is valid.
    if (sumOfAll % 10 == 0)
    {
        int firstTwo = cardnumber / pow(10, (number_length - 2));

        // if the number length is 15 and the first two numbers are 34 or 37 then the card is an American express card.
        if (number_length == 15 && (firstTwo == 34 || firstTwo == 37))
        {
            printf("AMEX\n");
        }
        // if the number length is 16 and the first two numbers are between 50 and 56 then the catd is a mastercard.
        else if (number_length == 16 && (firstTwo > 50 && firstTwo < 56))
        {
            printf("MASTERCARD\n");
        }
        // if the numver length is 16 or 13 and the first number is 4 then the card is a visa card.
        else if ((number_length == 16 || number_length == 13) && (firstTwo / 10 == 4))
        {
            printf("VISA\n");
        }
        // else the card is invalid.
        else
        {
            printf("INVALID\n");
        }
    }
    // and if the last number of the sum is not 0 then the card is invalid.
    else
    {
        printf("INVALID\n");
    }
}