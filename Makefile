CC=gcc
CFLAGS= -O3 -fopenmp -Wall -std=gnu11
EXEC= -o decrypt

decrypt: decrypt.c
	$(CC)  $(CFLAGS) decrypt.c $(EXEC)
