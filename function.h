#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "ial.h"

//konverzni funkce

//vraci hodnotu 0 - false, nebo hodnotu 1 - true
int boolval(NODE);

double doubleval(NODE);
int intval();
char *strval();

//prace s retezci
char *get_string();
void put_string();
int our_strlen(); //proc our_? protoze to koliduje s knihovni funkci v C
char *get_substring();
int find_string();
void sort_string();

#endif
