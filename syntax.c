#include "syntax.h"

const char*MESSAGE[]=
{
	"Dosazena maximalni velikost pole tokenu",
	"Chybna syntaxe struktury programu",
	"Nespravne zapsany vyraz",
};

int syntaxer()
{
    // promenna do ktere ulozime token
    TOKEN unit;
    unit = get_token();
    // vytvoreni pole tokenu a jeho inicializace
    TOKEN array[I_MAX];
    initialize_array(array);

    // vytvoreni zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);
   
    // pomocne promenne
    int type = 0;
    int top = 0;
    int i = 0;
    int brackets = 0;
    int super_brackets = 0;
    int eCode = sOK;
    /* -------------------------------- */
    
    while(unit.type_token > 0 && unit.type_token != 50 )    // konec pri chybe nebo EOF
    {
        array[i++] = unit;
        if( i == I_MAX )
        {
            printERR(eIMAX);
            eCode = sINTERN;
            break;
        }

        // if, elseif, while
        if( unit.type_token == 1 || unit.type_token == 3 || unit.type_token == 4 )
        {
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
                if( (i = expression(array, i, get_token(), END_B)) < 0 )
                {
                    // chyba ve vyrazu
                    printERR(eEXPR);
                    if(i == -1)
                    	eCode = sSyn;
                    else
                    	ecode = sINTERN;
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

                if( unit.type_token == 42 ) // {
                {
                    super_brackets++;
                }
                else
                {
                    // chybejici znak {
                    printERR(eWRONG);
                    eCode = sSyn;
                    break;
                }
            }
            else
            {
                // chybejici znak (
                printERR(eWRONG);
                eCode = sSyn;
                break;
            }
        }
        else if( unit.type_token == 7 )     // return
        {
            if( (i = expression(array, i, get_token(), END_S)) < 0)
            {
                // chyba ve vyrazu
                printERR(eEXPR);
                if(i == -1)
                	eCode = sSyn;
                else
                    	ecode = sINTERN;
                break;
            }
        }
        else if( unit.type_token == 2 )   // else
        {
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printERR(eIMAX);
                eCode = sINTERN;
                break;
            }

            if( unit.type_token == 42 ) // {
            {
                super_brackets++;
            }
            else
            {
                // chybejici znak {
                printERR(eWRONG);
                eCode = sSyn;
                break;
            }
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
                if( (i = expression(array, i, get_token(), END_S)) < 0 )
                {
                    // chyba ve vyrazu
                    printERR(eEXPR);
                    if(i == -1)
                    	eCode = sSyn;
                    else
                    	ecode = sINTERN;
                    break;
                }
            }
            else
            {
                // chybejici znak =
                printERR(eWRONG);
                eCode = sSyn;
                break;
            }
        }
        else if( unit.type_token == 6 )     // function
        {
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
                    
                    bool id = true;
                    bool err = false;
                    // kontrola posloupnosti zapisu parametru funkci
                    while( unit.type_token != 41 )  // )
                    {
                        if( id && unit.type_token == 36 )
                            id = false;
                        else if( !id && unit.type_token == 23 )     // ,
                            id = true;
                        else
                        {
                            // nespravny token ci posloupnost
                            printERR(eWRONG);
                            eCode = sSyn;
                            err = true;
                            break;
                        }

                        unit = get_token();
                        array[i++] = unit;
                        if( i == I_MAX )
                        {
                            printERR(eIMAX);
                            eCode = sINTERN;
                            err = true;
                            break;
                        }
                    }
                    if( id || err )
                    {
                        // spatna posloupnost parametru funkce nebo prokroceno I_MAX
                        printERR(eWRONG);
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

                    if( unit.type_token == 42 ) // {
                    {
                        super_brackets++;
                    }
                    else
                    {
                        // chybejici znak {
                        printERR(eWRONG);
                        eCode = sSyn;
                        break;
                    }
                }
            }
        }
        else if( unit.type_token == 43 )    // }
        {
            super_brackets--;
            if( super_brackets < 0 )
            {
                // vice } nez { zavorek
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
            switch(top) {
                case cWHILE:
                case cFOR:
                case cFUNCTION:
                case cELSE:     POP( &leStack );
                                break;
                case cIF:
                case cELSEIF:   POP( &leStack );
                                PUSH( &leStack, cREADY );
            }
        }
        else
        {
            // token, ktery se nesmi nalezat na zacatku radku
            printERR(eWRONG);
            eCode = sSyn;
            break;
        }

        // ukladani na zasobnik
        switch(type) {
            case 1: // if 
            case 4: // while
            case 5: // for
            case 6:
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

            case 2: // else
            case 3: // elseif
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
    
        unit = get_token(); // nacteni dalsiho tokenu
    }
    array[i] = unit;

    if( brackets != 0 && super_brackets != 0 )
    {
        // vice { nez } v celem programu
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
//    bool isFirst = true;
    bool wasExpr = false;
    bool wasSign = true;
    int brackets = 0;

    while( unit.type_token > 0 && unit.type_token != 50 && unit.type_token != 100 ) // nic, EOF, chyba
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
                    // chyba, ted nevim presne jaka
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
