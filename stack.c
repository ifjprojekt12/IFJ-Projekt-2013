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

void PUSHInstr(tSInstr *S, INSTRUCT val)
{
    tInstrPtr ptr = NULL;
    ptr = malloc(sizeof(struct tInstr));
    ptr->Instr = val;
    ptr->ptr=S->Last;
    S->Last = ptr;
}

void POPInstr(tSInstr *S, INSTRUCT *val)
{
    *val = S->Last->Instr;
    tInstrPtr ptr = NULL;
    ptr = S->Last;
    S->Last = S->Last->ptr;
    free(ptr);
}

bool SEmptyInstr(tSInstr *S)
{
    return (S->Last == NULL ? true : false);
}

//-----------------------
//fronta pro instrukce
//-----------------------
void init_front_3ak(FRONT_3AK *F){
	F->first = NULL;
	F->last = NULL;
}

void insert_front_3ak(FRONT_3AK * F, INSTRUCT val){
	tInstrPtr ptr = NULL;
	ptr = malloc(sizeof(struct tInstr));
	ptr->instr = val;
	ptr->ptr=NULL;
	if ( F->first == NULL && F->last == NULL ){
		F->first = ptr;
		F->last = ptr;
	}else{
		F->last->ptr = ptr;
		F->last = ptr;
		
	}

}

void take_out_3ak(FRONT_3AK *F, INSTRUCT *val){
	*val = F->first->instr;
	tInstrPtr ptr = NULL;
	ptr = F->first;
	F->first = F->first->ptr;
	free(ptr);
		
}

bool front_empty(FRONT_3AK *F){
	if (F->first == NULL && F->last == NULL) {
		return true;
	}else{
		return false;
	}
}

void Dispose_front (FRONT_3AK *F)	{
	tInstrPtr ptr = NULL;
	tInstrPtr Del = NULL;
	ptr = F->first;
	while (ptr != NULL)
	{
		Del = ptr;
		ptr = ptr->ptr;
		free(Del);
	}
	F->last = NULL;
	F->first = NULL;
}
