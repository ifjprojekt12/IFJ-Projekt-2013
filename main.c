#include "lexical.h"
#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>



int main(int argc, char *argv[])
{
	/*Soubor se nacte pres parametr*/
	if(argc != 2)
	{
		printf("Chybi argument!\n");
		return 0;
	}
	if(open_source(argv[1]) != 0)
	{
		printf("nepodarilo se otevrit soubor \"%s\".\n", argv[1]);
		return 1;
	}

	/*Po otevreni souboru prechazime do syntaxe*/
	syntaxer();

	return EXIT_SUCCESS;
}
