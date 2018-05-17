import sys
import crypt


def main():
    if len(sys.argv) == 2:
        brute('', 5, sys.argv[1])
    else:
        print("please enter a valid hashed password.")
        return 1


def brute(string, length, hashedPass):
    """takes in a string a length and the hahed password and tries every possible world hashes it and compares it to the already hashed password"""
    charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
    # getting the salt from the first two chars in the hashed password.
    salt = hashedPass[:2]
    # if the string's length becomes the same as the given length then stop the recursion.
    if len(string) == length:
        return
    # else try again and again till the password is found.
    for char in charset:
        temp = string + char
        # print(f"failed try: {temp}")
        hashedTemp = crypt.crypt(temp, salt)
        if hashedTemp == hashedPass:
            print(temp)
            sys.exit()
        brute(temp, length, hashedPass)


if __name__ == "__main__":
    main()