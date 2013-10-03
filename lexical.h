#ifndef _LEXICAL_H
#define _LEXICAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//datove typy pro lex.analyzator ...
FILE *source;

//struktura tokenu -> typ tokenu, dale pripadne hodnota cisla, text. retezec atd.
typedef struct {
  int type_token; //typ
  int c_number;   //cele cislo
  double d_number;//d. cislo
  char *string;   //retezec
  int boolean;    //bool hodnota
  int null;       //null --- je to to k necemu?
  char *id_name;
}TOKEN;


//funkce pro nacteni, identifikaci a poslani tokenu zpet zadateli
void token_init(TOKEN *token);
TOKEN get_token(void);

//nacteni dalsiho bloku pro zpracovani
int read_src();

//funkce pro praci nad souborem
int open_source(char *source);
void close_source(void);

#endif
