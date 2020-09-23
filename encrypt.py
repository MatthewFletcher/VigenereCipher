#!/usr/bin/env python3

import sys


def shiftChr(orig, shiftChar, encrypt=True):
    if encrypt:
        return chr((ord(orig)+ord(shiftChar))%127)
    else:
        return chr((ord(orig)-ord(shiftChar))%127)


def encryptStr(phrase,key):
    expandedKey = "".join([key for _ in range(int(len(phrase)/len(key))+1)])[0:len(phrase)]
    
    cipherText = ""
    for pt,kt in zip(phrase,expandedKey):
        #print(f"char {pt} with key {kt} = {shiftChr(pt,kt)}")
        cipherText += shiftChr(pt,kt)
    return cipherText

def decryptStr(phrase,key):
    expandedKey = "".join([key for _ in range(int(len(phrase)/len(key))+1)])[0:len(phrase)]
    
    plainText = ""
    for pt,kt in zip(phrase,expandedKey):
        #print(f"char {pt} with key {kt} = {shiftChr(pt,kt, encrypt=False)}")
        plainText += shiftChr(pt,kt,encrypt=False)
    return plainText

def encryptFile(fileName, key):
    data = None
    with open(fileName, "r") as f:
        data = f.read()
    with open(f"{fileName}.vig", "wb") as f:
        f.write(str.encode(encryptStr(data,key)))

def decryptFile(fileName, key):
    data = None
    with open(fileName, "r") as f:
        data = f.read()
    with open(f"{fileName[:-4]}", "w") as f:
        f.write(decryptStr(data,key))
        

def main():
    if len(sys.argv) < 3:
        print("ERROR: not enough arguments")
        return;
    inFile = sys.argv[1];
    key = sys.argv[2];
    encryptFile(inFile, key)


if __name__ == "__main__":
    main()
