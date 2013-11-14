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

// funkce pro kontrolni vypis obsahu stromu promennych/funkci
void print_tree(NODE a1, NODE a2)
{
    if( a1 != NULL )
    {
        if(a1->params != NULL)
        {
            printf("%s: %s, ", a1->key,a1->params->key);
            print_tree(a1->params->LPtr, a1->params->RPtr);
        }
        else
            printf("%s, ",a1->key);
        print_tree(a1->LPtr,a1->RPtr);
    }
    if( a2 != NULL )
    {
        if(a2->params != NULL)
        {
            printf("%s: %s, ", a2->key,a2->params->key);
            print_tree(a2->params->LPtr, a2->params->RPtr);
        }
        else
            printf("%s, ",a2->key);
        print_tree(a2->LPtr,a2->RPtr);
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
    	return eCode;

    //printf("syntaxe + semantika v poradku.\n"); fflush(stdout);
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
            printf(", ass1: %s", a->operand_1->key);
        if( a->operand_2 != NULL )
            printf(", ass2: %s", a->operand_2->key);
        if( a->result != NULL )
            printf(", ass3: %s", a->result->key);
        if( a->jump != NULL )
            printf(", jump: %d", a->jump->number);
        printf("\n");
        fflush(stdout);
    }
    }
*/
/*
    printf("strom promennych obsahuje:\n");
    if( root != NULL )
    {
        printf("%s, ",root->key);
        print_tree(root->LPtr,root->RPtr);
    }
    else
        printf("prazdny");
    printf("\nstrom funkci obsahuje:\n");
    if( tree != NULL )
    {
        if(tree->params != NULL)
        {
            printf("%s: %s, ", tree->key,tree->params->key);
            print_tree(tree->params->LPtr, tree->params->RPtr);
        }
        else
            printf("%s, ",tree->key);
        print_tree(tree->LPtr,tree->RPtr);
    }
    else
        printf("prazdny");
    printf("\n");
*/

    /* Po kontrole validace vstupu prechazime do interpretu */
    //printf("interpret:\n"); fflush(stdout);
    interpret( &list );

    /* Odstraneni veskere pouzite pameti */
    // TODO

    return eCode;
}
