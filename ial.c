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
        node->params = NULL;
        node->position = 0;
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
    // pokud neni nikde nic tak vraci null
    return NULL;
}

// funkce zkopiruje puvodni strom promennych do noveho
// novy strom MUSI byt inicializovany
NODE copyTree (NODE *ptr)
{
    NODE tree = *ptr;
    if (tree == NULL)
    {
        return NULL;
    }
    else
    {
        NODE treeNew = malloc(sizeof(struct NODE));
        if( treeNew == NULL )
            return NULL;

        treeNew->key = malloc((strlen(tree->key)+1) * sizeof(char));
        if( treeNew->key == NULL )
            return NULL;

        strcpy(treeNew->key, tree->key);
        treeNew->data.type_token = tree->data.type_token;
        treeNew->data.c_number = tree->data.c_number;
        treeNew->data.d_number = tree->data.d_number;

        if (tree->data.string != NULL)
        {
            treeNew->data.string = malloc((strlen(tree->data.string)+1) * sizeof(char));
            if( treeNew->data.string == NULL )
                return NULL;
            strcpy(treeNew->data.string, tree->data.string);
        }

        treeNew->data.boolean = tree->data.boolean;
        treeNew->data.null = tree->data.null;

        if( treeNew->data.id_name != NULL )
        {
            treeNew->data.id_name = malloc((strlen(tree->data.id_name)+1) * sizeof(char));
            if( treeNew->data.id_name == NULL )
                return NULL;
            strcpy(treeNew->data.id_name, tree->data.id_name);
        }

        treeNew->position = tree->position;
        treeNew->body = NULL;

        treeNew->params = NULL;
        if( tree->params != NULL )
            treeNew->params = copyTree(&(tree->params));

        if (tree->LPtr != NULL) treeNew->LPtr = copyTree(&(tree->LPtr));
        else treeNew->LPtr = NULL;
        if (tree->RPtr != NULL) treeNew->RPtr = copyTree(&(tree->RPtr));
        else treeNew->RPtr = NULL;
        return treeNew;
    }
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
    STACK stack = malloc(sizeof(struct STACK));
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

void reverse(char s[]) {
  int c, i, j;
  for( i=0, j=strlen(s)-1; i<j; i++, j--)
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// funkce vrací seøazený øetìzec podle shell sortu
char *my_sort_string (char *old_string) {

    char*string = malloc(strlen(old_string));
    strcpy(string, old_string);

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
    reverse(string);
    return string;
}

// pomocna funkce pro find
char *calculate_prefix(char *substring)
{
    int k = -1;
    int substring_length = strlen(substring);
    char *prefix = malloc(substring_length*sizeof(char));
    prefix[0] = k;
    for (int i = 1; i < substring_length; i++)
    {
        while (k > -1 && substring[k+1] != substring[i]) k = prefix[k];
        if (substring[i] == substring[k+1]) k++;
        prefix[i] = k;
    }
    return prefix;
}

// funkce hleda substring v retezci pomoci Knuth-Moris-Prattova algoritmu
int find_string (char *string, char *substring)
{
    int k = -1;
    int string_length = strlen(string);
    int substring_length = strlen(substring);
    if (substring_length == 0) return 0; // prazdny retezec
    char *prefix = calculate_prefix(substring);

    for (int i = 0; i < string_length; i++)
    {
        while (k > -1 && substring[k+1] != string[i]) k = prefix[k];
        if (string[i] == substring[k+1]) k++;

	if (k == substring_length - 1)
        {
            free(prefix);
            return (i-k);
        }
    }
    free(prefix);
    return -1;
}
