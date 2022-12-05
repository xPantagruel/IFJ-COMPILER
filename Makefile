CC=gcc
CFLAGS= -g -std=c11 -pedantic -Wall -Wextra

all: parser

run: parser
	./parser

copy:
	sshpass -p ${pass} rsync -a  /home/xpetri25/School/IFJ/IFJ xpetri25@merlin.fit.vutbr.cz:/homes/eva/xp/xpetri25/ifj --exclude='tests' --exclude='.git'

test: 
	python3 test.py ./parser

memcheck: parser
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./parser < testFile.txt

parser: parser.o scanner.o bottomUp.o stack.o symtable.o code_generation.o builtInFunctions.o
	$(CC) $(CFLAGS) parser.o scanner.o bottomUp.o stack.o symtable.o code_generation.o builtInFunctions.o -o parser

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

bottomUp.o: bottomUp.c
	$(CC) $(CFLAGS) -c bottomUp.c -o bottomUp.o

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c -o stack.o

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c -o scanner.o

symtable.o: symtable.c
	$(CC) $(CFLAGS) -c symtable.c -o symtable.o

builtInFunctions.o: builtInFunctions.c
	$(CC) $(CFLAGS) -c builtInFunctions.c -o builtInFunctions.o


code_generation.o: code_generation.c
	$(CC) $(CFLAGS) -c code_generation.c -o code_generation.o
clean:
	rm *.o parser