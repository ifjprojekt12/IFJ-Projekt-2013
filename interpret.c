/*
IFJ 2013/14

Interpret

*/

#include "interpret.h"

//konstanty etc.
const int str_rezerv = 50;

int interpret(LIST_3AK *list){

  NODE op_1, op_2, result;

  list->actual = list->first;

  while(1){
    //pro jistotu kontrola, ze instrukce neodkazuje na NULL
    if(list->actual == NULL){
      eCode = sINTERN;
      return EXIT_FAILURE;
    }

    //ulozeni ukazatelu, ciste pro zjednoduseni konstrukci -> a .
    op_1 = list->actual->operand_1;
    op_2 = list->actual->operand_2;
    result = list->actual->result;

    //*******************************************
    //instrukce konce programu
    //*******************************************
    if(list->actual->id == iEND){
      break;
    }

    //*******************************************
    //Aritmeticke a retezcove operatory
    //*******************************************
    //scitani
    if(list->actual->id == iPLUS){

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
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //-------------------------------------------
    //odcitani
    if(list->actual->id == iMINUS){

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
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //-------------------------------------------
    //nasobeni
    if(list->actual->id == iMUL){

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
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //-------------------------------------------
    //deleni
    if(list->actual->id == iDIV){

      //kontrola na deleni nulou
      if(op_2->data.type_token == 32){
        if(op_2->data.d_number == 0.0){
          eCode = sSynZero;
          return EXIT_FAILURE;
        }
      }
      if(op_2->data.type_token == 31){
        if(op_2->data.c_number == 0){
          eCode = sSynZero;
          return EXIT_FAILURE;
        }
      }

      //int a int
      if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        result->data.type_token = 32;
        result->data.d_number = (double)op_1->data.c_number / (double)op_2->data.c_number;
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
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //-------------------------------------------
    //konkatenace
    if(list->actual->id == iKONK){

      //string a string
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){

        result->data.type_token = 30;

        //ulozime stary retezec
        char *old_str = result->data.string;
        char *new_str;

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string) + strlen(op_2->data.string)+ str_rezerv)) == NULL){
          eCode = sINTERN;
          return EXIT_FAILURE;
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
          eCode = sINTERN;
          return EXIT_FAILURE;
        }

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string) + str_rezerv)) == NULL){
          eCode = sINTERN;
          return EXIT_FAILURE;
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
          eCode = sINTERN;
          return EXIT_FAILURE;
        }

        //zabereme misto pro spojene retezce s nejakou rezervou
        if((new_str = malloc(sizeof(char) * strlen(op_2->data.string) + str_rezerv)) == NULL){
          eCode = sINTERN;
          return EXIT_FAILURE;
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
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }

    //*******************************************
    //Operator prirazeni
    //*******************************************
    if(list->actual->id == iASSIGN){
      if(op_1->data.type_token == 30){
        char *old_str = result->data.string;
        char *new_str;

        if((new_str = malloc(sizeof(char) * strlen(op_1->data.string))) == NULL){
          eCode = sINTERN;
          return EXIT_FAILURE;
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
    //Porovnavaci operatory
    //*******************************************
    // rovno
    if(list->actual->id == iEQ){
      //odlisne typy
      if(op_1->data.type_token != op_2->data.type_token){
        list->actual = list->actual->jump;
        continue;
      }
      //string
      if(op_1->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) == 0){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //int
      if(op_1->data.type_token == 31){
        if(op_1->data.c_number == op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //double
      if(op_1->data.type_token == 32){
        if(op_1->data.d_number == op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //boolean
      if(op_1->data.type_token == 33){
        if(op_1->data.boolean == op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //null
      if(op_1->data.type_token == 34){
        list->actual = list->actual->right;
        continue;
      }
    }
    //---------------------------------
    // nerovno
    if(list->actual->id == iNEQ){
      //odlisne typy -- pokud vemu zadani doslova, tak stejne datove typy = false
      //a jak by se pak mohlo porovnavat?
      /*
      if(op_1->data.type_token == op_2->data.type_token){
        list->actual = list->actual->jump;
        continue;
      }*/
      //string
      if(op_1->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) != 0){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //int
      if(op_1->data.type_token == 31){
        if(op_1->data.c_number != op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //double
      if(op_1->data.type_token == 32){
        if(op_1->data.d_number != op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //boolean
      if(op_1->data.type_token == 33){
        if(op_1->data.boolean != op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        }
        else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      //null
      if(op_1->data.type_token == 34){
        list->actual = list->actual->jump;
        continue;
      }
    }
    //---------------------------------
    // vetsi
    if(list->actual->id == iHIGH){
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) > 0){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        if(op_1->data.c_number > op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        if(op_1->data.d_number > op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 33 && op_2->data.type_token == 33){
        if(op_1->data.boolean > op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else {
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //---------------------------------
    // vetsi rovno
    if(list->actual->id == iHEQ){
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) >= 0){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        if(op_1->data.c_number >= op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        if(op_1->data.d_number >= op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 33 && op_2->data.type_token == 33){
        if(op_1->data.boolean >= op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else {
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //---------------------------------
    // mensi
    if(list->actual->id == iLOW){
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) < 0){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        if(op_1->data.c_number < op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        if(op_1->data.d_number < op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 33 && op_2->data.type_token == 33){
        if(op_1->data.boolean < op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else {
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }
    //---------------------------------
    // mensi rovno
    if(list->actual->id == iLEQ){
      if(op_1->data.type_token == 30 && op_2->data.type_token == 30){
        if((strcmp(op_1->data.string,op_2->data.string)) <= 0){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 31 && op_2->data.type_token == 31){
        if(op_1->data.c_number <= op_2->data.c_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 32 && op_2->data.type_token == 32){
        if(op_1->data.d_number <= op_2->data.d_number){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else if(op_1->data.type_token == 33 && op_2->data.type_token == 33){
        if(op_1->data.boolean <= op_2->data.boolean){
          list->actual = list->actual->right;
          continue;
        } else {
          list->actual = list->actual->jump;
          continue;
        }
      }
      else {
        eCode = sSynCompatib;
        return EXIT_FAILURE;
      }
    }

    //*******************************************
    //Jednoduchy skok
    //*******************************************
    if(list->actual->id == iJUMP){
      if(list->actual->jump != NULL){
        list->actual = list->actual->jump;
        continue;
      }
      else {
        list->actual = list->actual->right;
        continue;
      }
    }

    //*******************************************
    //Vestavene funkce
    //*******************************************
    //put_string
    if(list->actual->id == iP_STR){
      put_string(&op_1);
      if(list->actual->left->id != iP_STR){
        result->data.c_number = 1;
      }
      else {
        result->data.c_number++;
      }
    }



    //nastavi dalsi instrukci v poradi
    list->actual = list->actual->right;
  }

  return EXIT_SUCCESS;
}

