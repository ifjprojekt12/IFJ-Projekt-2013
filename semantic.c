#include "semantic.h"

// precedencni tabulka
int table[MAXINDEX][MAXINDEX]={
    //              term    +   -   *   /   <   >   === <=  >=  !== .   (   )  =
    /* 1. term */ { H,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H, H },
    /* 2. + */    { L,      H,  H,  L,  L,  H,  H,  H,  H,  H,  H,  E,  L,  H, H },
    /* 3. - */    { L,      H,  H,  L,  L,  H,  H,  H,  H,  H,  H,  E,  L,  H, H },
    /* 4. * */    { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  E,  L,  H, H },
    /* 5. / */    { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  E,  L,  H, H },
    /* 6. < */    { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H, H },
    /* 7. > */    { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H, H },
    /* 8. === */  { L,      L,  L,  L,  L,  L,  L,  H,  L,  L,  H,  L,  L,  H, H },
    /* 9. <= */   { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H, H },
    /* 10. >= */  { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H, H },
    /* 11. !== */ { L,      L,  L,  L,  L,  L,  L,  H,  L,  L,  H,  L,  L,  H, H },
    /* 12. . */   { L,      E,  E,  E,  E,  H,  H,  H,  H,  H,  H,  H,  L,  H, H },
    /* 13. ( */   { L,      L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L, H },
    /* 14. ) */   { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H, H },
    /* 15. = */   { L,      L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L, H },
};

int special_count = 0;

// hlavni funkce semantiky
int semantixer(TOKEN *array)
{
    NODE assist1 = NULL;
    char *name = NULL;

    int n=0, top = 0;
    if( array[n].type_token == 36 )     // promenna
    {
        if( aux2 != NULL )
            aux2 = NULL;

        name = makeName(array[n]);
        if( name == NULL )
            return EXIT_FAILURE;
        if( (assist1 = searchIdent(name, &root)) == NULL )
            insertVarToTree(name, array[n], &root);

        if( array[n+2].type_token == 6 || (array[n+2].type_token >= 60 && array[n+2].type_token <= 69) )
        {
            if( functions(array,n) == EXIT_FAILURE )
                return EXIT_FAILURE;
        }
        else if( expression_sem(array, n, SEMICOLON) == EXIT_FAILURE )
                return EXIT_FAILURE;
    }
    else if( array[n].type_token == 1 || array[n].type_token == 3 || array[n].type_token == 4 )  // if, elseif, while
    {
        n++;
        if( expression_sem(array, n, BRACKET) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    else if( array[n].type_token == 7 )     // return
    {
        n++;
        if( expression_sem(array, n, SEMICOLON) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    else if( array[n].type_token == 2 )     // else
    {
        if( aux2 != NULL )
            PUSHInstr( &InstrStack, aux2, 0 );      // mela by to byt instrukce skoku
    }
    else if( array[n].type_token == 43 )    // }
    {
        if( aux2 != NULL )
            aux2 = NULL;

        TOPInstr( &InstrStack, &top );
        //printf("na vrcholu semantickeho zasobniku je: %d\n", top);
        switch( top )
        {
            case 0:     // skok (mel by to byt jen else)

                if( aux != NULL )
                    aux->jump = list.last;
                POPInstr( &InstrStack, &aux, &top );
                break;

            case 1:     // if
            case 3:     // elseif

                new_instr( &list, iJUMP, NULL, NULL, NULL, NULL );
                aux2 = list.last;
                if( aux != NULL )
                    aux->jump = list.last;
                POPInstr( &InstrStack, &aux, &top );
                break;

            case 4:     // while
            case 5:     // for

                POPInstr( &InstrStack, &aux2, &top );
                new_instr( &list, iJUMP, NULL, NULL, NULL, aux2 );
                if( aux != NULL )
                    aux->jump = list.last;
                aux = aux2;
                aux2 = NULL;
                break;

            default:
                fprintf(stderr, "Na vrcholu zasobniku instrukci je neco spatne.\n");
                return EXIT_FAILURE;
        }
    }
    else if( array[n].type_token == 50 )    // EOF
    {
        new_instr(&list, iEND, NULL, NULL, NULL, NULL);
        if( aux != NULL )
            aux->jump = list.last;
    }
    else
    {
        printf("nedodelana semantika? token: %d\n",array[n].type_token);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int functions(TOKEN *array, int n)
{
    char *name = makeName(array[n]);
    if( name == NULL )
        return EXIT_FAILURE;
    NODE assist1, assist2 = searchIdent(name, &root);
    n = 4;      // zacatek vyctu argumentu funkce
    bool first = true;

    // zjisteni typu funkce a odecteni jeji hodnoty pro odpovidajici hodnotu id pro instrukce
    int type = array[2].type_token - 40;    // vestavena funkce
    if( type < 0 )
        type = 6;       // uzivatelem definovana funkce TODO

    while( array[n].type_token != 41 )   // )
    {
        if( array[n].type_token == 23 )     // preskakovani carek
        {
            n++;
            continue;
        }
        name = makeName(array[n]);
        if( name == NULL )
            return EXIT_FAILURE;
        assist1 = searchIdent(name, &root);
        if(assist1 == NULL)
        {
            if(array[n].type_token == 36)
            {
                printERR(eVAR);
                eCode = sSemVar;
                return EXIT_FAILURE;
            }

            insertVarToTree(name, array[n], &root);
            assist1 = searchIdent(name, &root);
        }
        n++;

        // vytvoreni instrukce
        new_instr(&list, type, &assist1, NULL, &assist2, NULL);
        if( first )
        {
            if( aux != NULL )
            {
                aux->jump = list.last;
                aux = NULL;
            }
            first = false;
        }
    }

    return EXIT_SUCCESS;
}

// funkce pro zapis vyrazu do postfixove notace a odeslani instrukci
int expression_sem(TOKEN *array, int n, int end)
{
    // deklarace zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);

    // pomocne pole tokenu (zatim velikost N_MAX -> udelat dynamicky !!! ) TODO
    TOKEN array_expr[N_MAX];
    int i, new = 0, old = 0, precedent = 0, type = array[0].type_token;

    for( i=0; i<N_MAX; i++)
    {
        array_expr[i].type_token = 0;
        array_expr[i].c_number = 0;
        array_expr[i].d_number = 0.0;
        array_expr[i].string = NULL;
        array_expr[i].boolean = 0;
        array_expr[i].null = 0;
        array_expr[i].id_name = NULL;
    }

    i = 0;
    while( array[n].type_token != end )      // SEMICOLON nebo BRACKET
    {
        switch( array[n].type_token )
        {
            case 30:    // string
            case 31:    // int
            case 32:    // double
            case 33:    // bool
            case 34:    // null
            case 36:    // promenna
            {
                array_expr[i++] = array[n++];
                break;
            }
            case 40:    // (
            {
                PUSH( &leStack, array[n++].type_token );
                break;
            }
            case 10:    // =
            case 11:    // -
            case 12:    // *
            case 13:    // /
            case 14:    // +
            case 15:    // .
            case 16:    // ===
            case 17:    // !==
            case 18:    // >
            case 19:    // >=
            case 20:    // <
            case 21:    // <=
            {
                if( SEmpty(&leStack) )
                {
                    PUSH( &leStack, array[n++].type_token );
                    break;
                }
                else if( (old = TOPCheck( &leStack )) == 40 )   // (
                {
                    PUSH( &leStack, array[n++].type_token );
                    break;
                }
                else
                {
                    if( old < 0 )
                    {
                        // TODO chyba
                        fprintf(stderr, "chyba, old < 0\n");
                        return EXIT_FAILURE;
                    }

                    new = Give_index(array[n].type_token);
                    old = Give_index(old);
                    precedent = table[old][new];

                    if( precedent == L )
                        PUSH( &leStack, array[n++].type_token);
                    else
                    {
                        array_expr[i++].type_token = TOPCheck( &leStack );
                        POP( &leStack );
                    }
                    break;
                }
            }
            case 41:    // )
            {
                if( (old = TOPCheck( &leStack )) != 40 )    // (
                {
                    array_expr[i++].type_token = TOPCheck( &leStack );
                    POP( &leStack );
                    break;
                }

                old = TOPCheck( &leStack );
                POP( &leStack );
                n++;
                break;
            }
            default:
            {
                fprintf(stderr, "semantika, switch default, type: %d.\n", array[n].type_token);
                eCode = sSyn;
                return EXIT_FAILURE;
            }
        }
    }

    while( !SEmpty( &leStack ) )
    {
        array_expr[i++].type_token = TOPCheck( &leStack );
        POP( &leStack );
    }

    return read_postfix(array_expr, type);
}

// funkce pro cteni postfixove notace vyrazu a odesilani instrukci interpretu
int read_postfix(TOKEN *array, int type)
{
    int i=0;
    char *name = NULL;

    NODE assist1 = NULL, assist2 = NULL, assist3 = NULL;
    TOKEN unit;

    tSNode nodeStack;
    initNode( &nodeStack );

    while( array[i].type_token != 0 )   // konec pole ?? TODO
    {
        // narazime na cisla int nebo double, booly, stringy nebo promenne
        if(array[i].type_token == 36 || (array[i].type_token >= 30 && array[i].type_token <= 34))
        {
            name = makeName(array[i]);
            if( name == NULL )
                return EXIT_FAILURE;
            assist1 = searchIdent(name, &root);
            if(assist1 == NULL)
            {
                
                if(array[i].type_token == 36)
                {
                    printERR(eVAR);
                    eCode = sSemVar;
                    return EXIT_FAILURE;
                }
                insertVarToTree(name, array[i], &root);
                assist1 = searchIdent(name, &root);
            } 

            PUSHNode( &nodeStack, assist1);
        }
        // narazili jsme na znamenko
        else if(array[i].type_token >= 10 && array[i].type_token <= 21)
        {
            TOPPOPNode(&nodeStack, &assist2);
            TOPPOPNode(&nodeStack, &assist1);

            if( !Compatible(&assist1, &assist2, array[i].type_token) )
            {
                printERR(eCOMPATIBLE);
                eCode = sSynCompatib;
                return EXIT_FAILURE;
            }

            token_init(&unit);
            unit.type_token = 37;   // vnitrni promenna semantiky
            name = makeName(unit);
            if( name == NULL )
                return EXIT_FAILURE;
            insertVarToTree(name, unit, &root);
            assist3 = searchIdent(name, &root);

            switch( array[i].type_token )
            {
                case 10:    // =
                    new_instr(&list, iASSIGN, &assist2, NULL, &assist1, NULL);
                    //printf("ASSIGN\n\tco: %s (%d)\n\tkam: %s (%d)\n",assist2->key,assist2->data.type_token,assist1->key,assist1->data.type_token);
                    break;
                case 11:    // -
                    new_instr(&list, iMINUS, &assist1, &assist2, &assist3, NULL);
                    //printf("MINUS\n\t%s (%d) - %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    break;
                case 12:    // *
                    new_instr(&list, iMUL, &assist1, &assist2, &assist3, NULL);
                    //printf("MUL\n\t%s (%d) * %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    break;
                case 13:    // /
                    new_instr(&list, iDIV, &assist1, &assist2, &assist3, NULL);
                    //printf("DIV\n\t%s (%d) : %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    break;
                case 14:    // +
                    new_instr(&list, iPLUS, &assist1, &assist2, &assist3, NULL);
                    //printf("PLUS\n\t%s (%d) + %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    break;
                case 15:    // .
                    new_instr(&list, iKONK, &assist1, &assist2, &assist3, NULL);
                    //printf("KONK\n\t%s (%d) . %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    break;
                case 16:    // ===
                    new_instr(&list, iEQ, &assist1, &assist2, NULL, NULL);
                    //printf("EQ\n\t%s (%d) === %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );      // 0 = od ted znaci skok :D
                    PUSHInstr( &InstrStack, list.last, type );
                    break;
                case 17:    // !==
                    new_instr(&list, iNEQ, &assist1, &assist2, NULL, NULL);
                    //printf("NEQ\n\t%s (%d) !== %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );
                    PUSHInstr( &InstrStack, list.last, type );
                    break;
                case 18:    // >
                    new_instr(&list, iHIGH, &assist1, &assist2, NULL, NULL);
                    //printf("HIGH\n\t%s (%d) > %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );
                    PUSHInstr( &InstrStack, list.last, type );
                    break;
                case 19:    // >=
                    new_instr(&list, iHEQ, &assist1, &assist2, NULL, NULL);
                    //printf("HEQ\n\t%s (%d) >= %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );
                    PUSHInstr( &InstrStack, list.last, type );
                    break;
                case 20:    // <
                    new_instr(&list, iLOW, &assist1, &assist2, NULL, NULL);
                    //printf("LOW\n\t%s (%d) < %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );
                    PUSHInstr( &InstrStack, list.last, type );
                    break;
                case 21:    // <=
                    new_instr(&list, iLEQ, &assist1, &assist2, NULL, NULL);
                    //printf("LEQ\n\t%s (%d) <= %s (%d)\n",assist1->key,assist1->data.type_token,assist2->key,assist2->data.type_token);
                    if( type == 3 )
                        PUSHInstr( &InstrStack, aux2, 0 );
                    PUSHInstr( &InstrStack, list.last, type );
            }

            if( aux != NULL )
            {
                aux->jump = list.last;
                aux = NULL;
            }
            if( aux2 != NULL )
            {
                aux2 = NULL;
            }

            if( array[i].type_token >= 11 && array[i].type_token <= 15 )
                PUSHNode( &nodeStack, assist3);

            /*doplnit assist3 a jedeme na znamenka*//*
            printf("---------------------------------------------------\n");
            printf("assist1\n");
            printf("prvek ve stromu data.string: %s\n",assist1->data.string);
            printf("prvek ve stromu data.id_name: %s\n",assist1->data.id_name);
            printf("prvek ve stromu data.c_number: %d\n",assist1->data.c_number);
            printf("prvek ve stromu data.d_number: %f\n",assist1->data.d_number);
            printf("prvek ve stromu data.boolean: %d\n",assist1->data.boolean);
            printf("assist2\n");
            printf("prvek ve stromu data.string: %s\n",assist2->data.string);
            printf("prvek ve stromu data.id_name: %s\n",assist2->data.id_name);
            printf("prvek ve stromu data.c_number: %d\n",assist2->data.c_number);
            printf("prvek ve stromu data.d_number: %f\n",assist2->data.d_number);
            printf("prvek ve stromu data.boolean: %d\n",assist2->data.boolean);
            printf("---------------------------------------------------\n");
            printf("assist3\n");
            printf("prvek ve stromu data.string: %s\n",assist3->data.string);
            printf("prvek ve stromu data.id_name: %s\n",assist3->data.id_name);
            printf("prvek ve stromu data.c_number: %d\n",assist3->data.c_number);
            printf("prvek ve stromu data.d_number: %f\n",assist3->data.d_number);
            printf("prvek ve stromu data.boolean: %d\n",assist3->data.boolean);
            printf("---------------------------------------------------\n");
*/
        }

        i++; assist1 = NULL; assist2 = NULL; assist3 = NULL;    // smazat nulovani assist* - rychlost
    }

/*
    for( i=0; i<N_MAX; i++)
    {
        if( array[i].type_token == 0 )
            break;
        printf("%d ", array[i].type_token);
    }
    printf("\n");
*/

    return EXIT_SUCCESS;
}


// funkce pro vytvoreni jmena pro ukladani do stromu
char* makeName(TOKEN unit)
{
    char *name = NULL;
    int size = 1, n=0;
    if( unit.type_token == 31 )         // int
    {
        n = unit.c_number;
        // cyklus pro zjisteni delky cisla (poctu cislic)
        while( n > 9 )
        {
            size++;
            n /= 10;
        }

        // alokace retezce pro ulozeni nazvu
        if( (name = malloc((size+1) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }

        n = unit.c_number;
        // skladani retezce
        name[size--] = '\0';
        while( size >= 0 )
        {
            name[size--] = (char)((n % 10) + ASCII);
            n /= 10;
        }
    }
    else if( unit.type_token == 33 )    // bool
    {
        if( unit.boolean == 1 )
            name = "true";
        else
            name = "false";
    }
    else if( unit.type_token == 32)                               // double
    {
        // vypocteni delky cisla (poctu cislic)
        n = (int)unit.d_number;
        while( n > 9 )
        {
            size++;
            n /= 10;
        }
        size+=7;        // pripocteni tecky a 6 desetinnych mist

        // alokace retezce pro velikost nazvu
        if( (name = malloc((size+1) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }

        // skladani nazvu do retezce
        n = (int)((unit.d_number - (int)unit.d_number) * 1000000);  // prevod vsech desetinnych mist pred desetinnou carku
        name[size--] = '\0';
        for( int count = 6; count > 0; count-- )    // 6 desetinnych mist
        {
            name[size--] = (char)((n % 10) + ASCII);
            n /= 10;
        }
        name[size--] = '.';
        n = (int)unit.d_number;
        while( size >= 0 )
        {
            name[size--] = (char)((n % 10) + ASCII);
            n /= 10;
        }
    }
    else if( unit.type_token == 34)    // null
        name = "null";
    else if( unit.type_token == 30)    // string
    {
        size = strlen(unit.string);
        // alokace retezce pro ulozeni nazvu
        if( (name = malloc((size+2) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }

        name[0] = 0x06; //ACK
        name[size+1] = '\0';
        while(size >= 0){
            name[size+1] = unit.string[size];
            size--;
        }   
    }       
    else if( unit.type_token == 36)    // promenna
    {   
        size = strlen(unit.id_name);
        // alokace retezce pro ulozeni nazvu
        if( (name = malloc((size+2) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }

        name[0] = 0x05; //ENQ
        name[size+1] = '\0';
        while(size >= 0)
        {
            name[size+1] = unit.id_name[size];
            size--;
        }   
    }
    else if( unit.type_token == 37 )        // interni promenna semantiky
    {
        n = special_count;
        // cyklus pro zjisteni delky cisla (poctu cislic)
        while( n > 9 )
        {
            size++;
            n /= 10;
        }

        if( (name = malloc((size+2) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }
        
        name[0] = 0x07;     // BELL
        name[size+1] = '\0';
        n = special_count++;
        while( size >= 0 )
        {
            name[size--] = (char)((n % 10) + ASCII);
            n /= 10;
        }
    }

    return name;
}

// funkce pro porovnani dvou datovych typu
bool Compatible(NODE *a1, NODE *a2, int sign)
{
    //printf("Compatible?\t%s (%d) -%d- %s (%d)\n",(*a1)->key,(*a1)->data.type_token,sign,(*a2)->key,(*a2)->data.type_token);

    switch( sign )
    {
        case 11:    // -
        case 12:    // *
        case 13:    // /
        case 14:    // +

            if( ((*a1)->data.type_token != 31 && (*a1)->data.type_token != 32 && (*a1)->data.type_token != 36 &&
                (*a1)->data.type_token != 37) || ((*a2)->data.type_token != 31 && (*a2)->data.type_token != 32 &&
                (*a2)->data.type_token != 36 && (*a2)->data.type_token != 37 ))

                return false;
            break;

        case 15:    // .

            if( (*a1)->data.type_token != 30 && (*a1)->data.type_token != 36 && (*a1)->data.type_token != 37 )
                return false;
            break;

        case 18:    // >
        case 19:    // >=
        case 20:    // <
        case 21:    // <=

            if( (*a1)->data.type_token != 36 && (*a1)->data.type_token != 37 &&
                (*a1)->data.type_token != (*a2)->data.type_token && (*a2)->data.type_token != 36 &&
                (*a2)->data.type_token != 37 )
                    return false;
    }

    return true;
}

// funkce pro prevod typu tokenu na index do precedencni tabulky
int Give_index( int type )
{
    switch( type )
    {
        case 14:    return ADD;
        case 11:    return SUB;
        case 12:    return MUL;
        case 13:    return DIV;
        case 20:    return LOW;
        case 18:    return HIGH;
        case 16:    return EQUAL;
        case 21:    return ELOW;
        case 19:    return EHIGH;
        case 17:    return ENOT;
        case 15:    return KON;
        case 40:    return LBRACKET;
        case 41:    return RBRACKET;
        case 10:    return ASSIGN;
        default:    break;
    }
    return -1;
}
