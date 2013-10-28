#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lexical.h"
#include "stack.h"
#include "ial.h"
#include "data_struct.h"

#define MAXINDEX    15
#define N_MAX       512
#define SEMICOLON   22      // type_token ;
#define BRACKET     42      // type_token {
#define ASCII       48      // prevod int -> char

extern NODE root;
extern NODE check_func;
extern LIST_3AK list;
extern int eCode;

// enum pro urceni priorit
enum table {
    L,  // lower priority
    H,  // higher or equal priority
    E,  // error
};

// enum pro indexy precedencni tabulky
enum tIndex {
    TERM,       // term
    ADD,        // +
    SUB,        // -
    MUL,        // *
    DIV,        // /
    LOW,        // <
    HIGH,       // >
    EQUAL,      // ===
    ELOW,       // <=
    EHIGH,      // >=
    ENOT,       // !==
    KON,        // .
    LBRACKET,   // (
    RBRACKET,   // )
    ASSIGN,     // =
};

// hlavni funkce semantiky
int semantixer(TOKEN *array);

// funkce pro vytvoreni instrukci pro volani funkce
int functions(TOKEN *array, int n);

// funkce pro zpracovani vyrazu - prevod do postfixove notace a odeslani instrukci interpretu
int expression_sem(TOKEN *array, int n, int end);

// funkce zpracovava vyrazy v postfixu a vytvari instrukce pro interpret
int read_postfix(TOKEN *array);

// funkce pro vytvoreni jmena do tabulky symbolu
char* makeName(TOKEN unit);

// funkce pro porovnavani datovych typu
bool Compatible(NODE *a1, NODE *a2, int sign);

// funkce pro prevod typu tokenu na index v precedencni tabulce
int Give_index(int type);

#endif
