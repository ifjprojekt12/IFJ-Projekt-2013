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

// funkce vraci delku retezce
unsigned int my_strlen (char *string)
{
    int i = 0;
    unsigned int length = 0;
    while (string[i] != '\0')
    {
        length++;
    }
    return length;
}

// funkce vraci podretezec z retezce ohraniceny indexy
char * my_get_substring (char *string, int start, int end)
{
    char *new_string = malloc((end - start + 1) + sizeof (char));
    for (int i = 0; i < (end-start); i++)
    {
        new_string[i] = string[i+start];
    }
    new_string[end-start] = '\0';
    return new_string;
}

// funkce precte radek ze standardniho vstupu a vrati ho jako retezec
char * my_get_string ()
{
    char buffer[BUFFERSIZE] = {0};
    char *string;
    int c, i  = 0;
    while ((c = getchar()) != EOF || c != '\n' || c != '\r') // cte se ze vstupu dokud se nenarazi na konec radku nebo EOF
    {
        buffer[i] = c;
        i++;
    }
    string = malloc ((i+1) * sizeof (char));
    strncpy (string, buffer, i);
    string[i] = '\0';
    return string;
}

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
