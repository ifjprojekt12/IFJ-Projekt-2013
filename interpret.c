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
#include "syntax.h"

//konstanty etc.
const int str_rezerv = 50;


int interpret(LIST_3AK *list){

  NODE op_1, op_2, result;

  list->actual = list->first;

  while(1){

    //ulozeni ukazatelu, ciste pro zjednoduseni konstrukci -> a .
    op_1 = list->actual->operand_1;
    op_2 = list->actual->operand_2;
    result = list->actual->result;

    //*******************************************
    //instrukce konce programu
    //*******************************************
    if(list->actual->id == END){
      break;
    }

    //*******************************************
    //Aritmeticke a retezcove operatory
    //*******************************************
    //scitani
    if(list->actual->id == PLUS){

      //int a int
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number + op_2->data.c_number;
      }

      //double a double
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number + op_2->data.d_number;
      }

      //int a double
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = (double)op_1->data.c_number + op_2->data.d_number;
      }

      //double a int
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 31){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number + (double)op_2->data.c_number;
      }

      //cokoliv jineho - chyba
      else {
        return sSynCompatib;
      }
    }
    //-------------------------------------------
    //odcitani
    if(list->actual->id == MINUS){

      //int a int
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number - op_2->data.c_number;
      }

      //double a double
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number - op_2->data.d_number;
      }

      //int a double
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = (double)op_1->data.c_number - op_2->data.d_number;
      }

      //double a int
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 31){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number - (double)op_2->data.c_number;
      }

      //cokoliv jineho - chyba
      else {
        return sSynCompatib;
      }
    }
    //-------------------------------------------
    //nasobeni
    if(list->actual->id == 3){

      //int a int
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number * op_2->data.c_number;
      }

      //double a double
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number * op_2->data.d_number;
      }

      //int a double
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = (double)op_1->data.c_number * op_2->data.d_number;
      }

      //double a int
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 31){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number * (double)op_2->data.c_number;
      }

      //cokoliv jineho - chyba
      else {
        return sSynCompatib;
      }
    }
    //-------------------------------------------
    //deleni
    if(list->actual->id == 4){

      //kontrola na deleni nulou
      if(op_2->data.type_token == 32){
        if(op_2->data.d_number == 0.0){
          return sSynZero;
        }
      }
      if(op_2->data.type_token == 31){
        if(op_2->data.c_number == 0){
          return sSynZero;
        }
      }

      //int a int
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number / op_2->data.c_number;
      }

      //double a double
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number / op_2->data.d_number;
      }

      //int a double
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = (double)op_1->data.c_number / op_2->data.d_number;
      }

      //double a int
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 31){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number / (double)op_2->data.c_number;
      }

      //cokoliv jineho - chyba
      else {
        return sSynCompatib;
      }
    }
    //-------------------------------------------
    //konkatenace
    if(list->actual->id == KONK){

      //string a string
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){

        result->data.type_token = 30;

        //ulozime stary retezec
        char *old_str = result->data.string;
        char *new_str;

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string) + strlen(op_2->data.string)+ str_rezerv)) == NULL){
          return sINTERN;
        }

        //a provedeme samotnou konkatenaci
        strcpy(new_str,op_1->data.string);
        strcat(new_str,op_2->data.string);
        result->data.string = new_str;

        //a uvolnime pripadny puvodni retezec z pameti
        if(old_str != NULL){
          free(old_str);
        }
      }

      //string a neco
      else if(op_1->data.type_token == 30){

        result->data.type_token = 30;

        //ulozime stary retezec
        char *old_str = result->data.string;
        char *new_str;
        char *buffer;

        if((buffer = malloc(sizeof(char) * str_rezerv)) == NULL){
          return sINTERN;
        }

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string) + str_rezerv)) == NULL){
          return sINTERN;
        }

        //samotna konkatenace
        //int
        if(op_2->data.type_token == 31){
          strcpy(new_str,op_1->data.string);
          sprintf(buffer,"%d",op_2->data.c_number);
          strcat(new_str,buffer);
          result->data.string = new_str;
        }
        //double
        if(op_2->data.type_token == 32){
          strcpy(new_str,op_1->data.string);
          sprintf(buffer,"%g",op_2->data.d_number);
          strcat(new_str,buffer);
          result->data.string = new_str;
        }
        //bool
        if(op_2->data.type_token == 33){
          strcpy(new_str,op_1->data.string);
          if(op_2->data.boolean == 0){
            strcat(new_str,"");
          }
          if(op_2->data.boolean == 1){
            strcat(new_str,"1");
          }
          result->data.string = new_str;
        }
        //null
        if(op_2->data.type_token == 34){
          strcpy(new_str,op_1->data.string);
          result->data.string = new_str;
        }

        //a uvolnime pripadny puvodni retezec z pameti
        if(old_str != NULL){
          free(old_str);
        }
        if(buffer != NULL){
          free(buffer);
        }

      }

      //neco a string
      else if(op_2->data.type_token == 30){

        result->data.type_token = 30;

        //ulozime stary retezec
        char *old_str = result->data.string;
        char *new_str;
        char *buffer;

        if((buffer = malloc(sizeof(char) * str_rezerv)) == NULL){
          return sINTERN;
        }

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_2->data.string) + str_rezerv)) == NULL){
          return sINTERN;
        }

        //samotna konkatenace
        //int
        if(op_1->data.type_token == 31){
          sprintf(buffer,"%d",op_1->data.c_number);
          strcpy(new_str,buffer);
          strcat(new_str,op_2->data.string);
          result->data.string = new_str;
        }
        //double
        if(op_1->data.type_token == 32){
          sprintf(buffer,"%g",op_1->data.d_number);
          strcpy(new_str,buffer);
          strcat(new_str,op_2->data.string);
          result->data.string = new_str;
        }
        //bool
        if(op_1->data.type_token == 33){
          if(op_1->data.boolean == 0){
            strcpy(new_str,"");
          }
          if(op_1->data.boolean == 1){
            strcpy(new_str,"1");
          }
          strcat(new_str,op_2->data.string);
          result->data.string = new_str;
        }
        //null
        if(op_1->data.type_token == 34){
          strcpy(new_str,op_2->data.string);
          result->data.string = new_str;
        }

        //a uvolnime pripadny puvodni retezec z pameti
        if(old_str != NULL){
          free(old_str);
        }
        if(buffer != NULL){
          free(buffer);
        }

      }

      //cokoliv jineho - chyba
      else {
        return sSynCompatib;
      }
    }

    //*******************************************
    //Operator prirazeni
    //*******************************************
    if(list->actual->id == ASSIGN){
      if(op_1->data.type_token == 30){
        char *old_str = result->data.string;
        char *new_str;

        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string))) == NULL){
          return sINTERN;
        }

        result->data.type_token = 30;

        strcpy(new_str,op_1->data.string);
        result->data.string = new_str;

        if(old_str != NULL){
          free(old_str);
        }

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

  return 0;
}

