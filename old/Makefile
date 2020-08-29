CC=gcc
CFLAGS=
EXEC=decrypt

DICT=/usr/share/dict/american-english
MYDICT=/tmp/myDict.txt

decrypt: decrypt.c
	PATH=PATH:.
	$(CC) decrypt.c $(CFLAGS) -o $(EXEC)
