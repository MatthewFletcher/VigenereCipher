#!/usr/bin/env python3



def expandKey(phrase,key):
    '''
    I'm terribly sorry for the eyesore that is this line of code, but it was
    the easiest way to get it working. 
    In English:

    [key for _ in range(int(len(phrase)/len(key))+1)]
        -> Create list of key n times to the length of the phrase divided by
        the length of the key, plus an extra key instance (there will be
        overhang)
        -> ["vig", "vig", "vig", "vig", ""vig", "vig"]
    "".join() 
        -> Join the list of strings
        -> "vigvigvigvigvigvig"
    [0:len(phrase)]
        -> Get the substring that is exactly the length of the plain text 
        -> "vigvigvigvigvigv"
    '''
    return "".join([key for _ in range(int(len(phrase)/len(key))+1)])[0:len(phrase)]

def shiftLetter(char, shiftChar):
    '''
    Shift the given letter by the value indicated by the shift character. 
    '''
    shift = ord(shiftChar)-ord("A" if shiftChar.isupper() else "a")

    #These few lines are semi-copied from 
    # https://www.thecrazyprogrammer.com/2018/05/caesar-cipher-in-python.html

    if char.isupper():
        return chr((ord(char) + shift - 65) % 26 + 65)
    elif char.islower():
        return chr((ord(char) + shift - 97) % 26 + 97)
    else:
        return char


def encrypt(phrase, key):
    """
    Pair the phrase and expanded key (they will be the same size) and perform
    the shifting. 
    """
    ret = ""
    for x,y in zip(phrase,expandKey(phrase,key)):
        ret += shiftLetter(x,y)
    return ret


def main():
    phrase = "THE BOY HAS THE BALL"
    key = "vig"
    print(f"Original string: {phrase}")
    print(f"Encrypted string: {encrypt(phrase, key)}")

if __name__ == "__main__":
    main()
