CC=gcc
CFLAGS= -Ofast -fopenmp -Wall -std=gnu11 -pg
EXECNAME=decrypt
EXEC= -o $(EXECNAME)
PTFILE= text.txt
WORDLIST=/usr/share/dict/american-english 
MESSAGELENGTH=500
SHELL:=/bin/bash
define \n
endef

default:
	$(error Choose decrypt, generateMessage, encrypt KEY='xxx', or run)

decrypt: decrypt.c decrypt.h Makefile
	$(CC)  $(CFLAGS) decrypt.c $(EXEC) 

generateMessage:
	@shuf -n $(MESSAGELENGTH) $(WORDLIST) | tr '\n' ' ' > text.txt

encrypt:
	@KEY=$(ARGS)
	@printf "Encrypting $(PTFILE) with key $${KEY:?Please enter key with make encrypt KEY='xxx'}\n"
	@./encrypt.py $(PTFILE) $(KEY)

run:
	@./$(EXECNAME)
