#include "ial.h"

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
            for (unsigned int i = 0; i < maxlen-1; i++)
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

NODE searchIdent (char *key, NODE *ptr)
{
    NODE node = *ptr;
    if (node == NULL) return NULL; // strom je prazdny
    if (strcmp(node->key, key) == 0) return node;
    else
    {
        unsigned int maxlen = strlen(key);
        for (unsigned int i = 0; i < maxlen; i++)
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
