CC=gcc
CFLAGS= -g -std=c11 -pedantic -Wall -Wextra

all: scanner

run: scanner
	./scanner

scanner: scanner.o
	$(CC) $(CFLAGS) scanner.o -o scanner

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

clean:
	rm *.o scanner
