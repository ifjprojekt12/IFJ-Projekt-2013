/*
IFJ 2013/14

Interpret

*/

#include "interpret.h"

//konstanty etc.
const int str_rezerv = 50;

int interpret(LIST_3AK *list){

  NODE op_1, op_2, result;

  STACK symbol_tables;
  tSInstr next_instr;

  stackInit(&symbol_tables);
  initInstr(&next_instr);

  push(&symbol_tables,&root);

  //zasobnik na zaznamy pro return
  tSNode ret_addr;
  initNode(&ret_addr);

  list->actual = list->first;

  while(1){
    //printf("%d...%d\n",list->actual->id,list->actual->right->id);
    //*********************
    //ladici vypisy
    //if(list->actual->id == iIVAL){printf("intval ");}
    //printf("%d\n",list->actual->id);

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

    if(list->actual->id == iSAVE_PAR && list->actual->result == NULL){
      list->actual = list->actual->right;
      continue;
    }
    //*******************************************
    //konec funkce
    //*******************************************
    if(list->actual->id == iEND_FCE){
      int a;
      INSTRUCT next = NULL;
      POPInstr(&next_instr,&next,&a);
      list->actual = next;
      pop(&symbol_tables);
      continue;
    }
    //*******************************************
    //*******************************************
    op_1 = NULL;op_2 = NULL;result = NULL;
    //vyhledavame v prislusnych tabulkach prislusne zaznamy
    if(list->actual->operand_1 != NULL){
      if(list->actual->operand_1->data.type_token == 35){
        op_1 = list->actual->operand_1;
      } else if(list->actual->id == iRETURN){
          op_1 = searchIdent(list->actual->operand_1->key,symbol_tables->ptr);
      }
      else {
        op_1 = searchIdent(list->actual->operand_1->key,symbol_tables->ptr);
      }
    } else op_1 = NULL;

    if(list->actual->operand_2 != NULL){
      if(list->actual->id != iRETURN){
      //if(list->actual->id != iRETURN && list->actual->id != iP_STR_NEW && list->actual->id != iP_STR && list->actual->id != iDVAL){
        op_2 = searchIdent(list->actual->operand_2->key,symbol_tables->ptr);
      }
    } else op_2 = NULL;

    if(list->actual->result != NULL){
      if(list->actual->id != iRETURN){
        result = searchIdent(list->actual->result->key,symbol_tables->ptr);
      }
    } else result = NULL;

    //*******************************************
    //Instrukce pro implementaci funkci
    //*******************************************
    //pomocna instrukce znacici, ze prijde volani funkce
    if(list->actual->id == iFUNCTION){
      //NODE *new_tab = malloc(struct NODE);
      NODE *new_tab = malloc(sizeof(struct NODE));
     //printf("NT:%p\n",new_tab);
      treeInit(new_tab);
      //printf("prvni");
      //fflush(stdout);
      //NODE old = op_1->params;
      *new_tab = copyTree(&op_1->params);
      //printf("FP:%p\n",&op_1->params);
      //new_tab = op_1->params;
      //printf("Vkladam tabulku\n");
      //printf("NT:%p\n",new_tab);
      push(&symbol_tables,new_tab);

     // printf("druha");
     // fflush(stdout);


      list->actual = list->actual->right;
      //printf("a sem zde");
      continue;
    }

    //volani funkce
    if(list->actual->id == iASSIGN && list->actual->operand_1->data.type_token == 35){
      //defaultni navratovy typ
      if(list->actual->result != NULL){
        result = searchIdent(list->actual->result->key,symbol_tables->next->ptr);
        result->data.type_token = 34;
      }
      //ulozeni instrukce nasledujici po funkci
      PUSHInstr(&next_instr,list->actual->right,0);

      //ulozeni kam ukladat navratovou hodnotu
      PUSHNode(&ret_addr,result);

      //jaka dalsi instrukce se bude vykonavat
      list->actual = op_1->body->first;
      continue;
    }

    //*******************************************
    //return a ukladani parametru funkci
    //*******************************************
    //return
    if(list->actual->id == iRETURN){
      TOPPOPNode(&ret_addr,&result);

      result->data.type_token = op_1->data.type_token;
      if(result->data.type_token == 30){
        if((result->data.string = malloc(strlen(op_1->data.string))) == NULL){
          eCode = 99;
          return EXIT_FAILURE;
        }
        strcpy(result->data.string,op_1->data.string);
      }
      if(result->data.type_token == 31){
        result->data.c_number = op_1->data.c_number;
      }
      if(result->data.type_token == 32){
        result->data.d_number = op_1->data.d_number;
      }
      if(result->data.type_token == 33){
        result->data.boolean = op_1->data.boolean;
      }

      list->actual = list->actual->right;
      continue;
    }

    //ukladame parametry funkci
    if(list->actual->id == iSAVE_PAR){
      if(list->actual->operand_1 != NULL){
        op_1 = searchIdent(list->actual->operand_1->key,symbol_tables->next->ptr);
      }

      result->data.type_token = op_1->data.type_token;
      if(result->data.type_token == 30){
        if((result->data.string = malloc(strlen(op_1->data.string))) == NULL){
          eCode = 99;
          return EXIT_FAILURE;
        }
        strcpy(result->data.string,op_1->data.string);

      }
      if(result->data.type_token == 31){
        result->data.c_number = op_1->data.c_number;
      }
      if(result->data.type_token == 32){
        result->data.d_number = op_1->data.d_number;
      }
      if(result->data.type_token == 33){
        result->data.boolean = op_1->data.boolean;
      }

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
        if((new_str = calloc((strlen(op_1->data.string) + strlen(op_2->data.string)+ str_rezerv),sizeof(char))) == NULL){
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
      if(op_1->data.type_token == 34){
        result->data.type_token = 34;
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
      result->data.string = strval(op_1);
      result->data.type_token = 30;
    }
    //intval
    if(list->actual->id == iIVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.c_number = intval(op_1);
      result->data.type_token = 31;
    }
    //doubleval
    if(list->actual->id == iDVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.d_number = doubleval(op_1);
      if(eCode == 13){
        return EXIT_FAILURE;
      }
      result->data.type_token = 32;
    }
    //boolval
    if(list->actual->id == iBVAL){
      if(op_1->data.type_token < 30 || op_1->data.type_token > 34){
        eCode = sSemVar;
        return EXIT_FAILURE;
      }
      result->data.boolean = boolval(op_1);
      result->data.type_token = 33;
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
        NODE result_gsubstr = NULL;
        result_gsubstr = searchIdent(list->actual->right->result->key,symbol_tables->ptr);
        if((result_gsubstr->data.string = my_get_substring(op_1->data.string,intval(op_2),intval(result))) == NULL){
          return EXIT_FAILURE;
        }
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
      result->data.type_token = 31;
      result->data.c_number = find_string(strval(op_1),strval(op_2));
    }
    //sort_string
    if(list->actual->id == iS_STR){
      result->data.type_token = 30;
      result->data.string = my_sort_string(strval(op_1));
    }


    //*******************************************
    //nastavi dalsi instrukci v poradi
    list->actual = list->actual->right;
  }

  return EXIT_SUCCESS;
}

