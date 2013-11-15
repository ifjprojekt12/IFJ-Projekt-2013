#include "syntax.h"

// hlavni funkce syntaxe
int syntaxer()
{
    // promenna do ktere ulozime token
    TOKEN unit;
    unit = get_token();

    // vytvoreni statickeho pole tokenu
    TOKEN *array=NULL;
	m = 64;			// pocatecni velikost pole
	i = 0;			// index
    initialize_array(&array);

    // vytvoreni zasobniku a jeho inicializace
    tStack leStack;
    init(&leStack);

    // inicializace tabulky promennych
    treeInit( &root );
    treeInit( &tree );

    // vytvoreni instrukcniho listu
    new_instr_list( &list );

    // inicializace pomocnych ukazatelu na instrukci, zasobniku, stromu pro funkce
    aux = NULL;
    func = NULL;
    func_end = false;
    initInstr( &InstrStack );
    initInstr( &InstrFor );
    treeInit( &check_func );
 
    // pomocne promenne
    int type = 0;
    int top = 0;
    int super_brackets = 0;
    bool inFOR = false;
    /* -------------------------------- */

    // cyklus zajistujici nacitani tokenu
    while(unit.type_token > 0 && unit.type_token != 100 )    // konec pri chybe nebo EOF
    {
        // pole pro ukladani tokenu celeho jednoho radku
        array[i++] = unit;
        if( i == m && realloc_array(array) == EXIT_FAILURE )
			return EXIT_FAILURE;

        // if, elseif, while
        if( unit.type_token == 1 || unit.type_token == 3 || unit.type_token == 4 )
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;

            unit = get_token();
            array[i++] = unit;
    	    if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

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
                    return EXIT_FAILURE;
                }

                unit = get_token();
                array[i++] = unit;
    		    if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                if( unit.type_token != 42 ) // {
                {
                    // chybejici znak {
                    printERR(eSBRACKETO);
                    eCode = sSyn;
                    return EXIT_FAILURE;
                }

                // promenna pro kontrolu poctu slozenych zavorek
                super_brackets++;
            }
            else
            {
                // chybejici znak (
                printERR(eBRACKETO);
                eCode = sSyn;
                return EXIT_FAILURE;
            }
        }
        else if( unit.type_token == 5 )   // for
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
	        if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

            if( unit.type_token == 40 ) // (
            {
                // nacteni dalsiho tokenu - prvni cast hlavicky
                unit = get_token();
                array[i++] = unit;
    		    if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                // prvni cast je neprazdna
                if( unit.type_token == 36 )     // promenna
                {
                    unit = get_token();
                    array[i++] = unit;
	    	    	if( i == m && realloc_array(array) == EXIT_FAILURE )
						return EXIT_FAILURE;

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
                            return EXIT_FAILURE;
                        }                
                    }
                    else
                    {
                        // chybi znamenko '='
                        printERR(eWRONG);
                        eCode = sSyn;
                        return EXIT_FAILURE;
                    }
                }
                else if( unit.type_token != 22 )     // pokud je prvni cast prazdna, musi nasledovat ';'
                {
                    // chybi znamenko ';'
                    printERR(eWRONG);
                    eCode = sSyn;
                    return EXIT_FAILURE;
                }

                // nacteni dalsiho tokenu - druha cast hlavicky
                unit = get_token();
                array[i++] = unit;
		        if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                // druha cast je neprazdna
                if( (unit.type_token >= 30 && unit.type_token <= 34) || unit.type_token == 36 )
                    // string, int, double, bool, null nebo promenna
                {
                    // zpracovani vyrazu, END_S = vyraz konci strednikem
                    if( (i = expression(array, i-1, unit, END_S)) < 0 )
                    {
                        // chyba ve vyrazu
                        printERR(eEXPR);
                        if(i == -1)
                        eCode = sSyn;
                        else
                        eCode = sINTERN;
                        return EXIT_FAILURE;
                     }                

                }
                else if( unit.type_token != 22 )     // pokud je druha cast prazdna, musi nasledovat ';'
                {
                    // chybi znamenko ';'
                    printERR(eWRONG);
                    eCode = sSyn;
                    return EXIT_FAILURE;
                }

                // nacteni dalsiho tokenu - treti cast hlavicky
                unit = get_token();
                array[i++] = unit;
        		if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;
               
                if( unit.type_token == 36 )     // promenna
                {
                    unit = get_token();
                    array[i++] = unit;
        			if( i == m && realloc_array(array) == EXIT_FAILURE )
						return EXIT_FAILURE;

                    if( unit.type_token == 10 )     // =
                    {
                        // zpracovani vyrazu, END_B = vyraz konci zavorkou
                        if( (i = expression(array, i, get_token(), END_B)) < 0 )
                        {
                            // chyba ve vyrazu
                            printERR(eEXPR);
                            if(i == -1)
                                eCode = sSyn;
                            else
                                eCode = sINTERN;
                            return EXIT_FAILURE;
                        }                
                    }
                    else
                    {
                        // chybejici znak '='
                        printERR(eWRONG);
                        eCode = sSyn;
                        return EXIT_FAILURE;
                    }

                }
                else if( unit.type_token != 41 )    // pokud je treti cast prazdna, musi nasledovat ')'
                {
                    // chybi znamenko ';'
                    printERR(eWRONG);
                    eCode = sSyn;
                    return EXIT_FAILURE;
                }

                unit = get_token();
                array[i++] = unit;
		        if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                if( unit.type_token != 42 )     // {
                {
                    // chybejici znak '{'
                    printERR(eSBRACKETO);
                    eCode = sSyn;
                    return EXIT_FAILURE;
                }

                // promenna pro kontrolu poctu slozenych zavorek
                super_brackets++;
            }
            else
            {
                // chybejici znak '('
                printERR(eBRACKETO);
                eCode = sSyn;
                return EXIT_FAILURE;
            }
            inFOR = true;
        }
        else if( unit.type_token == 8 || unit.type_token == 9 )     // break nebo continue
        {
            if( !inFOR )
            {
                // prikaz break nebo continue se nenachazi uvnitr prikazu FOR
                printERR(eFOR);
                eCode = sSyn;
                return EXIT_FAILURE;
            }

            unit = get_token();
            array[i++] = unit;
	        if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

            if( unit.type_token != 22 )     // ;
            {
                // chybejici znak ';'
                printERR(eWRONG);
                eCode = sSyn;
                return EXIT_FAILURE;
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
                return EXIT_FAILURE;
            }
        }
        else if( unit.type_token == 2 )   // else
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
    	    if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

            if( unit.type_token != 42 ) // {
            {
                // chybejici znak {
                printERR(eSBRACKETO);
                eCode = sSyn;
                return EXIT_FAILURE;
            }

            // promenna pro kontrolu poctu slozenych zavorek
            super_brackets++;
        }
        else if( unit.type_token == 36 )    // promenna
        {
            unit = get_token();
            array[i++] = unit;
    	    if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

            if( unit.type_token == 10 )     // =
            {
                unit = get_token();
                array[i++] = unit;
		        if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                if((unit.type_token >= 60 && unit.type_token <= 69) || unit.type_token == 35)    // vestavene nebo uzivatelem definovane funkce
                {
                    unit = get_token();
                    array[i++] = unit;
			        if( i == m && realloc_array(array) == EXIT_FAILURE )
						return EXIT_FAILURE;
                   
                    if( unit.type_token == 40 )     // (
                    {
                        unit = get_token(); 
                        array[i++] = unit;
				        if( i == m && realloc_array(array) == EXIT_FAILURE )
							return EXIT_FAILURE;

                        bool empty = true;  // seznam parametru je prazdny
                        bool term = true;
                        // kontrola posloupnosti zapisu parametru funkci
                        while( unit.type_token != 41 )  // )
                        {
                            // seznam parametru neni prazdny
                            if( empty )
                                empty = false;

                            if( term && ((unit.type_token >= 30 && unit.type_token <= 34) || unit.type_token == 36) )
                            // ocekavame term && prijde term
                                term = false;
                            else if( !term && unit.type_token == 23 )     // neocekavame promennou && prijde ','
                                term = true;
                            else
                            {
                                // nespravny token ci posloupnost
                                eCode = sSyn;
                                printERR(ePARAM);
                                return EXIT_FAILURE;
                            }

                            unit = get_token();
                            array[i++] = unit;
      						if( i == m && realloc_array(array) == EXIT_FAILURE )
								return EXIT_FAILURE;
                        }
                        if( term && !empty )
                        {
                            // spatna posloupnost argumentu funkce
                            printERR(ePARAM);
                            eCode = sSyn;
                            return EXIT_FAILURE;
                        }

                        unit = get_token();
                        array[i++] = unit;
				        if( i == m && realloc_array(array) == EXIT_FAILURE )
							return EXIT_FAILURE;

                        if( unit.type_token != 22 ) // ;
                        {
                            // chybejici znak ';'
                            printERR(eWRONG);
                            eCode = sSyn;
                            return EXIT_FAILURE;
                        }
                    }
                    else
                    {
                        // chybejici znak '('
                        printERR(eWRONG);
                        eCode = sSyn;
                        return EXIT_FAILURE;
                    }
                }
                else if( (i = expression(array, i-1, unit, END_S)) < 0 )    // vyraz
                // zpracovani vyrazu, END_S = vyraz konci strednikem
                {
/*                    printf("tady\n");

    printf("$x = funkce()\n");
    // vypis pole tokenu
    for(int x=0; x<m; x++)
    {
        if(array[x].type_token == 0)    break;
        printf("%d ", array[x].type_token);
    }
    printf("\n");
*/
                    // chyba ve vyrazu
                    printERR(eEXPR);
                    if(i == -1)
                        eCode = sSyn;
                    else
                        eCode = sINTERN;
                    return EXIT_FAILURE;
                }
            }
            else
            {
                // chybejici znak =
                printERR(eASSIGN);
                eCode = sSyn;
                return EXIT_FAILURE;
            }
        }
        else if( unit.type_token == 6 )     // function
        {
            // ulozeni typu prvniho tokenu pro zjisteni spravneho poradi if-elseif-else
            type = unit.type_token;
            unit = get_token();
            array[i++] = unit;
		    if( i == m && realloc_array(array) == EXIT_FAILURE )
				return EXIT_FAILURE;

            if( unit.type_token == 35 )     // id
            {
                unit = get_token();
                array[i++] = unit;
		        if( i == m && realloc_array(array) == EXIT_FAILURE )
					return EXIT_FAILURE;

                if( unit.type_token == 40 )     // (
                {
                    unit = get_token(); 
                    array[i++] = unit;
				    if( i == m && realloc_array(array) == EXIT_FAILURE )
						return EXIT_FAILURE;
                    
                    bool empty = true;  // seznam parametru je prazdny
                    bool id = true;
                    // kontrola posloupnosti zapisu parametru funkci
                    while( unit.type_token != 41 )  // )
                    {
                        // seznam parametru neni prazdny
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
                            printERR(ePARAM);
                            return EXIT_FAILURE;
                        }

                        unit = get_token();
                        array[i++] = unit;
				        if( i == m && realloc_array(array) == EXIT_FAILURE )
							return EXIT_FAILURE;
                    }
                    if( id && !empty )
                    {
                        // spatna posloupnost parametru funkce
                        printERR(ePARAM);
                        eCode = sSyn;
                        return EXIT_FAILURE;
                    }
                    
                    unit = get_token();
                    array[i++] = unit;
			        if( i == m && realloc_array(array) == EXIT_FAILURE )
						return EXIT_FAILURE;

                    if( unit.type_token != 42 ) // {
                    {
                        // chybejici znak {
                        printERR(eSBRACKETO);
                        eCode = sSyn;
                        return EXIT_FAILURE;
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
                return EXIT_FAILURE;
            }

            if( SEmpty( &leStack ) )
            {
                // zavorka s prazdnym zasobnikem
                printERR(eWRONG);
                eCode = sSyn;
                return EXIT_FAILURE;
            }

            // vyjmeme ze zasobniku posledni otevreny prikaz
            TOP( &leStack, &top );
            if( top == cREADY )
            {
                // pokud jsme nacli cREADY, nacteme ze zasobniku jeste jednou
                POP( &leStack );
                TOP( &leStack, &top );
            }

            // smazeme ze zasobniku nacteny prikaz
            POP( &leStack);
            if( top == cIF || top == cELSEIF )
                // pokud byl tento prikaz IF nebo ELSEIF, ulozime do zasobniku cREADY - umiznime prikaz ELSE
                PUSH( &leStack, cREADY );
            else if( top == cFUNCTION )
                func_end = true;
            else if( top == cFOR )
            {
                inFOR = false;
                if( !SEmpty( &leStack ) )
                {
                    tElemPtr aux = leStack.Last;
                    // kontrola, zda se stale nachazime v otevrenem cyklu for - prochazime cely zasobnik
                    while( aux != NULL )
                    {
                        if( aux->Elem == cFOR )
                        {
                            inFOR = true;
                            break;
                        }
                        aux = aux->ptr;
                    }
                }
            }
        }
        else if( unit.type_token == 50 )        // EOF
        {
            type = unit.type_token;
            semantixer(array);
            break;
        }
        else
        {
            //printf("token: %d\n", unit.type_token);
            // token, ktery se nesmi nalezat na zacatku radku
            printERR(eUNKNOWN);
            eCode = sSyn;
            return EXIT_FAILURE;
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
                        return EXIT_FAILURE;
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
                        return EXIT_FAILURE;
                    }
        }
        type = 0;

/*
        // vypis celeho zasobniku typu int
        if( !SEmpty( &leStack ) )
        {
            printf("Zasobnik int:\ntop: ");
            tElemPtr aux = leStack.Last;
            while( aux != NULL )
            {
                printf("| %d ", aux->Elem);
                aux = aux->ptr;
            }
            printf("|\n");
        }
        else
            printf("Zasobnik je prazdny.\n");
*/
/*
    // vypis pole tokenu
    for(int x=0; x<I_MAX; x++)
    {
        if(array[x].type_token == 0)    break;
        printf("%d ", array[x].type_token);
    }
    printf("\n");
*/

        // po zpracovani jednoho radku volame funkci pro kontrolu semantiky
        if( semantixer(array) == EXIT_FAILURE )
            return EXIT_FAILURE;

        // po zpracovani jednoho radku si pripravime pole pro radek novy
        initialize_array(&array);
        i = 0;
    
        unit = get_token(); // nacteni dalsiho tokenu
    }

    if( unit.type_token == 0 || unit.type_token == 100 )
    {
        printERR(eWRONG);
        if( eCode == 0 )
            eCode = sSyn;
        return EXIT_FAILURE;
    }
    else if( super_brackets != 0 )
    {
        // vice '{' nez '}' v celem programu
        printERR(eWRONG);
        eCode = sSyn;
        return EXIT_FAILURE;
    }
    else if( func_defined(check_func, NULL) != EXIT_SUCCESS )
    {
        // kontrola, zda vsechny volane funkce byly definovany
        printERR(eFCENEDEF);
        eCode = sSemFceDef;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int func_defined(NODE a1, NODE a2)
{
    NODE ass;
    if( a1 != NULL )
    {
        ass = searchIdent(a1->key, &tree);
        if( ass == NULL )
            // nektera z volanych funkci nebyla v programu definovana
            return EXIT_FAILURE;
        if( func_defined(a1->LPtr, a1->RPtr) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    if( a2 != NULL )
    {
        ass = searchIdent(a2->key, &tree);
        if( ass == NULL )
            // nektera z volanych funkci nebyla v programu definovana
            return EXIT_FAILURE;
        if( func_defined(a2->LPtr, a2->RPtr) == EXIT_FAILURE )
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// funkce pro inicializaci pole tokenu
int initialize_array(TOKEN**array)
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

int realloc_array(TOKEN*array)
{
	TOKEN *aux = NULL;
	if( (aux = realloc(array,m*2*sizeof(TOKEN))) == NULL )
	{
		free(array);
		printERR(eINTERN);
		eCode = sINTERN;
		return EXIT_FAILURE;
	}
	array = aux;

	for(int x=m; x<m*2; x++)
	{
		token_init(&array[x]);
	}
	m*=2;

	return EXIT_SUCCESS;
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
        if( i == m && realloc_array(array) == EXIT_FAILURE )
			return -2;

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
