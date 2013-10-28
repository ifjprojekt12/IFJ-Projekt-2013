#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "ial.h"

extern int eCode;

//konverzni funkce

//vraci hodnotu 0 - false, nebo hodnotu 1 - true
int boolval(NODE);

double doubleval(NODE);
int intval(NODE);
char *strval(NODE);

//prace s retezci
unsigned int my_strlen (char *);
char * my_get_substring (char *, int, int);
char * my_get_string ();
void put_string(NODE *);
int find_string();

#endif
