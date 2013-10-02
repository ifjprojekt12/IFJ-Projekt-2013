#ifndef _SYNTAX_H
#define _SYNTAX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "lexical.h"

int syntaxer();
void initialize_array(TOKEN*array);
int expression(TOKEN*array, int i);

#endif
