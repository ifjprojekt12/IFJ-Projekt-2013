/**

Implementace interpretu imperativniho jazyka IFJ13.

xhanak27 - Jan Hanak
xfiser10 - Jakub Fiser

*/
#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "semantic.h"

#define END_B   30	// bracket
#define END_S   31	// semicolon

INSTRUCT aux;           // pomocny ukazatel instrukce pro vytvareni skokovych instrukci v semantice
tSInstr InstrStack;
tSInstr InstrFor;
tSInstr InstrBreak;
NODE check_func;
NODE func;
bool func_end;

extern NODE tree;       // tabulka funkci
extern NODE root;       // tabulka promennych
extern LIST_3AK list;   // list instrukci
extern int eCode;       // navratovy kod

// hlavni funkce syntaxe
int syntaxer();

// funkce pro zpracovani vyrazu, array - pole tokenu, i - pozice v poli, unit - prvni token vyrazu, ending - ocekavany konec vyrazu
int expression(TOKEN*array, int i, TOKEN unit, int ending, int *m);

// funkce prochazejici celym stromem, kontroluje zda byly volane funkce definovane
int func_defined(NODE a1, NODE a2);

#endif
