from cs50 import get_int

# ask for the number till the user inputs a valid number.
while True:
    number = get_int("Enter an int: ")
    if (number < 24 and number > - 1):
        break

# Do this on every line
for i in range(number):
    # prints the initial spaces to make it look like a half pyramid.
    for k in range(number - i - 1):
        print(" ", end="")

    # print the first half of the pyramid.
    for j in range(i + 1):
        print("#", end="")

    print("  ", end="")

    # prints the second half of the pyramid.
    for j in range(i + 1):
        print("#", end="")

    # prints a line at the end of every line.
    print("\n", end="")
