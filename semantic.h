#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "stack.h"

#define MAXINDEX    14
#define N_MAX       512
#define SEMICOLON   22      // type_token ;
#define BRACKET     42      // type_token {

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
};

// hlavni funkce semantiky
int semantixer(TOKEN *array);

// funkce pro zpracovani vyrazu - prevod do postfixove notace a odeslani instrukci interpretu
int expression_sem(TOKEN *array, int n, int end);

// funkce pro prevod typu tokenu na index v precedencni tabulce
int Give_index(int type);
