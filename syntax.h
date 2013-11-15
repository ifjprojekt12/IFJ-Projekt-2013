#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "semantic.h"

#define END_B   30	// bracket
#define END_S   31	// semicolon
#define I_MAX   512

INSTRUCT aux;
tSInstr InstrStack;
tSInstr InstrFor;
NODE check_func;
NODE func;
bool func_end;
int m,i;      // velikost pole tokenu

extern NODE tree;
extern NODE root;
extern LIST_3AK list;
extern int eCode;

// hlavni funkce syntaxe
int syntaxer();

// funkce pro inicializaci pole tokenu
int initialize_array(TOKEN**array);

int realloc_array(TOKEN*array);

// funkce pro zpracovani vyrazu, array - pole tokenu, i - pozice v poli, unit - prvni token vyrazu, ending - ocekavany konec vyrazu
int expression(TOKEN*array, int i, TOKEN unit, int ending);

#endif
