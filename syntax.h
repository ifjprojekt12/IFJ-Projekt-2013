#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "semantic.h"

#define OK      0
#define END_B   30	// bracket
#define END_S   31	// semicolon
#define END_CB  32	// comma or bracket
#define END_SB  33	// semicolon or bracket
#define I_MAX   512

NODE root;
LIST_3AK list;

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


enum finalCode {
	sOK,		/* vse v poradku */
	sSyn,		/* chyba syntakticke struktury programy */		
	sSemFceDef, /* nedefinovana funkce nebo redefinovana */
	sSemFceParam,	/* chybejici parametr pro volani funkce */
	sSemVar,	/*  Nedeklarovana promenna */
	sSynZero = 10,	/* Chyba deleni nulou */
	sSynTypeConv,	/* pretypovani promenne na cislo funkce doubleval */
	sSynCompatib,	/* chyba typove kompatibility v aritmetickych a relacnich vyrazech */
	sSynRest,	/* ostatni chyby semantiky */
	sINTERN = 99,   /* interni chyba interpretu tj. neovlivnena vstupnim programem */
};

enum error {
	eIMAX,
	eWRONG,
	eEXPR,
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
