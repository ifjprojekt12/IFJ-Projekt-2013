/*
IFJ 2013/14

Interpret

*/
/*
end    (0, null, null, null, null)     END    nutna instrukce, znaci konec programu
+     (1, id1, id2, id3, null)    PLUS
-    (2, id1, id2, id3, null)    MINUS
*    (3, id1, id2, id3, null)    MUL
/    (4, id1, id2, id3, null)    DIV
.    (5, id1, id2, id3, null)    KONK
=     (6, id1, null, id3, null)    ASSIGN
30    string
31    int
32    double
33    bool
34    null - Data Type
*/

#include "interpret.h"

int interpret(LIST_3AK *list){

  NODE op_1, op_2, result;

  list->actual = list->first;

  while(1){
    op_1 = list->actual->operand_1;
    op_2 = list->actual->operand_2;
    result = list->actual->result;


    //instrukce konce programu
    if(list->actual->id == 0){
      break;
    }

    //*******************************************
    //Aritmeticke a retezcove operatory
    //+
    if(list->actual->id == 1){
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number + op_2->data.c_number;
      }
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number + op_2->data.d_number;
      }
    }
    //-
    if(list->actual->id == 2){
    }
    //*
    if(list->actual->id == 3){
    }
    // /
    if(list->actual->id == 4){
    }
    //.
    if(list->actual->id == 5){
    }
    //*******************************************
    //Operator prirazeni
    if(list->actual->id == 6){
      //dodelat string
      if(op_1->data.type_token == 30){

      }
      if(op_1->data.type_token == 31){
        result->data.c_number = op_1->data.c_number;
      }
      if(op_1->data.type_token == 32){
        result->data.d_number = op_1->data.d_number;
      }
      if(op_1->data.type_token == 33){
        result->data.boolean = op_1->data.boolean;
      }
    }
    //*******************************************

    //nastavi dalsi instrukci v poradi
    list->actual = list->actual->right;
  }

  //zde bude while(1) a obecne jadro interpretu
  return 0;
}

