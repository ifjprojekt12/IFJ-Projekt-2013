#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"
#include "stack.h"

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
