/**

Implementace interpretu imperativniho jazyka IFJ13.

xhanak27 - Jan Hanak
xfiser10 - Jakub Fiser

*/
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
    //printf("semantixer\n");
    // deklarace pomocnych promennych
    LIST_3AK *dest = &list;
    NODE *dest_root = &root;
    if( func != NULL )
    {
        dest = func->body;
        dest_root = &(func->params);
    }

    NODE assist1 = NULL;
    char *name = NULL;
    int n=0, top = 0;

    if( array[n].type_token == 36 )     // promenna
    {
        name = makeName(array[n]);
        if( name == NULL )
            return EXIT_FAILURE;
        if( (assist1 = searchIdent(name, dest_root)) == NULL )
        {
            // prvni vyskyt promenne, jeji zapis do stromu
            insertVarToTree(name, array[n], dest_root);
            if( eCode != sOK )
            {
                printERR(eINTERN);
                return EXIT_FAILURE;
            }
        }
        else
            free(name);

        if( array[n+2].type_token == 35 || (array[n+2].type_token >= 60 && array[n+2].type_token <= 69) )
        {
            // volani vestavene nebo uzivatelem definovane funkce
            if( functions(array,n) == EXIT_FAILURE )
                return EXIT_FAILURE;
        }
        else if( expression_sem(array, &n, SEMICOLON, false) == EXIT_FAILURE )
            // vyraz prirazeni
            return EXIT_FAILURE;
    }
    else if( array[n].type_token == 1 || array[n].type_token == 3 || array[n].type_token == 4 )  // if, elseif, while
    {
        n++;
        if( expression_sem(array, &n, BRACKET, false) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    else if( array[n].type_token == 7 )     // return
    {
        n++;
        if( expression_sem(array, &n, SEMICOLON, false) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    else if( array[n].type_token == 2 )     // else
    {
        // vytvoreni skokove instrukce a jeji ulozeni na zasobnik
        new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
        PUSHInstr( &InstrStack, dest->last, array[n].type_token );
    }
    else if( array[n].type_token == 43 )    // }
    {
        if( func_end )
        {
            // bude potreba vytvorit praznou skokovou instrukci v pripade, kdy funkce konci
            // neuplnym ifem bez prikazu return
            new_instr(dest, iEND_FCE, NULL, NULL, NULL, NULL );
            if( aux != NULL )
                aux->jump = dest->last;
            if( !SEmptyInstr( &InstrStack ) )
            {
                TOPInstr( &InstrStack, &top );
                while( top == 43 )
                {
                    POPInstr( &InstrStack, &aux, &top );
                    aux->jump = dest->last;
                    if( !SEmptyInstr( &InstrStack) )
                        TOPInstr( &InstrStack, &top);
                    else
                        break;
                }
            }
            aux = NULL;
            func = NULL;
            func_end = false;
        }
        else
        {
            bool quit = false;
            bool wasFor = false;
            INSTRUCT aux2 = NULL, aux3 = NULL;
            new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
            TOPInstr( &InstrStack, &top );
            if( top == 4 || top == 5 )  // pro while nebo for si ulozime instrukci aux
                aux2 = aux;
            else if( aux != NULL )
            {
                aux->jump = dest->last;
                aux = NULL;
            }
            POPInstr( &InstrStack, &aux, &top );

            while( !quit )
            {
                //printf("na vrcholu semantickeho zasobniku je: %d\n", top);
                switch( top )
                {
                    case 1:     // if
                    case 3:     // elseif

                        if( aux->id == iBVAL )
                            aux = aux->right;
                        PUSHInstr( &InstrStack, dest->last, array[n].type_token );
                        quit = true;
                        break;

                    case 43:    // }

                        aux->jump = dest->last;
                        POPInstr( &InstrStack, &aux, &top );
                        break;

                    case 5:     // for

                        POPInstr( &InstrFor, &aux3, &top );
                        bool first = true;
                        while( aux3 != NULL )
                        {
                            NODE help1 = NULL, help2 = NULL, help3 = NULL;
                            if( aux3->operand_1 != NULL )
                                help1 = aux3->operand_1;
                            if( aux3->operand_2 != NULL )
                                help2 = aux3->operand_2;
                            if( aux3->result != NULL )
                                help3 = aux3->result;
                            new_instr(dest, aux3->id, &help1, &help2, &help3, aux3->jump);
                            if( first && aux2 != NULL )
                                aux2->jump = dest->last;

                            first = false;
                            aux3 = aux3->right;
                        }
                        new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
                        wasFor = true;

                    case 4:     // while

                        dest->last->jump = aux;
                        if( aux->id == iBVAL )
                            aux = aux->right;
                        new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
                        if( wasFor )
                        {
                            POPInstr(&InstrBreak, &aux3, &top);
                            while( aux3 != NULL && aux3->id == iJUMP )
                            {
                                aux3->jump = dest->last;
                                aux3 = aux3->right;
                                POPInstr(&InstrBreak, &aux3, &top);
                            }
                        }
                        if( aux2 != NULL )
                            aux2->jump = dest->last;
                        quit = true;
                        break;

                    case 2:     // else

                        aux = NULL;

                    default:

                        PUSHInstr( &InstrStack, dest->last, array[n].type_token );
                        quit = true;
                }
            }
        }
    }
    else if( array[n].type_token == 50 )    // EOF
    {
        new_instr(dest, iEND, NULL, NULL, NULL, NULL);
        if( aux != NULL )
            aux->jump = dest->last;
        while( !SEmptyInstr( &InstrStack ) )
        {
            POPInstr( &InstrStack, &aux, &top );
            aux->jump = dest->last;
        }
    }
    else if( array[n].type_token == 5 )     // for
    {
        n+=2;
        if( array[n].type_token != 22 )     // prvni cast hlavicky neni prazdna
        {
            name = makeName(array[n]);
            if( name == NULL )
                return EXIT_FAILURE;
            if( (assist1 = searchIdent(name, dest_root)) == NULL )
            {
                // prvni vyskyt promenne, jeji zapis do stromu
                insertVarToTree(name, array[n], dest_root);
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
            }

            if( expression_sem(array, &n, SEMICOLON, false) == EXIT_FAILURE )     // ;
                return EXIT_FAILURE;
        }
        n++;

        if( array[n].type_token != 22 )     // druha cast hlavicky neni prazdna
        {
            if( expression_sem(array, &n, SEMICOLON, false) == EXIT_FAILURE )     // ;
                return EXIT_FAILURE;
        }
        else
        {
            fprintf(stderr,"sem bych se nemel nikdy dostat.\n");
            // co kdyz bude prazdna, jakou instrukci vytvorit ?? TODO
        }
        n++;

        new_instr(dest, 80, NULL, NULL, NULL, NULL);
        PUSH_last(true, &InstrBreak);

        if( array[n].type_token != 41 )     // treti cast hlavicky neni prazdna
        {
            // ulozit na zasobnik, nejspis to nebude jen jedna instrukce, takze celkem sranda
            if( expression_sem(array, &n, B_BRACKET, true) == EXIT_FAILURE )
                return EXIT_FAILURE;
        }
        else
        {
            // ackoliv je treti cast hlavicky prazdna, je nutno do zasobniku ulozit nejakou instrukci
            // jinak bychom narusili prirazeni instrukci jednotlivym vrstvam zanoreni
            new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
            PUSH_last(true, &InstrFor);
        }
    }
    else if( array[n].type_token == 8 )     // break
    {
        // vytvorena instrukce se bude ukladat na zasobnik
        new_instr(dest, iJUMP, NULL, NULL, NULL, NULL);
        PUSHInstr(&InstrBreak, dest->last, 0);
    }
    else if( array[n].type_token == 9 )     // continue
    {
        tInstrPtr h = InstrStack.Last;

        // cyklus pro nalezeni instrukce pro for, ve ktere se zrovna nachazime
        while( h != NULL )
        {
            if( h->type == 5 )  // for
                break;
            h = h->ptr;
        }
        new_instr(dest, iJUMP, NULL, NULL, NULL, h->Instr);
    }
    else if( array[n].type_token == 6 )     // function
    {
        n++;
        assist1 = searchIdent(array[n].id_name, &tree);
        if( assist1 != NULL )
        {
            // redefinice funkce
            printERR(eFCEREDEF);
            eCode = sSemFceDef;
            return EXIT_FAILURE;
        }
        insertVarToTree(array[n].id_name, array[n], &tree);
        if( eCode != sOK )
        {
            printERR(eINTERN);
            return EXIT_FAILURE;
        }

        // ulozime si uzel pro funkci, v niz se budeme nachazet
        func = searchIdent(array[n].id_name, &tree);

        // vytvoreni a inicializace listu instrukci pro danou funkci
        if( (func->body = malloc(sizeof(struct list_3ak))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return EXIT_FAILURE;
        }
        new_instr_list(func->body);
        //treeInit( &(func->params) );

        int m = n;

        // ukladame veskere parametry do stromu k dane funkci
        n+=2;
        top = 1;                    // poradi parametru
        while( array[n].type_token != 41 )  // )
        {
            if( array[n].type_token != 23 )     // ,
            {
                name = makeName(array[n]);
                if( name == NULL )
                    return EXIT_FAILURE;
                insertVarToTree(name, array[n], &(func->params));
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist1 = searchIdent(name, &(func->params));
                assist1->position = top++;
            }
            n++;
        }

        n = m;
        // kontrola, zda byla funkce volana pred jeji definici
        NODE assist2 = searchIdent(array[n].id_name, &check_func);

        // pokud byla funkce jiz volana, existuji instrukce prirazeni parametru ovsem do nespravenho stromu
        // projdeme cely seznam instrukci a najdeme tedy tyto instrukce k aktualni funkci, prvni z nich bude v aux
        if( assist2 != NULL )
        {
            // hlavni telo programu
            correct_list(&list);

            // tela jednotlivych funkci
            NODE aux = tree;
            tSNode stack;
            initNode(&stack);

            while( aux != NULL )
            {
                PUSHNode(&stack, aux);
                correct_list(aux->body);
                aux = aux->LPtr;
            }

            while( !SEmptyNode(&stack) )
            {
                TOPPOPNode(&stack, &aux);
                aux = aux->RPtr;
                while( aux != NULL )
                {
                    PUSHNode(&stack, aux);
                    correct_list(aux->body);
                    aux = aux->LPtr;
                }
            }
        }
    }
    else
    {
        fprintf(stderr,"nedodelana semantika? token: %d\n",array[n].type_token);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int functions(TOKEN *array, int n)
{
    LIST_3AK *dest = &list;
    NODE *dest_root = &root;
    if( func != NULL )
    {
        dest = func->body;
        dest_root = &(func->params);
    }

    char *name = makeName(array[n]);
    if( name == NULL )
        return EXIT_FAILURE;
    NODE assist1, assist3, assist4;
    NODE assist2 = searchIdent(name, dest_root);    // ulozeni uzlu odpovidajicimu promenne pro prirazeni
    free(name);
    NODE substr1 = NULL, substr2 = NULL, substr3 = NULL;
    n = 2;      // zacatek vyctu argumentu funkce
    bool first = true;
    int params = 0, top;     // pocitani argumentu pro kontrolu nedostatku ci prebytku

    // zjisteni typu funkce a odecteni jeji hodnoty pro odpovidajici hodnotu id pro instrukce
    int type = array[n].type_token - 40;

    if( type < 0 )      // uzivatelem definovana funkce
    {
        bool def = true;
        int x = 1;
        TOKEN unit;
        token_init(&unit);
        unit.type_token = 31;

        assist1 = searchIdent(array[n].id_name, &tree);
        if( assist1 == NULL && (assist1 = searchIdent(array[n].id_name, &check_func)) == NULL )
        {
            // definice funkce se nachazi az za jejim volanim
            insertVarToTree(array[n].id_name, array[n], &check_func);
            if( eCode != sOK )
            {
                printERR(eINTERN);
                return EXIT_FAILURE;
            }
            assist1 = searchIdent(array[n].id_name, &check_func);
            treeInit( &(check_func->params) );
            def = false;
        }
        n+=2;

        // prirazeni volani funkce do promenne
        new_instr(dest, iFUNCTION, &assist1, NULL, NULL, NULL);

        if( aux != NULL )
        {
            aux->jump = dest->last;
            aux = NULL;
        }
        if( !SEmptyInstr( &InstrStack ) )
        {
            TOPInstr( &InstrStack, &top );
            while( top == 43 )
            {
                POPInstr( &InstrStack, &aux, &top );
                aux->jump = dest->last;
                aux = NULL;
                if( !SEmptyInstr( &InstrStack ) )
                    TOPInstr( &InstrStack, &top );
                else
                    break;
            }
        }

        // prirazeni hodnot parametrum
        while(array[n].type_token != 41 )       // )
        {
            if( array[n].type_token == 23 )
            {
                n++;
                continue;
            }

            if( !def )
            {
                unit.c_number = x;
                name = makeName(unit);      // vytvoreni nazvu z cisla poradi
                if( name == NULL )
                    return EXIT_FAILURE;
                insertVarToTree(name, array[n], &(assist1->params));
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist4 = searchIdent(name, &(assist1->params));
                assist4->position = x;
            }
            assist4 = searchParam(x++, &(assist1->params));     // najde parametr na pozici x
            if( assist4 == NULL )
                break;      // prebytecne parametry jsou ignorovany

            name = makeName(array[n]);
            if( name == NULL )
                return EXIT_FAILURE;
            assist3 = searchIdent(name, dest_root);     // hledani hodnoty/promenne ve stromu
            if( assist3 == NULL )                       // hledani neuspesne
            {
                insertVarToTree(name, array[n], dest_root);     // vlozeni hodnoty do stromu
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist3 = searchIdent(name, dest_root);
            }
            else
                free(name);

            new_instr(dest, iSAVE_PAR, &assist3, NULL, &assist4, NULL); // vytvoreni instrukce pro ulozeni hodnoty parametru
            n++;
        }

        // detekce chybejicich parametru pri volani jiz definovave funkce
        if( assist1->params != NULL && (assist4 = searchParam(x, &(assist1->params))) != NULL )
        {
            printERR(eFCEPARAM);
            eCode = sSemFceParam;
            return EXIT_FAILURE;
        }

        new_instr(dest, iASSIGN, &assist1, NULL, &assist2, NULL);
    }
    else        // volani vestavene funkce
    {
        n = 4;
        if( type == iP_STR )
        {
            new_instr(dest, iP_STR_NEW, NULL, NULL, NULL, NULL);   // uvodni instrukce pro funkci put_string
            if( aux != NULL )
            {
                aux->jump = dest->last;
                aux = NULL;
            }

            if( !SEmptyInstr( &InstrStack ) )
            {
                TOPInstr( &InstrStack, &top );
                while( top == 43 )  // }
                {
                    POPInstr( &InstrStack, &aux, &top );
                    aux->jump = dest->last;
                    aux = NULL;
                    if( !SEmptyInstr( &InstrStack ) )
                        TOPInstr( &InstrStack, &top );
                    else
                        break;
                }
            }
            first = false;
        }
        else if( type == iG_STR )   // get_string()
        {
            new_instr(dest, type, NULL, NULL, &assist2, NULL);
            if( aux != NULL )
            {
                aux->jump = dest->last;
                aux = NULL;
            }

            if( !SEmptyInstr( &InstrStack ) )
            {
                TOPInstr( &InstrStack, &top );
                while( top == 43 )  // }
                {
                    POPInstr( &InstrStack, &aux, &top );
                    aux->jump = dest->last;
                    aux = NULL;
                    if( !SEmptyInstr( &InstrStack ) )
                        TOPInstr( &InstrStack, &top );
                    else
                        break;
                }
            }
            first = false;
        }

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
            assist1 = searchIdent(name, dest_root);
            if(assist1 == NULL)
            {
                /*
                if(array[n].type_token == 36)
                {
                    // nedeklarovana promenna
                    printERR(eVAR);
                    eCode = sSemVar;
                    return EXIT_FAILURE;
                }
                */

                insertVarToTree(name, array[n], dest_root);
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist1 = searchIdent(name, dest_root);
            }
            else
                free(name);
            n++;
            params++;   // pocitame pocet parametru

            // ignorovani prebytecnych parametru pro jednotlive vestavene funkce
            if( (((type >= iBVAL && type <= iSVAL) || type == iSTRLEN || type == iS_STR) && params <= 1 ) || (type == iG_SUBSTR && params <= 3 )
                || (type == iF_STR && params <= 2) || type == iP_STR )
            {
                if( type == iG_SUBSTR )
                {
                    if( substr1 == NULL )
                        substr1 = assist1;
                    else if( substr2 == NULL )
                        substr2 = assist1;
                    else if( substr3 == NULL )
                    {
                        substr3 = assist1;
                        new_instr(dest, type, &substr1, &substr2, &substr3, NULL);
                        if( first )
                        {
                            if( aux != NULL )
                            {
                                aux->jump = dest->last;
                                aux = NULL;
                            }
                            if( !SEmptyInstr( &InstrStack ) )
                            {
                                TOPInstr( &InstrStack, &top );
                                while( top == 43 )
                                {
                                    POPInstr( &InstrStack, &aux, &top );
                                    aux->jump = dest->last;
                                    aux = NULL;
                                    if( !SEmptyInstr( &InstrStack ) )
                                        TOPInstr( &InstrStack, &top );
                                    else
                                        break;
                                }
                            }
                            first = false;
                        }
                        new_instr(dest, iG_SUBSTR2, NULL, NULL, &assist2, NULL);
                    }
                }
                else if( type == iF_STR )
                {
                    if( substr1 == NULL )
                        substr1 = assist1;
                    else
                    {
                        new_instr(dest, type, &substr1, &assist1, &assist2, NULL );
                        if( first )
                        {
                            if( aux != NULL )
                            {
                                aux->jump = dest->last;
                                aux = NULL;
                            }
                            if( !SEmptyInstr( &InstrStack ) )
                            {
                                TOPInstr( &InstrStack, &top );
                                while( top == 43 )
                                {
                                    POPInstr( &InstrStack, &aux, &top );
                                    aux->jump = dest->last;
                                    aux = NULL;
                                    if( !SEmptyInstr( &InstrStack ) )
                                        TOPInstr( &InstrStack, &top );
                                    else
                                        break;
                                }
                            }
                            first = false;
                        }
                    }
                }
                else
                {
                    // vytvoreni instrukce
                    new_instr(dest, type, &assist1, NULL, &assist2, NULL);
                    if( first )
                    {
                        if( aux != NULL )
                        {
                            aux->jump = dest->last;
                            aux = NULL;
                        }
                        if( !SEmptyInstr( &InstrStack ) )
                        {
                            TOPInstr( &InstrStack, &top );
                            while( top == 43 )
                            {
                                POPInstr( &InstrStack, &aux, &top );
                                aux->jump = dest->last;
                                aux = NULL;
                                if( !SEmptyInstr( &InstrStack ) )
                                    TOPInstr( &InstrStack, &top );
                                else
                                    break;
                            }
                        }
                        first = false;
                    }
                }
            }
        }

        // kontrola chybejicich parametru pro jednotlive vestavene funkce
        if( (((type >= iBVAL && type <= iSVAL) || type == iSTRLEN || type == iS_STR) && params < 1 ) || (type == iG_SUBSTR && params < 3 )
            || (type == iF_STR && params < 2) )
        {
            printERR(eFCEPARAM);
            eCode = sSemFceParam;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

// funkce pro zapis vyrazu do postfixove notace a odeslani instrukci
int expression_sem(TOKEN *array, int *m, int end, bool is_for)  // m = index v poli
{
    // deklarace zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);

    TOKEN *array_expr;
    int i = 0, max = 64, new = 0, old = 0, precedent = 0, type, n = *m;
    initialize_array( &array_expr, i, max );

    if( is_for )
        // cislo 50 bude znacit, ze se nachazime ve treti hlaviky cyklu for, o cemz nas informuje is_for
        // zvolil jsem cislo 50, jelikoz neni pouzito a cislo 5 nalezi token FOR
        type = 50;
    else
        type = array[0].type_token;

    for( ; i<max; i++)
        token_init(&(array_expr[i]));

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
                if( i == max && realloc_array(array_expr, &max) == EXIT_FAILURE )
                    return EXIT_FAILURE;
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
                        free(array_expr);
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
                        if( i == max && realloc_array(array_expr, &max) == EXIT_FAILURE )
                            return EXIT_FAILURE;
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
                    if( i == max && realloc_array(array_expr, &max) == EXIT_FAILURE )
                        return EXIT_FAILURE;
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
                // TODO
                eCode = sSyn;
                free(array_expr);
                return EXIT_FAILURE;
            }
        }
    }

    // jednoprvkove vyrazy
    while( !SEmpty( &leStack ) )
    {
        array_expr[i++].type_token = TOPCheck( &leStack );
        if( i == max && realloc_array(array_expr, &max) == EXIT_FAILURE )
           return EXIT_FAILURE;
        POP( &leStack );
    }

    *m = n;
    n = read_postfix(array_expr, type, max);
    free(array_expr);
    return n;
}

// funkce pro cteni postfixove notace vyrazu a odesilani instrukci interpretu
int read_postfix(TOKEN *array, int type, int max)
{
    /*
    printf("read_postfix array: ");
    for(int h=0; h<max; h++)
    {
        if( array[h].type_token == 0 )
            break;
        printf("%d, ",array[h].type_token);
    }
    printf("\n"); */

    // pomocne promenne
    int i=0, top;
    char *name = NULL;
    NODE assist1 = NULL, assist2 = NULL, assist3 = NULL;
    TOKEN unit;
    tSNode nodeStack;
    initNode( &nodeStack );

    bool first = false;     // rozliseni treti casti hlavicky for a tedy ukladani instrukci na zasobnik
    if( type == 50 )
        first = true;

    // cil pro ukladani instrukci (hlavni telo programu nebo telo funkce)
    LIST_3AK *dest = &list;
    NODE *dest_root = &root;
    if( func != NULL )
    {
        dest = func->body;
        dest_root = &(func->params);
    }

    while( i < max && array[i].type_token != 0 )
    {
        // narazime na cislo int nebo double, bool, string nebo promennou
        if(array[i].type_token == 36 || (array[i].type_token >= 30 && array[i].type_token <= 34))
        {
            name = makeName(array[i]);
            if( name == NULL )
                return EXIT_FAILURE;
            assist1 = searchIdent(name, dest_root);
            if(assist1 == NULL)
            {               
                /*
                if(array[i].type_token == 36)
                {
                    // nedeklarovana promenna
                    printERR(eVAR);
                    eCode = sSemVar;
                    return EXIT_FAILURE;
                }
                */

                insertVarToTree(name, array[i], dest_root);
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist1 = searchIdent(name, dest_root);
            } 
            else
                free(name);

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
            insertVarToTree(name, unit, dest_root);
            if( eCode != sOK )
            {
                printERR(eINTERN);
                return EXIT_FAILURE;
            }
            assist3 = searchIdent(name, dest_root);

            switch( array[i].type_token )
            {
                case 10:    // =
                    new_instr(dest, iASSIGN, &assist2, NULL, &assist1, NULL);
                    break;
                case 11:    // -
                    new_instr(dest, iMINUS, &assist1, &assist2, &assist3, NULL);
                    break;
                case 12:    // *
                    new_instr(dest, iMUL, &assist1, &assist2, &assist3, NULL);
                    break;
                case 13:    // /
                    if( assist2->data.c_number == 0 || assist2->data.d_number == 0.0 )
                    {
                        printERR(eEXPR);        // napsat hlaseni pro deleni nulou !!
                        eCode = sSynZero;
                        return EXIT_FAILURE;
                    }
                    new_instr(dest, iDIV, &assist1, &assist2, &assist3, NULL);
                    break;
                case 14:    // +
                    new_instr(dest, iPLUS, &assist1, &assist2, &assist3, NULL);
                    break;
                case 15:    // .
                    new_instr(dest, iKONK, &assist1, &assist2, &assist3, NULL);
                    break;
                case 16:    // ===
                    new_instr(dest, iEQ, &assist1, &assist2, NULL, NULL);
                    break;
                case 17:    // !==
                    new_instr(dest, iNEQ, &assist1, &assist2, NULL, NULL);
                    break;
                case 18:    // >
                    new_instr(dest, iHIGH, &assist1, &assist2, NULL, NULL);
                    break;
                case 19:    // >=
                    new_instr(dest, iHEQ, &assist1, &assist2, NULL, NULL);
                    break;
                case 20:    // <
                    new_instr(dest, iLOW, &assist1, &assist2, NULL, NULL);
                    break;
                case 21:    // <=
                    new_instr(dest, iLEQ, &assist1, &assist2, NULL, NULL);
            }

            if( type != 50 )
            {
                if( aux != NULL )
                {
                    aux->jump = dest->last;
                    aux = NULL;
                }

                if( type == 1 || (type >= 3 && type < 5) || ( type == 5 && array[i].type_token != 10 ) )     // if, elseif, while, for
                {
                    PUSHInstr( &InstrStack, dest->last, type );
                    type = 0;
                }
                else if( !SEmptyInstr( &InstrStack ) )
                {
                    TOPInstr( &InstrStack, &top );
                    while( top == 43 )
                    {
                        POPInstr( &InstrStack, &aux, &top );
                        aux->jump = dest->last;
                        aux = NULL;
                        if( !SEmptyInstr( &InstrStack ) )
                            TOPInstr( &InstrStack, &top );
                        else
                            break;
                    }
                }
            }
            else
            {
                // zpracovavame treti cast hlavicky cyklu FOR
                // instrukce se pridaji az na konec tohoto cyklu
                PUSH_last(first, &InstrFor);
                first = false;
            }

            if( array[i].type_token >= 11 && array[i].type_token <= 15 )    // ulozeni mezivysledku na zasobnik
                PUSHNode( &nodeStack, assist3);
        }

        i++; assist1 = NULL; assist2 = NULL; assist3 = NULL;    // smazat nulovani assist* - rychlost
    }

    if( !SEmptyNode( &nodeStack ) )
    {
        // odchytavani jednoprvkovych vyrazu
        assist1 = NULL;
        TOPPOPNode(&nodeStack, &assist1);
        switch(type)
        {
            case 1:     // if
            case 3:     // elseif
            case 4:     // while
            case 5:     // for

                token_init(&unit);
                unit.type_token = 37;
                name = makeName(unit);
                if( name == NULL )
                    return EXIT_FAILURE;
                insertVarToTree(name, unit, dest_root);
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist2 = searchIdent(name, dest_root);
                new_instr(dest, iBVAL, &assist1, NULL, &assist2, NULL);

                if( aux != NULL )
                {
                    aux->jump = dest->last;
                    aux = NULL;
                }

                PUSHInstr( &InstrStack, dest->last, type );
                type = 0;

                token_init(&unit);
                unit.type_token = 33;   // bool
                unit.boolean = 1;
                insertVarToTree("true", unit, dest_root);
                if( eCode != sOK )
                {
                    printERR(eINTERN);
                    return EXIT_FAILURE;
                }
                assist3 = searchIdent("true", dest_root);

                new_instr(dest, iEQ, &assist2, &assist3, NULL, NULL);
                break;

            case 7:     // return

                new_instr(dest, iRETURN, &assist1, NULL, NULL, NULL);
                if( aux != NULL )
                {
                    aux->jump = dest->last;
                    aux = NULL;
                }
                if( !SEmptyInstr( &InstrStack ) )
                {
                    TOPInstr( &InstrStack, &top );
                    while( top == 43 )
                    {
                        POPInstr( &InstrStack, &aux, &top );
                        aux->jump = dest->last;
                        aux = NULL;
                        if( !SEmptyInstr( &InstrStack ) )
                            TOPInstr( &InstrStack, &top );
                        else
                            break;
                    }
                }
        }
    }

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
        {
            if( (name = malloc((strlen("true")+1) * sizeof(char))) == NULL )
            {
                printERR(eINTERN);
                eCode = sINTERN;
                return NULL;
            }
            strcpy(name,"true");
        }
        else
        {
            if( (name = malloc((strlen("false")+1) * sizeof(char))) == NULL )
            {
                printERR(eINTERN);
                eCode = sINTERN;
                return NULL;
            }
            strcpy(name,"false");
        }
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
    {
        if( (name = malloc((strlen("null")+1) * sizeof(char))) == NULL )
        {
            printERR(eINTERN);
            eCode = sINTERN;
            return NULL;
        }
        strcpy(name,"null");
    }
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
        size--;
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
        size--;
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
    else
    {
        printERR(eINTERN);
        eCode = sINTERN;
    }

    return name;
}

// funkce pro ukladani posledni instrukce v listu do zasobniku pro FOR cyklus
// funkce musi podsledni instrukci z listu vyjmout a zrusit veskere ukazatele
void PUSH_last(bool first, tSInstr *Stack)
{
    LIST_3AK *dest = &list;
    if( func != NULL )
        dest = func->body;

    INSTRUCT a = NULL, b = dest->last->left;

    if( first )     // prvni instrukce, nutno vytvorit dalsi seznam v zasobniku
    {
        PUSHInstr( Stack, dest->last, 0 );      // seznam zacina instrukci
        dest->last->left = NULL;
    }
    else
    {
        a = Stack->Last->Instr;
        while( a->right != NULL )       // hledame posledni instrukci v seznamu
            a = a->right;

        a->right = dest->last;
        dest->last->left = a;
    }

    b->right = NULL;
    dest->last = b;     // smazani posledni instrukce z listu
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

// funkce pro inicializaci pole tokenu
int initialize_array(TOKEN**array, int i, int m)
{
	if( i == 0 && (*array = (TOKEN*) malloc(m*sizeof(TOKEN))) == NULL )
	{
		printERR(eINTERN);
		eCode = sINTERN;
		return EXIT_FAILURE;
	}

    for(int x=0; x<m; x++)
    {
        token_init(&((*array)[x]));
    }
	return EXIT_SUCCESS;
}

// funkce pro realokaci pole
int realloc_array(TOKEN*array, int*m)
{
	TOKEN *aux = NULL;
	if( (aux = realloc(array,(*m)*2*sizeof(TOKEN))) == NULL )
	{
		free(array);
		printERR(eINTERN);
		eCode = sINTERN;
		return EXIT_FAILURE;
	}
	array = aux;

	for(int x=(*m); x<(*m)*2; x++)
	{
		token_init(&array[x]);
	}
	(*m)*=2;

	return EXIT_SUCCESS;
}

void correct_list(LIST_3AK *check)
{
    if( check != NULL )
    {
        INSTRUCT aux, aux2;
        NODE assist1 = NULL;
        int top;
        aux = check->first;
        while( aux != NULL )    // prochazeni celym listem
        {
            top = 1;
            if( aux->id == iSAVE_PAR )  // nalezeni instrukce prirazeni hodnot parametrum
            {
                aux2 = aux->right;
                while( aux2 != NULL && aux2->id != iASSIGN )    // cyklus pro kontrolu, ke ktere funkci instrukce nalezi
                {
                    aux2 = aux2->right;
                }
                
                if( aux2 != NULL && aux2->id == iASSIGN && aux2->operand_1 != NULL &&
                    strcmp(aux2->operand_1->key,func->data.id_name) == 0)
                {
                    // nasli jsme aktualni funkci
                    while( aux->id == iSAVE_PAR )      // )
                    {
                        if( (assist1 = searchParam(top++, &(func->params))) != NULL )
                        {
                            aux->result = assist1;
                        }
                        else
                        {
                            aux->result = NULL;
                        }
                        aux = aux->right;
                    }
                }
                else
                    aux = aux2;     // nenasli jsme aktualni funkci, hledani pokracuje
            }
            else
                // nenasli jsme instrukci typu iSAVE_PAR
                aux = aux->right;
        }
    }
}
