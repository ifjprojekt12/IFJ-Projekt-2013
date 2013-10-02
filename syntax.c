#include "syntax.h"

int syntaxer()
{
	//pomocna promenna
	TOKEN unit;
	// vytvoreni pole tokenu a jeho inicializace
	TOKEN array[512];	// 512 - magicke cislo, nutno prepsat (I_MAX)
	initialize_array(array);

	unit = get_token();
	int i = 0;
	int brackets = 0;
	while(unit.type_token > 0 && unit.type_token != 11 )	// konec pri chybe nebo EOF (ted pri znaku '-')
	{
		array[i++] = unit;

		// if, elseif, while, for
		if( unit.type_token == 1 || unit.type_token == 3 || unit.type_token == 4 || unit.type_token == 5 )
		{
			unit = get_token();
			array[i++] = unit;
			if( unit.type_token == 40 ) // (
			{
				if( (i = expression(array, i)) < 0 )
				{
					// TODO chyba
				}
				unit = get_token();
				array[i++] = unit;
				if( unit.type_token == 41 )	// )
				{
					unit = get_token();
					array[i++] = unit;
					if( unit.type_token == 42 )	// {
					{
						brackets++;
					}
				}
			}
		}
		else if( unit.type_token == 43 )	// }
		{
			brackets--;
			if( brackets < 0 )
			{
				printf("prilis mnoho uzaviracich zavorek\n");
				// TODO chyba
			}
		}
	
		unit = get_token();	// nacteni dalsiho tokenu
	}
	array[i] = unit;

	if( brackets != 0 )
	{
		printf("nespravny pocet zavorek\n");
		// TODO chyba
	}

	// vypis pole tokenu
	for(int x=0; x<512; x++)
	{
		if(array[x].type_token == 0)	break;
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
int expression(TOKEN*array, int i)
{
	bool isFirst = true;
	bool wasExpr = false;
	bool wasSign = false;
	int brackets = 0;

	TOKEN unit = get_token();

	while( unit.type_token > 0 && unit.type_token != 50 && unit.type_token != 100 )	// nic, EOF, chyba
	{
		array[i++] = unit;
		if( isFirst && unit.type_token == 40 )	// (
		{
			brackets++;
		}
		else if( isFirst && unit.type_token >= 30 && unit.type_token <= 36 )	// string, int, double, boolean, null, id, $id
		{
			isFirst = false;
			wasExpr = true;
			wasSign = false;
		}

		if( unit.type_token
	}

	return i;
}
