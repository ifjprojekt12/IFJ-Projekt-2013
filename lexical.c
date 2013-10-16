/*
IFJ 2013/14

Lexikalni analyzator

*/

#include "lexical.h"

#define buffer_size 100
#define keywords_table_length 18
#define NaN 0.0/0.0

//ZNAME BUGY
//retezce, promenne, identifikatory a podobny shit pri velikosti radku vetsi nez je buffer - dodelat
//dodelat reallocy u identifikatoru, retezcu a promennych
//a dodelat podminky na to, ze malloc prosel a neexnul nam nekde v prubehu (0/100?)
//retezec neukonceny " zacykli lexikalni analyzator...

FILE *source; //prom. - zdrojovy soubor
char buffer[buffer_size]; //buffer
unsigned int pos_buffer = 0; //aktualni pozice v bufferu

int start = 0;

//tabulka klicovych slov - retezce se porovnaji s timto a token se tomu podridi
const char table_words[keywords_table_length][15] = {"if","else","elseif","while","for","function","return",
            "boolval","doubleval","intval","strval","get_string","put_string","strlen",
            "get_substring","find_string","sort_string"};


//***********************************************
//operace nad souborem, vola se z mainu na zacatku a konci
//***********************************************

//otevre zdrojovy soubor
int open_source(char *source_file){

  if((source = fopen(source_file,"r")) == NULL){
    return 1;
  }

  return 0;
}

//zavre zdrojovy soubor
void close_source(void){
  fclose(source);
}

//naplni buffer vstupnimi daty, na konci souboru vraci 1
int read_src(){

  if(fgets(buffer,buffer_size-1,source) == NULL)
    return 1;

  //kontrola prazdneho radku... PREDELAT NA VSECHNY BILE ZNAKY PRAZDNEHO RADKU
  while(1){
    if(buffer[0] == 10) {
      if(fgets(buffer,buffer_size,source) == NULL)
        return 1;
    } else break;
  }

  return 0;
}

//***********************************************
//automat pro cteni a analyzu tokenu...
//***********************************************
void token_init(TOKEN *token){
  token->type_token = 0;
  token->c_number = NaN;
  token->d_number = NaN;
  token->string = NULL;
  token->boolean = -1;
  token->null = -1;
  token->id_name = NULL;
}

//------------------------------------------------
TOKEN get_token(){
  TOKEN new_tok;

  //nastaveni vychazich hodnot noveho tokenu
  token_init(&new_tok);

    /* kontroly pred jadrem zjistovani dalsiho tokenu
  prebytecna bila mista, konce radku a podobne */

  //jednoducha kontrola na zacatek souboru <?php
  if(start == 0){
    char *o_w;

    read_src();

    if((o_w = strstr(buffer,"<?php")) != NULL){
      if(*o_w == buffer[0] && (isspace(buffer[5])) != 0){
        start = 1;
        pos_buffer = 6;
      }
    }

    if(start == 0){
      new_tok.type_token = 0;
      return new_tok;
    }
  }

  //kontrola konce radku (bufferu)
  if(pos_buffer == (strlen(buffer))){
    pos_buffer = 0;
  }

  //tak detekujeme posledni \n na radku.... no neni to prasacke?
  if(buffer[pos_buffer] == '\n'){
    pos_buffer = 0;
  }

  //nacteni noveho radku a detekce konce souboru
  if(pos_buffer == 0){
    if(read_src() == 1){
      new_tok.type_token = 50;
      return new_tok;
    }
  }

  //------------------------------------------------
  //oddelavame komentare - radkove
  while(1){
    if(buffer[pos_buffer] == '/' && buffer[pos_buffer+1] == '/'){
      read_src();
      pos_buffer = 0;
    } else {break;}
  }

  //oddelavame blokove komentare
  if(buffer[pos_buffer] == '/' && buffer[pos_buffer+1] == '*'){
    pos_buffer = pos_buffer + 2;
    while(1){
      if(buffer[pos_buffer] == '\n'){
        if(read_src() == 1){
          new_tok.type_token = 0;
          return new_tok;
        }
        pos_buffer = 0;
      }
      if(buffer[pos_buffer] == '*' && buffer[pos_buffer+1] == '/'){
        pos_buffer = pos_buffer + 2;
        break;
      }
      pos_buffer++;
    }

    //tak detekujeme posledni \n na radku... again
    if(buffer[pos_buffer] == '\n'){
      if(read_src() == 1){
        new_tok.type_token = 50;
        return new_tok;
      }
      pos_buffer = 0;
    }
  }

  //prebytecna bila mista
  while(1){
    if((isspace(buffer[pos_buffer])) != 0 && buffer[pos_buffer] != '\n'){
      pos_buffer++;
      //a toto nacte novy radek, pokud jsou prebytecna bila mista na konci radku
      if(buffer[pos_buffer] == '\n'){
        if(read_src() == 1){
          new_tok.type_token = 50;
          return new_tok;
        }
        pos_buffer = 0;
      }

    } else break;
  }

  //-------------------------------------------------
  //konecny automat...
  //-------------------------------------------------
  //zavorky
  //znak (
  if(buffer[pos_buffer] == '('){
    new_tok.type_token = 40;
    pos_buffer++;
    return new_tok;
  }

  //znak )
  if(buffer[pos_buffer] == ')'){
    new_tok.type_token = 41;
    pos_buffer++;
    return new_tok;
  }

  //znak {
  if(buffer[pos_buffer] == '{'){
    new_tok.type_token = 42;
    pos_buffer++;
    return new_tok;
  }

  //znak }
  if(buffer[pos_buffer] == '}'){
    new_tok.type_token = 43;
    pos_buffer++;
    return new_tok;
  }

  //************************************************
  //aritmeticke operatory, rovna se a podobne
  //+
    if(buffer[pos_buffer] == '+'){
    new_tok.type_token = 14;
    pos_buffer++;
    return new_tok;
  }

  //-
  if(buffer[pos_buffer] == '-'){
    new_tok.type_token = 11;
    pos_buffer++;
    return new_tok;
  }

  //*
  if(buffer[pos_buffer] == '*'){
    new_tok.type_token = 12;
    pos_buffer++;
    return new_tok;
  }

  // /
  if(buffer[pos_buffer] == '/'){
    new_tok.type_token = 13;
    pos_buffer++;
    return new_tok;
  }

  //=== a =
  if(buffer[pos_buffer] == '='){
    pos_buffer++;
    if(buffer[pos_buffer] == '=' && buffer[pos_buffer+1] == '='){
      pos_buffer = pos_buffer+2;
      new_tok.type_token = 16;
      return new_tok;
    } else {
      new_tok.type_token = 10;
      return new_tok;
    }
  }

  // !==
  if(buffer[pos_buffer] == '!' && buffer[pos_buffer+1] == '=' && buffer[pos_buffer+2] == '=' ){
    new_tok.type_token = 17;
    pos_buffer = pos_buffer+3;
    return new_tok;
  }

  //.
  if(buffer[pos_buffer] == '.'){
    new_tok.type_token = 15;
    pos_buffer++;
    return new_tok;
  }

  //>= a >
  if(buffer[pos_buffer] == '>'){
    pos_buffer++;
    if(buffer[pos_buffer] == '='){
      pos_buffer++;
      new_tok.type_token = 19;
      return new_tok;
    } else {
      new_tok.type_token = 18;
      return new_tok;
    }
  }

  //<= a <
  if(buffer[pos_buffer] == '<'){
    pos_buffer++;
    if(buffer[pos_buffer] == '='){
      pos_buffer++;
      new_tok.type_token = 21;
      return new_tok;
    } else {
      new_tok.type_token = 20;
      return new_tok;
    }
  }

  //;
  if(buffer[pos_buffer] == ';'){
    new_tok.type_token = 22;
    pos_buffer++;
    return new_tok;
  }

  //,
  if(buffer[pos_buffer] == ','){
    new_tok.type_token = 23;
    pos_buffer++;
    return new_tok;
  }

  //************************************************
  //cisla --- bug .... problem je akorat v tom, ze sem po par dnech zapomnel jaky, nebo jestli uz je spraveny :))
  if((isdigit(buffer[pos_buffer])) != 0){
    char value[20];
    int value_pos = 0;
    int cel_or_double = 0;
    int exponent = 0;

    while(1){
      if((isdigit(buffer[pos_buffer])) != 0){

      //par podminek zjistujicich, zda cislo bude desetine, nebo zustane jako cele
      } else if(buffer[pos_buffer] == 'E' || buffer[pos_buffer] == 'e'){
          cel_or_double = 1;

          //duplikace exponentu v cisle
          if(exponent == 1){
            return new_tok;
          }

          exponent = 1;

          //neprazdny exponent
          if(!((isdigit(buffer[pos_buffer + 1])) != 0)){
            if(buffer[pos_buffer + 1] != '+' && buffer[pos_buffer +1] != '-')
              return new_tok;
          }

      } else if(buffer[pos_buffer] == '.'){

          //duplikace desetinne carky v cisle
          if(cel_or_double == 1){
            return new_tok;
          }
          cel_or_double = 1;

          //neprazdna desetinna cast
          if((isdigit(buffer[pos_buffer + 1])) != 0){}
          else return new_tok;

      } else if((buffer[pos_buffer] == '+' || buffer[pos_buffer] == '-') && exponent == 1){
          cel_or_double = 1;

          //neprazdny exponent
          if(!((isdigit(buffer[pos_buffer + 1])) != 0)){
            return new_tok;
          }

      } else {
          break;
      }

      value[value_pos] = buffer[pos_buffer];
      value_pos++;
      pos_buffer++;
    }

    //podminka zhruba na tvary 1aa,1.0aa a tak dale, funguje zatim na pismenka
    if((isalpha(buffer[pos_buffer])) != 0){
      new_tok.type_token = 0;
      return new_tok;
    }

    value[value_pos] = 0;

    if(cel_or_double == 0){
      new_tok.c_number = atoi(value);
    }

    if(cel_or_double == 1){
      new_tok.d_number = atof(value);
    }

    new_tok.type_token = 31;

    if(cel_or_double == 1){
      new_tok.type_token = 32;
    }

    return new_tok;
  }

  //************************************************
  //retezce v " "
  if(buffer[pos_buffer] == '"'){
    int ende_string = 0;
    int length_string = 0;

    pos_buffer++;

    //kontrola na prazdny string
    if(buffer[pos_buffer] == '"'){
      pos_buffer++;
      if(new_tok.string == NULL){
        new_tok.string = malloc(sizeof(char)*100);
      }
      new_tok.string[0] = '\0';
      new_tok.type_token = 30;
      return new_tok;
    }

    while(ende_string == 0){
      pos_buffer++;

      if(pos_buffer == strlen(buffer)){
        if((read_src()) == 1){
          return new_tok;
        }
        pos_buffer = 0;
      }

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //ukladani obsahu retezce a prace s pameti kam se uklada
      if(new_tok.string == NULL){
        new_tok.string = malloc(sizeof(char)*100);
      }

      //DOPLNIT REALLOC <<------

      new_tok.string[length_string] = buffer[pos_buffer-1];
      length_string++;
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

      if(buffer[pos_buffer] == '"'){
        ende_string = 1;
        new_tok.type_token = 30;
        pos_buffer++;

        new_tok.string[length_string] = '\0';
        return new_tok;
      }
    }
  }

  //************************************************
  //predpokladam, ze promenne a identifikatory vzdy maji za sebou bily znak... asi?
  if((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_'){
    char id_name[500];
    int uk = 0;
    id_name[uk] = buffer[pos_buffer];
    uk++;

    pos_buffer++;
    while((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_' || (isdigit(buffer[pos_buffer])) != 0){
      id_name[uk] = buffer[pos_buffer];
      pos_buffer++;
      uk++;
    }
    id_name[uk]='\0';

    new_tok.type_token = 35;

    //null...
    if((strcmp(id_name,"null")) == 0){
      new_tok.type_token = 34;
      return new_tok;
    }

    //bool...
    if((strcmp(id_name,"false")) == 0){
      new_tok.type_token = 33;
      new_tok.boolean = 0;
      return new_tok;
    }
    if((strcmp(id_name,"true")) == 0){
      new_tok.type_token = 33;
      new_tok.boolean = 1;
      return new_tok;
    }
    //_________!!!
    for(unsigned int a = 0;a < keywords_table_length;a++){
      if((strcmp(table_words[a],id_name)) == 0){
        if(a < 7){
          new_tok.type_token = a+1;
        } else {
          new_tok.type_token = a+51;
        }
      }

    }

    //pokud to neni klicove slovo, nebo vestavena funkce ulozime to do id_name v tokenu
    if(new_tok.type_token == 35){
      if(new_tok.id_name == NULL){
        new_tok.id_name = malloc(sizeof(char)*strlen(id_name)+10);
      }

      strcpy(new_tok.id_name,id_name);

    }

    return new_tok;
  }

  //************************************************
  //promenna
  if(buffer[pos_buffer] == '$'){
    int length_string = 0;

    pos_buffer++;
    if((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_'){
      while((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_' || (isdigit(buffer[pos_buffer])) != 0){

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //ukladani nazvu promenne a prace s pameti kam se uklada
        if(new_tok.id_name == NULL){
          new_tok.id_name = malloc(sizeof(char)*100);
        }

        //DOPLNIT REALLOC <<------

        new_tok.id_name[length_string] = buffer[pos_buffer];
        length_string++;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        pos_buffer++;
      }

      new_tok.id_name[length_string] = '\0';

      new_tok.type_token = 36;
      return new_tok;
    }
  }

  //************************************************
  //kontrolujem zda mame konec radku po tokenu...
  if(pos_buffer == (strlen(buffer)-1)){
    pos_buffer = 0;
  }

  return new_tok;
}

