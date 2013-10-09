#include "stack.h"

void init (tStack *S)	{
	S->Last = NULL;
}

void DisposeStack (tStack *S)	{
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

void PUSH (tStack *S, int val)	{
    	tElemPtr ptr = NULL;
	ptr =  malloc(sizeof(struct tElem));
	ptr->Elem = val;
	ptr->ptr=S->Last;
	S->Last = ptr;
}

void TOP (tStack *S, int *val)		{
		*val = S->Last->Elem;
}

void POP (tStack *S)	{
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
