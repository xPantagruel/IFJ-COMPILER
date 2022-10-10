CC=gcc
CFLAGS= -g -std=c11 -pedantic -Wall -Wextra

all: parser

run: parser
	./parser <test.txt

parser: parser.o scanner.o 
	$(CC) $(CFLAGS) parser.o scanner.o -o parser

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

clean:
	rm *.o parser
