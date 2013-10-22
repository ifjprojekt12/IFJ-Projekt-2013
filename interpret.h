/*
IFJ 2013/14

Interpret

*/

#ifndef _INTERPRET_H
#define _INTERPRET_H

#include "data_struct.h"
#include "syntax.h"

enum intstructions {
    END,
    PLUS,
    MINUS,
    KONK,
    ASSIGN,
};

int interpret(LIST_3AK *);

#endif
