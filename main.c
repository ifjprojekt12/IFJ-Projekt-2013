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

// funkce pro kontrolni vypis obsahu stromu promennych/funkci
void print_tree(NODE a1, NODE a2)
{
    if( a1 != NULL )
    {
/*
        if(a1->params != NULL)
        {
            printf("%s, ", a1->params->key);
            print_tree(a1->params->LPtr, a1->params->RPtr);
        }
        else
*/
/*        printf("%s, ",a1->key);
        print_tree(a1->LPtr,a1->RPtr);

        printf("\n");
*/
/*
        if( a1->body != NULL && a1->body->first != NULL )
        {
            printf("%s:\n",a1->key);
            for( INSTRUCT a = a1->body->first; a != NULL; a = a->right )
            {
                printf("\tid: %d, typ: %d", a->number, a->id);
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
        else
            printf("%s: seznam instrukci je prazdny\n",a1->key);
*/
    }
    if( a2 != NULL )
    {
/*        if(a2->params != NULL)
        {
            printf("%s, ", a2->params->key);
            print_tree(a2->params->LPtr, a2->params->RPtr);
        }
        else
*/
/*        printf("%s, ",a2->key);
        print_tree(a2->LPtr,a2->RPtr);

        printf("\n");
*/
/*
        if( a2->body != NULL && a2->body->first != NULL )
        {
            printf("%s:\n",a2->key);
            for( INSTRUCT a = a2->body->first; a != NULL; a = a->right )
            {
                printf("\tid: %d, typ: %d", a->number, a->id);
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
        else
            printf("%s: seznam instrukci je prazdny\n",a2->key);
*/
    }
}

void free_node(NODE aux)
{
    if( aux != NULL )
    {
        free_node(aux->LPtr);
        free_node(aux->RPtr);
        free(aux->key);
        if( aux->params != NULL )
            free_node(aux->params);
        if( aux->body != NULL )
            free_instr_list(aux->body);
        if( aux->data.string != NULL )
            free(aux->data.string);
        if( aux->data.id_name != NULL )
            free(aux->data.id_name);
        free(aux);
    }
}

void free_all()
{
    free_node(root);
    free_node(tree);
    free_instr_list(&list);
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
        free_all();
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
                printf(", ass3: %s", a->result->key);
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
/*
    printf("strom promennych obsahuje:\n");
    if( root != NULL )
    {
        printf("%s, ",root->key);
        print_tree(root->LPtr,root->RPtr);
    }
    else
        printf("prazdny");
*/
/*
    printf("\nstrom funkci obsahuje:\n");
    if( tree != NULL )
    {
        printf("%s:\n",tree->key);
*/
/*
        if(tree->params != NULL)
        {
            printf("%s, ", tree->params->key);
            print_tree(tree->params->LPtr, tree->params->RPtr);
        }
        else
            printf("%s, ",tree->key);
        print_tree(tree->LPtr,tree->RPtr);
        printf("\n");
*/
/*
        if( tree->body->first != NULL )
        {
            for( INSTRUCT a = tree->body->first; a != NULL; a = a->right )
            {
                printf("\tid: %d, typ: %d", a->number, a->id);
                if( a->operand_1 != NULL )
                    printf(", ass1: %s", a->operand_1->key);
                if( a->operand_2 != NULL )
                    printf(", ass2: %s", a->operand_2->key);
                if( a->result != NULL )
                    printf(", ass3: %s", a->result->key);
                if( a->jump != NULL )
                    printf(", jump: %d (%d)", a->jump->number,a->jump->id);
                printf("\n");
                fflush(stdout);
            }   
        }
        else
            printf("seznam instrukci je prazdny\n");
        print_tree(tree->LPtr,tree->RPtr);
    }
    else
        printf("prazdny");
    printf("\n");
*/

    /* Po kontrole validace vstupu prechazime do interpretu */
    //printf("interpret:\n");fflush(stdout);
    interpret( &list );

    /* Odstraneni veskere pouzite pameti */
    //printf("mazani pameti:\n");fflush(stdout);
    free_all();

    return eCode;
}
