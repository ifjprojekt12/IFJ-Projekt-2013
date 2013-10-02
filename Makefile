RM=rm
CC=gcc
CFLAGS=-c -Wall -std=c99 -pedantic -Werror

#timto se sestavi spustitelny soubor z *.o binarek
interpret: lexical.o syntax.o main.o
	$(CC) lexical.o syntax.o main.o -lm -o interpret

clean:
	$(RM)   *.o
