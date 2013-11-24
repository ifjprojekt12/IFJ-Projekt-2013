/*
IFJ 2013/14

Pomocne datove struktury

*/

#include "data_struct.h"

// tabulka pro vypis chyboveho hlaseni
const char*MESSAGE[]=
{
    "Interni chyba programu",
    "Dosazena maximalni velikost pole tokenu",
    "Chybna syntaxe struktury programu",
    "Nespravne zapsany vyraz",
    "Chybi oteviraci zavorka",
    "Chybi slozena oteviraci zavorka",
    "Nespravne zapsany tvar prirazeni",
    "Spatna posloupnost parametru v definici funkce",
    "Neznamy token",
    "Prikaz break nebo continue mimo telo prikazu for",
    "Nedeklarovana promenna",
    "Chybne datove typy ve vyrazu",
    "Chybejici parametr pro volani funkce",
    "Pokus o redefinici funkce",
    "Volana funkce neni definovana",
};

//*************************************
//inicializuje seznam instrukci
void new_instr_list(LIST_3AK *new_list){

  new_list->first = NULL;
  new_list->actual = NULL;
  new_list->last = NULL;

}

//pridava novou instrukci na konec seznamu
int new_instr(LIST_3AK *list, int id, NODE *op_1, NODE *op_2, NODE *result, INSTRUCT jump){
  INSTRUCT new_instr;

  //urcime pamet pro novou instrukci
  new_instr = malloc(sizeof(struct instruct));

  // inicializace
  new_instr->operand_1 = NULL;
  new_instr->operand_2 = NULL;
  new_instr->result = NULL;
  new_instr->jump = NULL;
  new_instr->left = NULL;
  new_instr->right = NULL;

  if(new_instr == NULL)
    return 99;

  //vlozime data
  if(list->first == NULL){
    new_instr->number = 0;
  } else {
    new_instr->number = list->last->number + 1;
  }

  new_instr->id = id;
  if(op_1 != NULL){
    new_instr->operand_1 = *op_1;
  }
  if(op_2 != NULL){
    new_instr->operand_2 = *op_2;
  }
  if(result != NULL){
    new_instr->result = *result;
  }
  new_instr->jump = jump;

  new_instr->left = list->last;

  if(list->last != NULL){
    list->last->right = new_instr;
  } else {
    list->first = new_instr;
  }

  list->last = new_instr;

  return 0;
}

//zrusi seznam instrukci a uvolni pamet
void free_instr_list(LIST_3AK *list){
  INSTRUCT del;

  while(1){
    if(list->first != NULL){
      del = list->first;
      list->first = list->first->right;
      list->first->left = NULL;
      free(del);
    }

    if(list->first->right == NULL)
      break;
  }
}

// vypis chyboveho hlaseni
void printERR(int err)
{
    fprintf(stderr, "Chyba: %s.\n", MESSAGE[err]);
}

//zasobnik instrukci
/*void stack_3ak_init(STACK_3AK *stack){
  stack->top = NULL;
  stack->prev = NULL;
}

void push_3ak(STACK_3AK *stack, INSTRUCT instr){
  stack->top = instr;
  stack->prev = stack;
}

INSTRUCT pop_3ak(STACK_3AK *stack){
  INSTRUCT ret = stack->top;
  stack->top = stack->prev->top;
  stack = stack->prev;
  return ret;
}*/

//*************************************
