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

// vyctovy typ pro id instrukci
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
    iG_SUBSTR,
    iF_STR,
    iS_STR,
    iFUNCTION,
    iRETURN,
    iJUMP,
    iSAVE_PAR,
    iEND_FCE,
    iP_STR_NEW = 250,
    iG_SUBSTR2 = 270,
};

// vyctovy typ pro vypis chyboveho hlaseni
enum error {
    eINTERN,        // interni chyba, napr malloc
    eIMAX,          // prekrocena max velikost pole
    eWRONG,         // chyba v syntaxi
    eEXPR,          // syntakticka chyba ve vyrazu
    eBRACKETO,      // chybi znak '('
    eSBRACKETO,     // chybi znak '{'
    eASSIGN,        // chybi znak '=' pri prirazovani
    ePARAM,         // chyba v posloupnosti parametru funkce
    eUNKNOWN,       // neznamy token
    eFOR,           // break nebo continue mimo prikaz FOR
    eVAR,           // nedefinovana promenna
    eCOMPATIBLE,    // nekompatibilni datove typy ve vyrazu
    eFCEPARAM,      // chybejici parametr pro volani funkce
    eFCEREDEF,      // pokus o redefinici funkce
    eFCENEDEF,      // nedefinovana funkce
};

// vyctovy typ pro navratovy kod programu
enum finalCode {
    sOK,        /* vse v poradku */
    sLex,       /* chyba pri lexikalni analyze */
    sSyn,       /* chyba syntakticke struktury programy */
    sSemFceDef, /* nedefinovana funkce nebo redefinovana */
    sSemFceParam,   /* chybejici parametr pro volani funkce */
    sSemVar,    /*  Nedeklarovana promenna */
    sSynZero = 10,  /* Chyba deleni nulou */
    sSynTypeConv,   /* pretypovani promenne na cislo funkce doubleval */
    sSynCompatib,   /* chyba typove kompatibility v aritmetickych a relacnich vyrazech */
    sSynRest,   /* ostatni chyby semantiky */
    sINTERN = 99,   /* interni chyba interpretu tj. neovlivnena vstupnim programem */
};

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

// vypis chyboveho hlaseni
void printERR(int err);
//*************************************

#endif
