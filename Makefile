RM=rm
CC=gcc
CFLAGS=-c -std=c99 -pedantic -Werror -Wall -g

#timto se sestavi spustitelny soubor z *.o binarek
interpret: data_struct.o ial.o function.o stack.o lexical.o interpret.o semantic.o syntax.o main.o
	$(CC) data_struct.o ial.o function.o stack.o lexical.o interpret.o semantic.o syntax.o main.o -lm -o interpret

clean:
	$(RM)   *.o interpret 2>/dev/null
