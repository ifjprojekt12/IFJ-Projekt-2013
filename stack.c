#include "stack.h"

void init (tStack *S)   {
    S->Last = NULL;
}

void DisposeStack (tStack *S)   {
    tElemPtr ptr = NULL;
    tElemPtr Del = NULL;
    ptr = S->Last;
    while (ptr != NULL)
    {
        Del = ptr;
        ptr = ptr->ptr;
        free(Del);
    }
    S->Last = NULL;
}

void PUSH (tStack *S, int val)  {
        tElemPtr ptr = NULL;
    ptr =  malloc(sizeof(struct tElem));
    ptr->Elem = val;
    ptr->ptr=S->Last;
    S->Last = ptr;
}

void TOP (tStack *S, int *val)      {
        *val = S->Last->Elem;
}

int TOPCheck(tStack *S)     {
    return S->Last->Elem;
}

void POP (tStack *S)    {
    tElemPtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    free(ptr);
}

bool SEmpty(tStack *S){
    if (S->Last == NULL){
        return true;
    }else{
        return false;
    }
}

void initNode(tSNode *S){
    S->Last = NULL;
}

/*void DisposeSNode (tSNode *S) {
    tNodePtr ptr = NULL;
    tNodePtr Del = NULL;
    ptr = S->Last;
    while (ptr != NULL)
    {
        Del = ptr;
        ptr = ptr->ptr;
        free(Del);
    }
    S->Last = NULL;
}*/

void PUSHNode (tSNode *S, NODE val) {
        tNodePtr ptr = NULL;
    ptr = malloc(sizeof(struct tNode));
    ptr->Node = val;
    ptr->ptr=S->Last;
    S->Last = ptr;
}

void TOPNode (tSNode *S, NODE *val)     {
        *val = S->Last->Node;
}

void POPNode (tSNode *S)    {
    tNodePtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    free(ptr);
}

void TOPPOPNode(tSNode *S, NODE *val){
    *val = S->Last->Node;
    tNodePtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    free(ptr);
}

/*NODE* ToPoPtr(tSNode *){
    NODE *val = S->Last->Node;
    tSNodePtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    //free(ptr);
    return val;
}*/

bool SEmptyNode(tSNode *S){
    if (S->Last == NULL){
        return true;
    }else{
        return false;
    }
}

void initInstr(tSInstr *S)
{
    S->Last = NULL;
}

void PUSHInstr(tSInstr *S, INSTRUCT val, int t)
{
    tInstrPtr ptr = NULL;
    ptr = malloc(sizeof(struct tInstr));
    ptr->Instr = val;
    ptr->type = t;
    ptr->ptr=S->Last;
    S->Last = ptr;
}

void TOPInstr(tSInstr *S, int *t)
{
    *t = S->Last->type;
}

void POPInstr(tSInstr *S, INSTRUCT *val, int *t)
{
    *val = S->Last->Instr;
    *t = S->Last->type;
    tInstrPtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    free(ptr);
}

bool SEmptyInstr(tSInstr *S)
{
    return (S->Last == NULL ? true : false);
}
