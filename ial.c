#include "ial.h"

//////////////////////////////////////
///// FUNKCE PRO TABULKU SYMBOLU /////
//////////////////////////////////////

// funkce inicializuje strom
void treeInit (NODE *ptr)
{
    *ptr = NULL;
}
// funkce vlozi promennou do stromu
void insertVarToTree (char *key, TOKEN data, NODE *ptr)
{
    NODE node = *ptr;
    if (node == NULL) // strom je prazdny
    {
        node = malloc(sizeof(struct NODE));
        node->LPtr = NULL;
        node->RPtr = NULL;
        node->key = malloc((strlen(key)+1)*sizeof(char));
        strcpy(node->key, key);
        node->data = data;
        *ptr = node;
    }
    else
    {
        if((strcmp(node->key, key) == 0)) // klic je stejny -> prepise se hodnota promenne
        {
            node->data = data;
        }
        else
        {
            unsigned int maxlen = strlen(key); // maximalni delka klice
            if (strlen(node->key) > maxlen) maxlen = strlen(node->key);
            for (unsigned int i = 0; i < maxlen; i++)
            {
                if (i == strlen(key) && i < strlen(node->key))
                {
                    insertVarToTree(key, data, &node->LPtr);
                    break;
                }
                else if (i == strlen(node->key) && i < strlen(key))
                {
                    insertVarToTree(key, data, &node->RPtr);
                    break;
                }
                if (key[i] > node->key[i]) // klic pridavaneho prvku je vetsi nez klic aktualni polozky stromu -> doprava
                {
                    insertVarToTree(key, data, &node->RPtr);
                    break;
                }
                else if (key[i] < node->key[i]) // mensi -> doleva
                {
                    insertVarToTree(key, data, &node->LPtr);
                    break;
                }
            }
        }
    }
}
// funkce vyhleda promennou ve stromu a vrati jeji hodnotu, popr. NULL
NODE searchIdent (char *key, NODE *ptr)
{
    NODE node = *ptr;
    if (node == NULL) return NULL; // strom je prazdny
    if (strcmp(node->key, key) == 0) return node;
    else
    {
        unsigned int maxlen = strlen(key);
        for (unsigned int i = 0; i < maxlen+1; i++)
        {
            if (i == maxlen)
            {
                node = searchIdent(key, &node->LPtr);
                return node;
            }
            if (key[i] > node->key[i]) // hledany klic je vetsi nez klic aktualniho prvku -> doprava
            {
                node = searchIdent(key, &node->RPtr);
                return node;
            }
            else if (key[i] < node->key[i]) // mensi -> doleva
            {
                node = searchIdent(key, &node->LPtr);
                return node;
            }
        }
    }
    printf("Hledas neco co tam neni\n");
    return NULL;
}

// funkce zkopiruje puvodni strom promennych do noveho
// novy strom MUSI byt inicializovany
NODE copyTree (NODE *ptr, NODE *ptrNew)
{
    NODE tree = *ptr;
    NODE treeNew = *ptrNew;
    if (tree == NULL)
    {
        treeNew = NULL;
        return treeNew;
    }
    else
    {
        treeNew->key = malloc((strlen(tree->key)+1) * sizeof(char));
        strcpy(treeNew->key, tree->key);
        treeNew->data.type_token = tree->data.type_token;
        treeNew->data.c_number = tree->data.c_number;
        treeNew->data.d_number = tree->data.d_number;
        if (tree->data.string != NULL)
        {
            treeNew->data.string = malloc((strlen(tree->data.string)+1) * sizeof(char));
            strcpy(treeNew->data.string, tree->data.string);
        }
        treeNew->data.boolean = tree->data.boolean;
        treeNew->data.null = tree->data.null;
        treeNew->data.id_name = malloc((strlen(tree->data.id_name)+1) * sizeof(char));
        strcpy(treeNew->data.id_name, tree->data.id_name);

        if (tree->LPtr != NULL) treeNew->LPtr = copyTree(&tree->LPtr, &treeNew->LPtr);
        else treeNew->LPtr = NULL;
        if (tree->RPtr != NULL) treeNew->RPtr = copyTree(&tree->RPtr, &treeNew->RPtr);
        else treeNew->RPtr = NULL;
    }
    return treeNew;
}

NODE searchParam (int index, NODE *ptr)
{
    NODE tree = *ptr;
    NODE treeSearched  = NULL;
    if (tree->position == index) return tree;
    else
    {
        if ((tree->LPtr == NULL) && (tree->RPtr == NULL)) return NULL;
        if (tree->LPtr != NULL)
        {
            if ((treeSearched = searchParam(index, &tree->LPtr)) != NULL) return treeSearched;
        }
        if (tree->RPtr != NULL)
        {
            if ((treeSearched = searchParam(index, &tree->RPtr)) != NULL) return treeSearched;
        }
    }
    return NULL;
}

//////////////////////////////////////
////////////// ZASOBNIK //////////////
//////////////////////////////////////

// inicializace zasobniku
void stackInit (STACK *zasobnik)
{
    *zasobnik = NULL;
}

// vlozeni prvku na vrchol zasobniku
void push (STACK *zasobnik, NODE *ptr)
{
    STACK stack = malloc(sizeof(STACK));
    if (*zasobnik == NULL) // zasobnik je prazdny
    {
        stack->ptr = ptr;
        stack->next = NULL;
        *zasobnik = stack;
    }
    else
    {
        stack->ptr = ptr;
        stack->next = *zasobnik;
        *zasobnik = stack;
    }
}

// funkce odstrani prvek z vrcholu zasobniku a vrati odkaz na nej
NODE pop (STACK *zasobnik)
{
    STACK stack = *zasobnik;
    NODE prvek;
    prvek = *(stack->ptr);
    *zasobnik = stack->next;
    return prvek;
}

//////////////////////////////////////
////////// VESTAVENE FUNKCE //////////
//////////////////////////////////////

// funkce vrac� se�azen� �et�zec podle shell sortu
char * my_sort_string (char *string)
{
    unsigned int length = strlen(string);
    int increment = length / 2;
    int j = 0; char pom;
    while (increment > 0)
    {
        for (unsigned int i = 0; i < length - increment; i++)
        {
            j = i + increment;
            pom = string [j];
            while (j >= increment && pom > string [j - increment])
            {
                string[j] = string[j - increment];
                j -= increment;
            }
            string[j] = pom;
        }
        if (increment == 2) increment = 1;
        else increment /= 2.2;
    }
    return string;
}
// pomocna funkce pro find
char *vypocitejPrefix (char podretezec[])
{
    int k = -1;
    int delkaPodretezce = strlen(podretezec);
    char *prefix = malloc(delkaPodretezce*sizeof(char));
    prefix[0] = k;
    for (int i = 1; i < delkaPodretezce; i++)
    {
        while (k > -1 && podretezec[k+1] != podretezec[i]) k = prefix[k];
        if (podretezec[i] == podretezec[k+1]) k++;
        prefix[i] = k;
    }
    return prefix;
}

// funkce hleda podretezec v retezci pomoci Knuth-Moris-Prattova algoritmu
int find_string (char *retezec, char *podretezec)
{
    // malej hack pro ty nase retezce
    if (strlen(retezec) == 512)
	 for (int i = 0; i<512; i++)
		 if (retezec[i] == 0)
			retezec[i] = '\0';
	
    if (strlen(podretezec) == 512)
	 for (int i = 0; i<512; i++)
		 if (podretezec[i] == 0)
			podretezec[i] = '\0';

    int k = -1;
    int delkaRetezce = strlen(retezec);
    int delkaPodretezce = strlen(podretezec);
    if (delkaPodretezce == 0)
	return 0.0;
    char *prefix = vypocitejPrefix(podretezec);

    for (int i = 0; i < delkaRetezce; i++)
    {
        while (k > -1 && podretezec[k+1] != retezec[i]) k = prefix[k];
        	if (retezec[i] == podretezec[k+1]) k++;

	if (k == delkaPodretezce - 1)
        {
            free(prefix);
            return (i-k);
        }
    }
    free(prefix);
    return -1.0;
}
