#!/usr/bin/env python3

import string
import itertools

KEYLEN = 3
PHRASE = list("OPKJUTNVA")

alphabet = string.ascii_uppercase

def getLetterPos(c):
    return ord(c) - ord('A')

def shiftLetter(orig, shift):
    return chr((getLetterPos(orig) - getLetterPos(shift))%26 + ord('A'))

def tryKey(key):
    testKey = list(key)
    otherCt = 0
    for idx in range(len(PHRASE)):
        #Skip spaces 
        if not PHRASE[idx].isalpha():
            otherCt += 1
            continue

        #TEMPPHRASE[idx] = chr((ord(PHRASE[idx]) - ord(testKey[(idx-otherCt)%KEYLEN]) + ord('A'))%26 + 65)
        TEMPPHRASE[idx] = shiftLetter(PHRASE[idx], testKey[(idx+otherCt)%KEYLEN])
    print("".join(TEMPPHRASE),"|", "".join(testKey))
 

def main():
    for i,j,k in itertools.product(alphabet,alphabet,alphabet):
        print()
        PHRASE = list("OPKJUTNVA")
        TEMPPHRASE = PHRASE
        testKey = [i,j,k]
        otherCt = 0
        for idx in range(len(PHRASE)):
            #Skip spaces 
            if not PHRASE[idx].isalpha():
                otherCt += 1
                continue
            print(f"\tLetter{PHRASE[idx]} - {[testKey[(idx+otherCt)%KEYLEN]]} ----> {shiftLetter(PHRASE[idx], testKey[(idx+otherCt)%KEYLEN])}")
            #TEMPPHRASE[idx] = chr((ord(PHRASE[idx]) - ord(testKey[(idx-otherCt)%KEYLEN]) + ord('A'))%26 + 65)
            TEMPPHRASE[idx] = shiftLetter(PHRASE[idx], testKey[(idx+otherCt)%KEYLEN])
        print("".join(TEMPPHRASE),"|", "".join(testKey))

if __name__ == "__main__":
    main()

