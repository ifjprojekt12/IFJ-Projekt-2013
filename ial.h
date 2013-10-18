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

typedef struct NODE
{
	char *key; // klic = jmeno promenne / funkce
	TOKEN data;
    LIST *params;

	struct NODE *LPtr;                                    // levy podstrom
	struct NODE *RPtr;                                   // pravy podstrom
} *NODE;

#endif // IAL_H_INCLUDED


void treeInit (NODE*);
void insertVarToTree (char *, TOKEN, NODE*);
NODE searchIdent (char *, NODE*);
NODE copyTree (NODE*, NODE*);
