CC=gcc
CFLAGS= -O3 -fopenmp -Wall
EXEC= -o decrypt

decrypt: decrypt.c
	$(CC)  $(CFLAGS) decrypt.c $(EXEC)
