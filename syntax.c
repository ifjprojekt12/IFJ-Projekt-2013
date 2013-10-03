#include "syntax.h"

int syntaxer()
{
    //pomocna promenna
    TOKEN unit;
    // vytvoreni pole tokenu a jeho inicializace
    TOKEN array[I_MAX];
    initialize_array(array);

    unit = get_token();
    int i = 0;
    int brackets = 0;
    int super_brackets = 0;
    while(unit.type_token > 0 && unit.type_token != 50 )    // konec pri chybe nebo EOF
    {
        array[i++] = unit;
        if( i == I_MAX )
        {
            printf("Dosazena maximalni velikost pole tokenu.\n");
            break;
        }

        // if, elseif, while
        if( unit.type_token == 1 || unit.type_token == 3 || unit.type_token == 4 )
        {
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                printf("Dosazena maximalni velikost pole tokenu.\n");
                break;
            }

            if( unit.type_token == 40 ) // (
            {
                if( (i = expression(array, i, get_token(), END_B)) < 0 )
                {
                    printf("chyba\n");
                    break;
                    // TODO chyba
                }
                unit = get_token();
                array[i++] = unit;
                if( i == I_MAX )
                {
                    printf("Dosazena maximalni velikost pole tokenu.\n");
                    break;
                }

                if( unit.type_token == 42 ) // {
                {
                    super_brackets++;
                }
                else
                {
                    // TODO chyba
                    printf("Chybi slozena oteviraci zavorka.\n");
                    break;
                }
            }
            else
            {
                // TODO chyba
                printf("chyba\n");
                break;
            }
        }
        else if( unit.type_token == 7 )     // return
        {
            if( (i = expression(array, i, get_token(), END_S)) < 0)
            {
                printf("chyba\n");
                break;
                // TODO chyba
            }
        }
        else if( unit.type_token == 2 )   // else
        {
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            {
                // TODO chyba
                printf("Dosazena maximalni velikost pole tokenu.\n");
                break;
            }

            if( unit.type_token == 42 ) // {
            {
                super_brackets++;
            }
            else
            {
                // TODO chyba
                printf("Chybi slozena oteviraci zavorka.\n");
                break;
            }
        }
        else if( unit.type_token == 36 )    // promenna
        {
            unit = get_token();
            array[i++] = unit;
            if( i == I_MAX )
            { 
                printf("Dosazena maximalni velikost pole tokenu.\n");
                break;
            }

            if( unit.type_token == 10 )     // =
            {
                if( (i = expression(array, i, get_token(), END_S)) < 0 )
                {
                    // TODO chyba
                    printf("chyba\n");
                    break;
                }
            }
            else
            {
                // TODO chyba
                printf("chyba\n");
                break;
            }
        }
        else if( unit.type_token == 6 )
        else if( unit.type_token == 43 )    // }
        {
            super_brackets--;
            if( super_brackets < 0 )
            {
                printf("prilis mnoho slozenych uzaviracich zavorek\n");
                break;
                // TODO chyba
            }
        }
        else
        {
            // TODO chyba
            printf("chyba\n");
            break;
        }
    
        unit = get_token(); // nacteni dalsiho tokenu
    }
    array[i] = unit;

    if( brackets != 0 && super_brackets != 0 )
    {
        printf("nespravny pocet zavorek\n");
        // TODO chyba
    }

    // vypis pole tokenu
    for(int x=0; x<I_MAX; x++)
    {
        if(array[x].type_token == 0)    break;
        printf("%d ", array[x].type_token);
    }
    printf("\n");

    return EXIT_SUCCESS;
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
            printf("Dosazena maximalni velikost pole tokenu.\n");
            break;
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
                    // TODO chyba!!
                    printf("chyba\n");
                    return -1;
                }
            }
        }
        else if( ending == END_S && wasExpr && unit.type_token == 22 )     // ;
        {
            if( brackets != 0 )
            {
                // TODO chyba
                printf("chybi zavorka ve vyrazu.\n");
                return -1;
            }
            break;
        }
        else
        {
            // TODO chyba
            printf("Znak na nespravnem miste.\n");
            return -1;
        }

        unit = get_token();
    }

    return i;
}
