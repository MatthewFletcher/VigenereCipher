CC=gcc
CFLAGS= -O0 -fopenmp -Wall -std=gnu11 -pg
PT= -DDEBUG="TRUE"
CT= 
EXEC= -o decrypt


decrypt: decrypt.c 
	$(CC)  $(CFLAGS) decrypt.c $(EXEC) $(CT)

debug: decrypt.c 
	$(CC)  $(CFLAGS) decrypt.c $(EXEC) $(PT)

