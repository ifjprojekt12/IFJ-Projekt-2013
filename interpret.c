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
    //*******************************************
    //instrukce konce programu
    //*******************************************
    if(list->actual->id == iEND){
      break;
    }

    //projistotu kontrola, ze instrukce neodkazuje na NULL
    if(list->actual == NULL){
      eCode = sINTERN;
      return EXIT_FAILURE;
    }

    //a pripadne preskakuje ty ciste pomocne instrukce
    if(list->actual->id == iP_STR_NEW){
      list->actual = list->actual->right;
    }

    //ulozeni ukazatelu, ciste pro zjednoduseni konstrukci -> a .
    //po rozmysleni i pro funkci funkci a podobneho shitu
    if(list->actual->operand_1 != NULL){
      //printf("OP_1: %d_%d_%s\n",list->actual->operand_1->data.type_token,list->actual->id,list->actual->operand_1->data.id_name);
      //printf("RES:  %d_%d_%s\n",list->actual->result->data.type_token,list->actual->id,list->actual->result->data.id_name);
      if(list->actual->operand_1->data.type_token == 35){
        op_1 = searchIdent(list->actual->operand_1->key,&tree);
      } else {
        op_1 = searchIdent(list->actual->operand_1->key,&root);
      }
    } else op_1 = NULL;

    if(list->actual->operand_2 != NULL){
      op_2 = searchIdent(list->actual->operand_2->key,&root);
    } else op_2 = NULL;

    if(list->actual->result != NULL){
      result = searchIdent(list->actual->result->key,&root);
    } else result = NULL;

    //*******************************************
    //Instrukce pro funkce
    //*******************************************
    if(list->actual->id == iASSIGN && list->actual->operand_1->data.type_token == 35){
      //tady bude jina instrukce nez nasledujici
      list->actual = list->actual->right;
      continue;
    }

    //*******************************************
    //Aritmeticke a retezcove operatory
    //*******************************************
    //scitani
    if(list->actual->id == iPLUS){
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      /*else if(op_2->data.type_token == 30){

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

      }*/

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

      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
        result->data.type_token = 31;
        result->data.c_number = op_1->data.c_number;
      }
      if(op_1->data.type_token == 32){
        result->data.type_token = 32;
        result->data.d_number = op_1->data.d_number;
      }
      if(op_1->data.type_token == 33){
        result->data.type_token = 33;
        result->data.boolean = op_1->data.boolean;
      }
    }

    //*******************************************
    //Porovnavaci operatory
    //*******************************************
    // rovno
    if(list->actual->id == iEQ){
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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

      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
      //kontrola zda pouzivame deklarovane promenne
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }

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
    //-------------------------------------------
    //konverzni funkce
    //strval
    if(list->actual->id == iSVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.type_token = 30;
      result->data.string = strval(op_1);
    }
    //intval
    if(list->actual->id == iIVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.type_token = 31;
      result->data.c_number = intval(op_1);
    }
    //doubleval
    if(list->actual->id == iDVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.type_token = 32;
      if((result->data.d_number = doubleval(op_1)) == EXIT_FAILURE){
        return EXIT_FAILURE;
      }
    }
    //boolval
    if(list->actual->id == iBVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.type_token = 33;
      result->data.boolean = boolval(op_1);
    }
    //-------------------------------------------
    //funkce pro praci s retezci
    //put_string
    if(list->actual->id == iP_STR){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      put_string(&op_1);
      if(list->actual->left != NULL && list->actual->left->id != iP_STR){
        result->data.type_token = 31;
        result->data.c_number = 1;
      }
      else {
        result->data.c_number++;
      }
    }
    //get_string
    if(list->actual->id == iG_STR){
      result->data.type_token = 30;
      result->data.string = my_get_string();
    }
    //get_substring
    if(list->actual->id == iG_SUBSTR){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(op_2->data.type_token < 30 || op_2->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(result->data.type_token < 30 || result->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      if(list->actual->right->id == iG_SUBSTR2){
        list->actual->right->result->data.type_token = 30;
        if((list->actual->right->result->data.string = my_get_substring(op_1->data.string,intval(op_2),intval(result))) == NULL){
          return EXIT_FAILURE;
        };
        //op_2->data.c_number,result->data.c_number))
      }
    }
    if(list->actual->id == iG_SUBSTR2){
      list->actual = list->actual->right;
      continue;
    }
    //strlen
    if(list->actual->id == iSTRLEN){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.type_token = 31;
      result->data.c_number = (int) my_strlen(strval(op_1));
    }
    //find_string
    if(list->actual->id == iF_STR){
    }
    //sort_string
    if(list->actual->id == iS_STR){
    }


    //*******************************************
    //nastavi dalsi instrukci v poradi
    list->actual = list->actual->right;
  }

  return EXIT_SUCCESS;
}

