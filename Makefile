CC=gcc
CFLAGS= -O3 -fopenmp
EXEC= -o decrypt

decrypt: decrypt.c
	$(CC)  $(CFLAGS) decrypt.c $(EXEC)
