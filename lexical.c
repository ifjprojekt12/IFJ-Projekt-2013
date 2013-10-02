/*implementace lexikalniho analyzatoru... */

#include "lexical.h"

#define buffer_size 100
#define keywords_table_length 18
#define NaN 0.0/0.0

//ZNAME BUGY
//retezce, promenne, identifikatory a podobny shit pri velikosti radku vetsi nez je buffer
//stejne veci jako predchozi, akorat od zacatku radku vzdy doplni dalsim nulovym tokenem do paru...
//bile mista na konci radku... (krome \n)

FILE *source; // - ukazatel na soubor
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

  token_init(&new_tok);

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
  //blokove... DODELAT!!!

  //a prebytecna bila mista... DODELAT!!!
  while(1){
    if(buffer[pos_buffer] == ' ' || buffer[pos_buffer] == '\t'){
      pos_buffer++;
    } else break;
  }
  //-------------------------------------------------
  //konecny automat...
  //-------------------------------------------------
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
  //+ 10-23
    if(buffer[pos_buffer] == '+'){
    new_tok.type_token = 10;
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
      new_tok.type_token = 14;
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
  //cisla - DODELAT VYCISLENI HODNOTY DANEHO CISLA A KONTROLU TVARU CISLA
  if((isdigit(buffer[pos_buffer])) != 0){
    int cel_or_double = 0;
    pos_buffer++;
    while(1){
      if((isdigit(buffer[pos_buffer])) != 0){

      } else if(buffer[pos_buffer] == 'E' || buffer[pos_buffer] == 'e'){
          cel_or_double = 1;
      } else if(buffer[pos_buffer] == '.'){
          cel_or_double = 1;
      } else if(buffer[pos_buffer] == '+' || buffer[pos_buffer] == '-'){
          cel_or_double = 1;
      } else {
        break;
      }
      pos_buffer++;
    }
    new_tok.type_token = 31;

    if(cel_or_double == 1){
      new_tok.type_token = 32;
    }
    return new_tok;
  }
  //string - DODELAT ZJISTENI OBSAHU RETEZCE!!!
  if(buffer[pos_buffer] == '"'){
    int ende_string = 0;

    pos_buffer++;

    while(ende_string == 0){
      pos_buffer++;
      if(pos_buffer == strlen(buffer)){
        if((read_src()) == 1){
          return new_tok;
        }
        pos_buffer = 0;
      }
      if(buffer[pos_buffer] == '"'){
        ende_string = 1;
        new_tok.type_token = 30;
        pos_buffer++;
        return new_tok;
      }
    }
  }
  //************************************************
  //predpokladam, ze promenne a identifikatory vzdy maji za sebou bily znak...
  //identifikator - DODELAT ZJISTENI A ULOZENI NAZVU IDENTIFIKATORU!!!
  if((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_'){
    char id_name[50];
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
    if((strcmp(id_name,"null"))){
      new_tok.type_token = 34;
      return new_tok;
    }
    //bool...
    if((strcmp(id_name,"false"))){
      new_tok.type_token = 33;
      new_tok.boolean = 0;
      return new_tok;
    }
    if((strcmp(id_name,"true"))){
      new_tok.type_token = 33;
      new_tok.boolean = 1;
      return new_tok;
    }

    for(unsigned int a = 0;a < keywords_table_length;a++){
      if((strcmp(table_words[a],id_name)) == 0){
        if(a < 7){
          new_tok.type_token = a+1;
        } else {
          new_tok.type_token = a+51;
        }
      }

    }
    //printf("%s ",id_name);
    return new_tok;
  }

  //promenna - DODELAT ZJISTENI A ULOZENI NAZVU PROMENNE!!!
  if(buffer[pos_buffer] == '$'){
    pos_buffer++;
    if((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_'){
      while((isalpha(buffer[pos_buffer])) != 0 || buffer[pos_buffer] == '_' || (isdigit(buffer[pos_buffer])) != 0){
        pos_buffer++;
      }
      new_tok.type_token = 36;
      return new_tok;
    }
  }

  //kontrolujem zda mame konec radku po tokenu...
  if(pos_buffer == (strlen(buffer)-1)){
    pos_buffer = 0;
  }

  return new_tok;
}

