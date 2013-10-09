RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -pedantic -Werror

#timto se sestavi spustitelny soubor z *.o binarek
interpret: lexical.o main.o syntax.o stack.o
	$(CC) lexical.o main.o syntax.o stack.o -lm -o interpret

clean:
	$(RM)   *.o
