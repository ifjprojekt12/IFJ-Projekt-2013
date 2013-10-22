/*
IFJ 2013/14

Interpret

*/

#ifndef _INTERPRET_H
#define _INTERPRET_h

#include "data_struct.h"

enum intstructions {
    END,
    PLUS,
    MINUS,
    KONK,
    ASSIGN,
};

int interpret(LIST_3AK *);

#endif
