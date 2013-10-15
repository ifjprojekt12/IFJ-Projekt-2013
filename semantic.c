#include "semantic.h"

// precedencni tabulka
int table[MAXINDEX][MAXINDEX]={
    //              term    +   -   *   /   <   >   === <=  >=  !== .   (   )
    /* 1. term */ { H,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H },
    /* 2. + */    { L,      H,  H,  L,  L,  H,  H,  H,  H,  H,  H,  E,  L,  H },
    /* 3. - */    { L,      H,  H,  L,  L,  H,  H,  H,  H,  H,  H,  E,  L,  H },
    /* 4. * */    { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  E,  L,  H },
    /* 5. / */    { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  E,  L,  H },
    /* 6. < */    { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H },
    /* 7. > */    { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H },
    /* 8. === */  { L,      L,  L,  L,  L,  L,  L,  H,  L,  L,  H,  L,  L,  H },
    /* 9. <= */   { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H },
    /* 10. >= */  { L,      L,  L,  L,  L,  H,  H,  H,  H,  H,  H,  L,  L,  H },
    /* 11. !== */ { L,      L,  L,  L,  L,  L,  L,  H,  L,  L,  H,  L,  L,  H },
    /* 12. . */   { L,      E,  E,  E,  E,  H,  H,  H,  H,  H,  H,  H,  L,  H },
    /* 13. ( */   { L,      L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L,  L },
    /* 14. ) */   { L,      H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H,  H },
};

// hlavni funkce semantiky
int semantixer(TOKEN *array)
{
    printf("semantixer\n");

    int n=0;
    if( array[n].type_token == 36 )
        n+=2;

    if( expression_sem(array, n) == EXIT_FAILURE )
        return EXIT_FAILURE;
    printf("semantixer\n");

    return EXIT_SUCCESS;
}

// funkce pro zapis vyrazu do postfixove notace a odeslani instrukci
int expression_sem(TOKEN *array, int n)
{
    printf("expression_sem\n");

    // deklarace zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);

    // pomocne pole tokenu (zatim velikost I_MAX -> udelat dynamicky nejspis) TODO
    TOKEN array_expr[N_MAX];
    int i, new = 0, old = 0, precedent = 0;

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
    while( array[n].type_token != 22 )      // ;
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
                    {
                        PUSH( &leStack, array[n++].type_token);
                        break;
                    }
                    else
                    {
                        array_expr[i++].type_token = TOPCheck( &leStack );
                        POP( &leStack );
                        break;
                    }
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
                fprintf(stderr, "switch default\n");
                return EXIT_FAILURE;
            }
        }
    }

    while( !SEmpty( &leStack ) )
    {
        array_expr[i++].type_token = TOPCheck( &leStack );
        POP( &leStack );
    }

    for( i=0; i<N_MAX; i++)
    {
        if( array_expr[i].type_token == 0 )
            break;
        printf("%d ", array_expr[i].type_token);
    }
    printf("\n");

    return EXIT_SUCCESS;
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
        default:    break;
    }
    return -1;
}
