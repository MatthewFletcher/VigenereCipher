CC=gcc
CFLAGS= -Ofast -fopenmp -Wall -std=gnu11 -pg
EXECNAME=decrypt
EXEC= -o $(EXECNAME)
PTFILE= text.txt
WORDLIST=/usr/share/dict/american-english 
MESSAGELENGTH=1000

decrypt: decrypt.c decrypt.h Makefile
	$(CC)  $(CFLAGS) decrypt.c $(EXEC) 

generateMessage:
	@shuf -n $(MESSAGELENGTH) $(WORDLIST) | tr '\n' ' ' > text.txt

encrypt:
	@./encrypt.py $(PTFILE) 'zyx'

run:
	@./$(EXECNAME)
