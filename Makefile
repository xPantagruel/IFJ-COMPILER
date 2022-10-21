CC=gcc
CFLAGS= -g -std=c11 -pedantic -Wall -Wextra

all: parser

run: parser
	./parser

parser: parser.o scanner.o bottomUp.o stack.o
	$(CC) $(CFLAGS) parser.o scanner.o bottomUp.o stack.o -o parser

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

bottomUp.o: bottomUp.c
	$(CC) $(CFLAGS) -c bottomUp.c -o bottomUp.o

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c -o stack.o

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o


clean:
	rm *.o parser