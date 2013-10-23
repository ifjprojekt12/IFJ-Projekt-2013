#include "syntax.h"

// tabulka pro vypis chyboveho hlaseni
const char*MESSAGE[]=
{
	"Dosazena maximalni velikost pole tokenu",
	"Chybna syntaxe struktury programu",
	"Nespravne zapsany vyraz",
    "Chybi oteviraci zavorka",
    "Chybi slozena oteviraci zavorka",
    "Nespravne zapsany tvar prirazeni",
    "Spatna posloupnost parametru v definici funkce",
    "Neznamy token",
};

// hlavni funkce syntaxe
int syntaxer()
{
    // promenna do ktere ulozime token
    TOKEN unit;
    unit = get_token();

    // vytvoreni statickeho pole tokenu a jeho inicializace
    TOKEN array[I_MAX];
    initialize_array(array);

    // vytvoreni zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);

    // inicializace tabulky promennych
    treeInit( &root );

    // vytvoreni instrukcniho listu
    new_instr_list( &list );
 
    // pomocne promenne
    int type = 0;
    int top = 0;
    int i = 0;
    int super_brackets = 0;
    int eCode = sOK;
    /* -------------------------------- */
    
    // cyklus zajistujici nacitani tokenu
    while(unit.type_token > 0 && unit.type_token != 100 && unit.type_token != 50 )    // konec pri chybe nebo EOF
    {
        // pole pro ukladani tokenu celeho jednoho radku
        array[i++] = unit;
        if( i == I_MAX )
        {
            // vycerpani staticke velikosti pole
            printERR(eIMAX);
            eCode = sINTERN;
            break;
        }

        // if, elseif, while
        if( unit.type_token == 1 || unit.type_token == 3 || unit.type_token == 4 )
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;

            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printERR(eIMAX);
				eCode = sINTERN;
                break;
            }

            if( unit.type_token == 40 ) // (
            {
                // zpracovani vyrazu, ktery musi nasledovat, END_B = vyraz konci zavorkou
                if( (i = expression(array, i, get_token(), END_B)) < 0 )
                {
                    // chyba ve vyrazu
                    printERR(eEXPR);
                    if(i == -1)
						eCode = sSyn;
                    else
						eCode = sINTERN;
                    break;
                }

                unit = get_token();
                array[i++] = unit;
                if( i == I_MAX )
                {
                    printERR(eIMAX);
                    eCode = sINTERN;
                    break;
                }

                if( unit.type_token != 42 ) // {
                {
                    // chybejici znak {
                    printERR(eSBRACKETO);
                    eCode = sSyn;
                    break;
                }

                // promenna pro kontrolu poctu slozenych zavorek
                super_brackets++;
            }
            else
            {
                // chybejici znak (
                printERR(eBRACKETO);
                eCode = sSyn;
                break;
            }
        }
        else if( unit.type_token == 5 )   // for
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printERR(eIMAX);
				eCode = sINTERN;
                break;
            }

            if( unit.type_token == 40 ) // (
            {
                unit = get_token();
                array[i++] = unit;
                if( i == I_MAX )
                {
                    printERR(eIMAX);
                    eCode = sINTERN;
                    break;
                }

                if( unit.type_token == 36 )     // promenna
                {
                    unit = get_token();
                    array[i++] = unit;
                    if( i == I_MAX )
                    {
                        printERR(eIMAX);
                        eCode = sINTERN;
                        break;
                    }

                    if( unit.type_token == 10 )     // =
                    {
                        // zpracovani vyrazu, END_S = vyraz konci strednikem
                        if( (i = expression(array, i, get_token(), END_S)) < 0 )
                        {
                            // chyba ve vyrazu
                            printERR(eEXPR);
                            if(i == -1)
								eCode = sSyn;
                            else
								eCode = sINTERN;
                            break;
                        }                
                    }
                    else
                    {
                        printERR(eWRONG);
                        eCode = sSyn;
                        break;
                    }
                }

                if( unit.type_token == 22 )     // prvni ;
                {
                    
                }
            }
            else
            {
                // chybejici znak (
                printERR(eBRACKETO);
                eCode = sSyn;
                break;
            }
        }
        else if( unit.type_token == 7 )     // return
        {
            // zpracovani vyrazu, END_S = vyraz konci strednikem
            if( (i = expression(array, i, get_token(), END_S)) < 0)
            {
                // chyba ve vyrazu
                printERR(eEXPR);
                if(i == -1)
					eCode = sSyn;
                else
                    eCode = sINTERN;
                break;
            }
        }
        else if( unit.type_token == 2 )   // else
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printERR(eIMAX);
                eCode = sINTERN;
                break;
            }

            if( unit.type_token != 42 ) // {
            {
                // chybejici znak {
                printERR(eSBRACKETO);
                eCode = sSyn;
                break;
            }

            // promenna pro kontrolu poctu slozenych zavorek
            super_brackets++;
        }
        else if( unit.type_token == 36 )    // promenna
        {
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            { 
                printERR(eIMAX);
                eCode = sINTERN;
                break;
            }

            if( unit.type_token == 10 )     // =
            {
                // zpracovani vyrazu, END_S = vyraz konci strednikem
                if( (i = expression(array, i, get_token(), END_S)) < 0 )
                {
                    // chyba ve vyrazu
                    printERR(eEXPR);
                    if(i == -1)
						eCode = sSyn;
                    else
						eCode = sINTERN;
                    break;
                }
            }
            else
            {
                // chybejici znak =
                printERR(eASSIGN);
                eCode = sSyn;
                break;
            }
        }
        else if( unit.type_token == 6 )     // function
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printERR(eIMAX);
                eCode = sINTERN;
                break;
            }

            if( unit.type_token == 35 )     // id
            {
                unit = get_token();
                array[i++] = unit;
                if( i == I_MAX )
                {
                    printERR(eIMAX);
                    eCode = sINTERN;
                    break;
                }

                if( unit.type_token == 40 )     // (
                {
                    unit = get_token(); 
                    array[i++] = unit;
                    
                    bool empty = true;  // seznam parametru je prazdny
                    bool id = true;
                    bool err = false;
                    int err_type = ePARAM;
                    // kontrola posloupnosti zapisu parametru funkci
                    while( unit.type_token != 41 )  // )
                    {
                        // seznama parametru neni prazdny
                        if( empty )
                            empty = false;

                        if( id && unit.type_token == 36 )       // ocekavame promennou && prijde promenna
                            id = false;
                        else if( !id && unit.type_token == 23 )     // neocekavame promennou && prijde ','
                            id = true;
                        else
                        {
                            // nespravny token ci posloupnost
                            eCode = sSyn;
                            // priznak znacici vyskyt chyby
                            err = true;
                            err_type = ePARAM;
                            break;
                        }

                        unit = get_token();
                        array[i++] = unit;
                        if( i == I_MAX )
                        {
                            eCode = sINTERN;
                            // priznak znacici vyskyt chyby
                            err = true;
                            err_type = eIMAX;
                            break;
                        }
                    }
                    if( (id && !empty)  || err )
                    {
                        // spatna posloupnost parametru funkce nebo prokroceno I_MAX
                        printERR(err_type);
                        eCode = sSyn;
                        break;
                    }
                    
                    unit = get_token();
                    array[i++] = unit;
                    if( i == I_MAX )
                    {
                        printERR(eIMAX);
                        eCode = sINTERN;
                        break;
                    }

                    if( unit.type_token != 42 ) // {
                    {
                        // chybejici znak {
                        printERR(eSBRACKETO);
                        eCode = sSyn;
                        break;
                    }

                    // promenna pro kontrolu poctu slozenych zavorek
                    super_brackets++;
                }
            }
        }
        else if( unit.type_token == 43 )    // }
        {
            // promenna pro kontrolu poctu slozenych zavorek
            super_brackets--;
            if( super_brackets < 0 )
            {
                // vice '}' nez '{' zavorek
                printERR(eWRONG);
                eCode = sSyn;
                break;
            }

            if( SEmpty( &leStack ) )
            {
                // zavorka s prazdnym zasobnikem
                printERR(eWRONG);
                eCode = sSyn;
                break;
            }

            TOP( &leStack, &top );
            if( top == cREADY )
            {
                POP( &leStack );
                TOP( &leStack, &top );
            }

            POP( &leStack);
            if( top == cIF || top == cELSEIF )
                PUSH( &leStack, cREADY );
   
            // abychom neposilali semantice samostatny znak '}', urychlime beh programu 
            i = 0;
            initialize_array(array);
            unit = get_token();
            continue;
        }
        else
        {
            // token, ktery se nesmi nalezat na zacatku radku
            printERR(eUNKNOWN);
            eCode = sSyn;
            break;
        }

        // ukladani na zasobnik, kontrola if-elseif-else
        switch(type) {
            case 1:     // if 
            case 4:     // while
            case 5:     // for
            case 6:     // function
                    if( !SEmpty( &leStack ) )
                    {
                        TOP( &leStack, &top );
                        if( top == cREADY )
                            POP( &leStack );
                    }
                    if( type == 1 )
                        PUSH( &leStack, cIF );
                    else if( type == 4 )
                        PUSH( &leStack, cWHILE);
                    else if( type == 6 )
                        PUSH( &leStack, cFUNCTION );
                    else
                        PUSH( &leStack, cFOR);
                    break;

            case 2:     // else
            case 3:     // elseif
                    if( SEmpty( &leStack ) )
                    {
                        // prazdny zasobnik ( == else(if) bez IF ?)
                        printERR(eWRONG);
                        eCode = sSyn;
                        break;
                    }
                    TOP( &leStack, &top );
                    if( top == cREADY )
                    {
                        POP( &leStack );
                        if( type == 2 )
                            PUSH( &leStack, cELSE );
                        else
                            PUSH( &leStack, cELSEIF );
                    }
                    else
                    {
                        // else(if) bez IF
                        printERR(eWRONG);
                        eCode = sSyn;
                        break;
                    }
        }
        type = 0;

        // po zpracovani jednoho radku volame funkci pro kontrolu semantiky
        if( semantixer(array) == EXIT_FAILURE )
            return EXIT_FAILURE;

        // snad to nezpusobi chybu, ale novy radek -> nove pole TODO
    	// plus mazat pole!!
        i = 0;
        initialize_array(array);
    
        unit = get_token(); // nacteni dalsiho tokenu
    }
    // array[i] = unit;     // ukladame do pole i tokeny 0, 50, 100 ?? proc ?? TODO

    if( super_brackets != 0 )
    {
        // vice '{' nez '}' v celem programu
        printERR(eWRONG);
        eCode = sSyn;
    }

/*
    // vypis pole tokenu
    for(int x=0; x<I_MAX; x++)
    {
        if(array[x].type_token == 0)    break;
        printf("%d ", array[x].type_token);
    }
    printf("\n");
*/

    if( eCode == sOK )
        printf("vse ok!\n");
    return eCode;
}

// funkce pro inicializaci pole tokenu
void initialize_array(TOKEN*array)
{
    for(int x=0; x<512; x++)
    {
        token_init(&array[x]);
    }
}

// funkce pro kontrolu vyrazu
int expression(TOKEN*array, int i, TOKEN unit, int ending)
{
    // definice a deklarace pomocnych promennych
    bool wasExpr = false;
    bool wasSign = true;
    int brackets = 0;

    while( unit.type_token > 0 && unit.type_token != 100 && unit.type_token != 50 ) // chyba nebo EOF
    {
        array[i++] = unit;
        if( i == I_MAX )
        {
            printERR(eIMAX);
            return -2;
        }

        if( wasSign && unit.type_token == 40 )  // (
        {
            brackets++;
        }
        else if( wasSign && unit.type_token >= 30 && unit.type_token <= 36 )    // string, int, double, boolean, null, id, $id
        {
            wasExpr = true;
            wasSign = false;
        }
        else if( wasExpr && unit.type_token > 10 && unit.type_token <= 21 )     // -, *, /, +, >, <, >=, <=, ===, !==, .
        {
            wasExpr = false;
            wasSign = true;
        }
        else if( wasExpr && unit.type_token == 41 )    // )
        {
            brackets--;
            if( brackets < 0 )
            {
                if( ending != END_S )
                    break;
                else
                {
                    // prebytecna uzaviraci zavorka, cekame strednik (semicolon)
                    printERR(eEXPR);
                    return -1;
                }
            }
        }
        else if( ending == END_S && wasExpr && unit.type_token == 22 )     // ;
        {
            if( brackets != 0 )
            {
                // chybi zavorka ve vyrazu
                printERR(eEXPR);
                return -1;
            }
            break;
        }
        else
        {
            // znak na nespravnem miste
            printERR(eEXPR);
            return -1;
        }

        unit = get_token();
    }

    return i;
}

// vypis chyboveho hlaseni
void printERR(int err)
{
    fprintf(stderr, "Chyba: %s.\n", MESSAGE[err]);
}
