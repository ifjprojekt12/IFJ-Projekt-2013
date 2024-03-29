#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>
#include "lexical.h"

#define TRUE 1
#define FALSE 0

#define STACKSIZE 1024
#define BUFFERSIZE 1024

#define DEBUG printf("blablabla")

extern int eCode;
struct list3_ak;

// linearni seznam pro parametry funkci
typedef struct LIST
{
    char *param; // parametr
    struct LIST *next; //  ukazatel na dalsi polozku
} *LIST;

// binarni strom - samotna struktura je polozkou, ktera ukazuje na svoje podstromy
typedef struct NODE
{
	char *key; // klic = jmeno promenne / funkce
	TOKEN data; // struktura s daty
    struct NODE *params; // linearni seznam parametru pro funkce
    struct list_3ak *body;
    int position;

	struct NODE *LPtr;                                    // levy podstrom
	struct NODE *RPtr;                                   // pravy podstrom
} *NODE;

// zasobnik tabulek symbolu
typedef struct STACK
{
    NODE *ptr; // ukazatel na tabulku
    struct STACK *next; // ukazatel na polozku nad, pokud je NULL, tak je prvek na vrchu zasobniku
} *STACK;

#endif // IAL_H_INCLUDED

// tabulka symbolu
void treeInit (NODE*);
void insertVarToTree (char *, TOKEN, NODE*);
NODE searchIdent (char *, NODE*);
NODE searchParam(int, NODE*);
NODE copyTree (NODE*);

// zasobnik
void stackInit (STACK*);
void push (STACK*, NODE*);
NODE pop (STACK*);

// vestavene funkce
char * my_sort_string (char *);
int find_string (char *, char *);
