#include "lexical.h"
#include "syntax.h"
#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

int eCode;
NODE root;
LIST_3AK list;

int main(int argc, char *argv[])
{
	eCode = sOK;
	/* Soubor se nacte pres parametr */
	if(argc != 2)
	{
		fprintf(stderr, "Chybi argument!\n");
		return sINTERN;
	}

	if(open_source(argv[1]) != 0)
	{
		fprintf(stderr, "nepodarilo se otevrit soubor \"%s\".\n", argv[1]);
		return sINTERN;
	}

	/* Po otevreni souboru prechazime do syntaxe, ktera vyvolava i semantickou kontrolu */
	if( syntaxer() == EXIT_FAILURE )
    	return eCode;

/*
    // kontrola listu instrukci pro skoky (c) Kuba
    for( INSTRUCT a = list.first; a != NULL; a = a->right )
    {
        printf("%d", a->id);
        if( (a->id >= 10 && a->id <= 15 ) || a->id == 32 )   // porovnani nebo skok
        {
            if( a->jump != NULL )
                printf(" (-> %d), ", a->jump->id);
            else
                printf(" (-> null), ");
        }
        else
            printf(", ");
    }
    printf("\n");
*/

    /* Po kontrole validace vstupu prechazime do interpretu */
    interpret( &list );

    /* Odstraneni veskere pouzite pameti */
    // TODO

    return eCode;
}
