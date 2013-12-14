/**

Implementace interpretu imperativniho jazyka IFJ13.

xfiser10 - Jakub Fiser

*/

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
NODE tree;
LIST_3AK list;

void free_all()
{
    printf("root:\n");
    free_node(root);
    printf("tree:\n");
    free_node(tree);
    if( list.first != NULL ) {
        printf("list:\n");
        free_instr_list(&list);
        }
}

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
    {
        //free_all();
    	return eCode;
    }

/*
    printf("Seznam instrukci:\n");
    if( list.first == NULL )
    {
        printf("prazdny list instrukci!!\n");fflush(stdout);
    }
    else
    {
        // kontrola listu instrukci pro skoky (c) Kuba
        for( INSTRUCT a = list.first; a != NULL; a = a->right )
        {
            printf("id: %d, typ: %d", a->number, a->id);
            if( a->operand_1 != NULL )
            {
                printf(", ass1: %s", a->operand_1->key);
            }
            if( a->operand_2 != NULL )
            {
                printf(", ass2: %s", a->operand_2->key);
            }
            if( a->result != NULL )
            {
                printf(", result: %s", a->result->key);
            }
            if( a->jump != NULL )
            {
                printf(", jump: %d (%d)", a->jump->number, a->jump->id);
            }
            //if( a->left != NULL )
                //printf(", left: %d", a->left->number);
            //if( a->right != NULL )
                //printf(", right: %d", a->right->number);
            printf("\n");
        }
    }
*/

    /* Po kontrole validace vstupu prechazime do interpretu */
    //printf("interpret:\n");fflush(stdout);
    interpret( &list );

    /* Odstraneni veskere pouzite pameti */
    //printf("mazani pameti:\n");fflush(stdout);
    //free_all();

    return eCode;
}
