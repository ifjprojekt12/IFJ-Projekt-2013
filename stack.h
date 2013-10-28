#ifndef _STACK_H
#define _STACK_H

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>
#include<string.h>

#include "lexical.h"
#include "ial.h"
#include "data_struct.h"

typedef struct tElem {
    struct tElem *ptr;  //ukazatel na predchozi prvek
    int Elem;           //hodnota na zasobniku
} *tElemPtr;

typedef struct { 
    tElemPtr Last;
} tStack;

typedef struct tNode {
    struct tNode *ptr;  //ukazatel na predchozi prvek
    NODE Node;          //hodnota na zasobniku
} *tNodePtr;

typedef struct { 
    tNodePtr Last;
} tSNode;

typedef struct tInstr {
    struct tInstr *ptr; //ukazatel na predchozi prvek
    INSTRUCT Instr;  //hodnota na zasobniku
} *tInstrPtr;

typedef struct {
    tInstrPtr Last;
} tSInstr;


void initNode(tSNode *);    //Inicializace zasobniku
void PUSHNode(tSNode *,NODE);   // Vlozi na vrchol zasobniku El
void POPNode(tSNode *);     // Uvolni posledni prvek na zasobniku
void TOPNode(tSNode *,NODE *);      // Vrati prvek z vrcholu zasobniku
void TOPPOPNode(tSNode *, NODE *);
void DisposeSNode(tSNode *);    //Zrusi zasobnik
bool SEmptyNode(tSNode *);      // Je zasobnik prazdny


void init(tStack *);    //Inicializace zasobniku
void PUSH(tStack *,int);    // Vlozi na vrchol zasobniku El
void POP(tStack *);     // Uvolni posledni prvek na zasobniku
void TOP(tStack *,int *);       // Vrati prvek z vrcholu zasobniku
int TOPCheck(tStack *);     // vrati prvek z vrcholu zasobniku
void DisposeStack (tStack *);   //Zrusi zasobnik
bool SEmpty(tStack *);      // Je zasobnik prazdny

void initInstr(tSInstr *);
void PUSHInstr(tSInstr *, INSTRUCT);
void POPInstr(tSInstr *, INSTRUCT *);
bool SEmptyInstr(tSInstr *);


#endif
