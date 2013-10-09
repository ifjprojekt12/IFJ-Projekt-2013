#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "lexical.h"

#define TRUE 1
#define FALSE 0

typedef struct LIST
{
    char *param;
    struct LIST *next;
} *LIST;

typedef struct node
{
	char *key; // klic = jmeno promenne / funkce
	TOKEN data;
    LIST *params;

	struct node *LPtr;                                    // levy podstrom
	struct node *RPtr;                                   // pravy podstrom
} *node;



#endif // IAL_H_INCLUDED

void treeInit (node*);
void varInit (node*);
