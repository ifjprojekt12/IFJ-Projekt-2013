#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "semantic.h"

#define OK      0
#define END_B   30
#define END_S   31
#define END_CB  32
#define END_SB  33
#define I_MAX   512

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

void printERR(int err);
int syntaxer();
void initialize_array(TOKEN*array);
int expression(TOKEN*array, int i, TOKEN unit, int ending); // ending - urcuje, jaky znak ocekavame na konci vyrazu
/*
    END_B = 30  (bracket)
    END_S = 31  (semicolon)
    END_CB = 32 (comma, bracket)
    END_SB = 33 (semicolon, bracket)
*/

#endif
