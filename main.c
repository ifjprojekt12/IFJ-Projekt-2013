#include "lexical.h"
#include "syntax.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>



int main(int argc, char *argv[])
{
	int eCode;
	/*Soubor se nacte pres parametr*/
	if(argc != 2)
	{
		//TODO ODSTRANIT  printf
		printf("Chybi argument!\n");
		return sINTERN;
	}
	if(open_source(argv[1]) != 0)
	{
		//TODO ODSTRANIT  printf
		printf("nepodarilo se otevrit soubor \"%s\".\n", argv[1]);
		return sINTERN;
	}

	/*Po otevreni souboru prechazime do syntaxe*/
	eCode = syntaxer();
	return eCode;
}
