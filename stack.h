#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdbool.h>


typedef struct tElem {
    struct tElem *ptr;	//ukazatel na predchozi prvek
    int Elem;			//hodnota na zasobniku
} *tElemPtr;

typedef struct { 
	tElemPtr Last;
} tStack;


void init(tStack *);	//Inicializace zasobniku
void PUSH(tStack *,int);	// Vlozi na vrchol zasobniku El
void POP(tStack *);		// Uvolni posledni prvek na zasobniku
void TOP(tStack *,int *);		// Vrati prvek z vrcholu zasobniku
int TOPCheck(tStack *);		// vrati prvek z vrcholu zasobniku
void DisposeStack (tStack *);	//Zrusi zasobnik
bool SEmpty(tStack *);		// Je zasobnik prazdny

