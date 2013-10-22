/*
IFJ 2013/14

Pomocne datove struktury

*/

#ifndef _DATA_STRUCT_H
#define _DATA_STRUCT_H

/*
IFJ 2013/14

Pomocne datova struktury

*/

#include "ial.h"

//*************************************
//struktury a funkce pro intepreret a 3AK

enum intstructions {
    iEND,
    iPLUS,
    iMINUS,
    iMUL,
    iDIV,
    iKONK,
    iASSIGN,
    iEQ = 10,
    iNEQ,
    iHIGH,
    iHEQ,
    iLOW,
    iLEQ,
    iBVAL = 20,
    iDVAL,
    iIVAL,
    iSVAL,
    iG_STR,
    iP_STR,
    iSTRLEN,
    iG_SUSTR,
    iF_STR,
    iS_STR,
    iFUNCTION,
    iRETURN,
    iJUMP,
    iG_SUBSTR2 = 270,
};

//jedna instrukce
typedef struct instruct{
  int id;          //cislo dane 3AK instrukce
  struct NODE *operand_1; //vstup 1
  struct NODE *operand_2; //vstup 2
  struct NODE *result;    //vystup

  struct instruct *left; //predchozi prvek
  struct instruct *right; //nasledujici prvek

  struct instruct *jump; //adresa instrukce, kam se bude presunovat v pripade skoku na "pasce"

  int number; //a pro eleganci cislo instrukce, unikatni :)
} *INSTRUCT;

//seznam instrukci
typedef struct list_3ak{
  INSTRUCT first;
  INSTRUCT last;
  INSTRUCT actual;
} LIST_3AK;

//vytvoreni noveho seznamu instrukci
void new_instr_list(LIST_3AK *);

//vytvori instrukci a prida ji na konec seznamu
int new_instr(LIST_3AK *, int id, NODE *, NODE *, NODE *, INSTRUCT);

//smazani a uvolneni pameti po seznamu instrukci
void destroy_instr_list(LIST_3AK *list);

//korektne zrusime seznam
void free_instr_list(LIST_3AK *);

//*************************************

#endif
