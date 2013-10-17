RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -pedantic -Werror

#timto se sestavi spustitelny soubor z *.o binarek
interpret: lexical.o main.o syntax.o stack.o data_struct.o interpret.o semantic.o ial.o function.o
	$(CC) lexical.o main.o syntax.o stack.o data_struct.o interpret.o semantic.o ial.o function.o -lm -o interpret

clean:
	$(RM)   *.o interpret
