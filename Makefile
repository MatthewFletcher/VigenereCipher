CC=gcc
CFLAGS= -O0 -fopenmp -Wall -std=gnu11 -pg
EXEC= -o decrypt

decrypt: decrypt.c
	$(CC)  $(CFLAGS) decrypt.c $(EXEC)
