#include "function.h"

/*
30    string
31    int
32    double
33    bool
34    null - Data Type
*/

//-------------------------------------
//vraci hodnotu konvertovanou na type bool (0/false,1/true)
int boolval(NODE value){

  int result = -1;

  if(value->data.type_token == 34){
    result = 0;
  }

  if(value->data.type_token == 33){
    result = value->data.boolean;
  }

  //double 0 == 0 lze?
  if(value->data.type_token == 32){
    if(value->data.d_number == 0.0){
      result = 0;
    } else result = 1;
  }

  if(value->data.type_token == 31){
    if(value->data.c_number == 0){
      result = 0;
    } else result = 1;
  }

  if(value->data.type_token == 30){
    if(strcmp("",value->data.string) == 0){
      result = 0;
    } else result = 1;
  }

  return result;
}

//-------------------------------------

