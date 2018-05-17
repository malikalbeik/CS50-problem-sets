from cs50 import get_int
import math
import sys

def main():
    cardnumber = get_int("Enter the card's number: ")

    # check if the number is smaller than 13 digits, if so then the card is invalid.
    if len(str(cardnumber)) < 13:
        print("INVALID")
        sys.exit(0)

    sum = calcSum(cardnumber)

    if sum % 10 == 0:
        print(checkCardType(cardnumber))
    else:
        print("INVALID")

def calcSum(cardnumber):
    """calculates the Luhn’s sum of the card number"""
    firstnumbers = str(cardnumber)[len(str(cardnumber))::-2]
    secondnumbers = str(cardnumber)[len(str(cardnumber))-2::-2]

    firstnumberssum, secondnumberssum = 0, 0

    for i in range(len(firstnumbers)):
        firstnumberssum += int(firstnumbers[i])

    for i in range(len(secondnumbers)):
        if int(secondnumbers[i])*2 > 9:
            for digit in str(int(secondnumbers[i])*2):
                secondnumberssum += int(digit)
        else:    
            secondnumberssum += int(secondnumbers[i])*2
    return firstnumberssum + secondnumberssum



def checkCardType(cardnumber):
    """checks the card type depending on the first numbers of the card"""
    if int(str(cardnumber)[:2]) in [34, 37]:
        return "AMEX"
    elif int(str(cardnumber)[:2]) in [51, 52, 53, 54, 55]:
        return "MASTERCARD"
    elif int(str(cardnumber)[0]) == 4:
        return "VISA"

if __name__ == "__main__":
    main()