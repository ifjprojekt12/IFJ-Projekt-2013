RM=rm
CC=gcc
CFLAGS=-c -std=c99 -pedantic -Werror -Wall

#timto se sestavi spustitelny soubor z *.o binarek
interpret: function.o stack.o data_struct.o ial.o lexical.o interpret.o semantic.o syntax.o main.o
	$(CC) function.o stack.o data_struct.o ial.o lexical.o interpret.o semantic.o syntax.o main.o -lm -o interpret

clean:
	$(RM)   *.o interpret
