/**

Implementace interpretu imperativniho jazyka IFJ13.

xhanak27 - Jan Hanak
xfiser10 - Jakub Fiser

*/

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
#define SEMICOLON   22      // type_token ;
#define B_BRACKET   41      // type_token )
#define BRACKET     42      // type_token {
#define ASCII       48      // prevod int -> char

extern NODE check_func;
extern NODE root;
extern NODE tree;
extern NODE func;
extern LIST_3AK list;
extern INSTRUCT aux;
extern tSInstr InstrStack;
extern tSInstr InstrFor;
extern tSInstr InstrBreak;
extern int eCode;
extern bool func_end;

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
int expression_sem(TOKEN *array, int *m, int end, bool is_for);

// funkce zpracovava vyrazy v postfixu a vytvari instrukce pro interpret
int read_postfix(TOKEN *array, int type, int max);

// funkce pro vytvoreni jmena do tabulky symbolu
char* makeName(TOKEN unit);

// funkce pro smazani instrukce ze seznamu a ulozeni do zasobniku pro cyklus FOR
void PUSH_last(bool first, tSInstr *Stack);

// funkce pro porovnavani datovych typu
bool Compatible(NODE *a1, NODE *a2, int sign);

// funkce pro prevod typu tokenu na index v precedencni tabulce
int Give_index(int type);

// funkce pro inicializaci pole tokenu, i = index, m = velikost pole
int initialize_array(TOKEN**array, int i, int m);

// funkce pro zvetseni velikosti pole
int realloc_array(TOKEN*array, int*m);

#endif
