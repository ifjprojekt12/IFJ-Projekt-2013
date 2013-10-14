/*
IFJ 2013/14

Pomocne datove struktury

*/

#include "data_struct.h"

//*************************************
//inicializuje seznam instrukci
void new_instr_list(LIST_3AK *new_list){

  new_list->first = NULL;
  new_list->actual = NULL;
  new_list->last = NULL;

}

//pridava novou instrukci na konec seznamu
int new_instuct(LIST_3AK *list, int id, NODE *op_1, NODE *op_2, NODE *result){
  INSTRUCT new_instr;

  //urcime pamet pro novou instrukci
  new_instr = malloc(sizeof(struct instruct));

  if(new_instr == NULL)
    return 99;

  //vlozime data
  new_instr->id = id;
  new_instr->operand_1 = *op_1;
  new_instr->operand_2 = *op_2;
  new_instr->result = *result;

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

//*************************************
