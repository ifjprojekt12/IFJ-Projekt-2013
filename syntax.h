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

NODE check_func;
INSTRUCT aux;
tSInstr InstrStack;

extern NODE root;
extern LIST_3AK list;
extern int eCode;

// vyctovy typ pro ukladani stavu na zasobnik pro kontrolu spravnou posloupnost podminenych vyrazu
enum conditions {
    cIF,
    cELSE,
    cELSEIF,
    cWHILE,
    cFOR,
    cFUNCTION,
    cREADY,
};


// funkce pro vypis chybovych zprav
void printERR(int err);

// hlavni funkce syntaxe
int syntaxer();

// funkce pro inicializaci pole tokenu
void initialize_array(TOKEN*array);

// funkce pro zpracovani vyrazu, array - pole tokenu, i - pozice v poli, unit - prvni token vyrazu, ending - ocekavany konec vyrazu
int expression(TOKEN*array, int i, TOKEN unit, int ending);

#endif
