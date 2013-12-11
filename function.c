#include "function.h"

/*
30    string
31    int
32    double
33    bool
34    null - Data Type
*/

//-------------------------------------
//vraci hodnotu konvertovanou na type bool (0/false,1/true)
int boolval(NODE value){

  int result = -1;

  if(value->data.type_token == 34){
    result = 0;
  }

  if(value->data.type_token == 33){
    result = value->data.boolean;
  }

  //double 0 == 0 lze?
  if(value->data.type_token == 32){
    if(value->data.d_number == 0.0){
      result = 0;
    } else result = 1;
  }

  if(value->data.type_token == 31){
    if(value->data.c_number == 0){
      result = 0;
    } else result = 1;
  }

  if(value->data.type_token == 30){
    if(strcmp("",value->data.string) == 0){
      result = 0;
    } else result = 1;
  }

  return result;
}

//-------------------------------------
// 0 = 48
// 9 = 57
// 69 = E
// 101 = e

double doubleval(NODE node){ //TODO pokud je spatny format cisla tak ukoncit napr 1.3e chyba
		int i = 0;
		double result = 0.0;
		int exponent = 0;
		switch(node->data.type_token){
			case 30://string
				break;
			case 31://int
				result = (double)node->data.c_number;
				return result;
				break;
			case 32://double
				result = node->data.d_number;
				return result;
				break;
			case 33://bool
				if ( node->data.boolean != 0 ){
					result = 1.0;
				} else {
					result = 0.0;
				}
				return result;
				break;
			case 34://null
				result = 0.0;
				return result;
				break;
			default:	//chyba!!!!!!!!!!!!!!
				eCode = 11;
				return EXIT_FAILURE;
				break;
			}
		char *string = node->data.string;

		while ( string[i] == ' ' || string[i] == '\n' || string[i] == '\t' || string[i] == '\r' ) { //preskoci prazdna mista
				i++;
			}

		if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
			while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
				result *= 10;
				result += (double) string[i]-48;
				i++;

			}
			if ( string[i] == 'E'  || string[i] == 'e' ){
				i++;
				if ( string[i] == '-'){
					i++;
					if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
					while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
						exponent *= 10;
						exponent += (double) string[i]-48;
						i++;
					}
					for (int i = 1; i <=exponent;i++ ){
						result/=10;
					}
					return result; // vysledek
					}else{
						eCode = 11;
						return EXIT_FAILURE;
					}
				}else{
					if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
						while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
							exponent *= 10;
							exponent += (double) string[i]-48;
							i++;
						}
						for (int i = 1; i <exponent;i++ ){
							result*=10;
						}
						return result; //vysledek
					}else{
						eCode = 11;
						return EXIT_FAILURE;
					}
				}												
			} else if( string[i] == '.' ){	// nasleduje desetina cast
					i++;
					if ( string[i] >= 48 && string[i] <= 57 ){
						int count = 10;
					while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double

						result += ((double) string[i]-48)/ count;
						count *= 10;
						i++;
					}										
					if ( string[i] == 'E'  || string[i] == 'e' ){
						i++;
						if ( string[i] == '-'){
							i++;
							if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
							while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
								exponent *= 10;
								exponent += (double) string[i]-48;
								i++;
							}
							for (int i = 1; i <=exponent;i++ ){
								result/=10;
							}
							return result; // vysledek
							}else{
								eCode = 11;
								return EXIT_FAILURE;
							}
						}else{
							if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
								while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
									exponent *= 10;
									exponent += (double) string[i]-48;
									i++;
								}
								for (int i = 1; i <exponent;i++ ){
									result*=10;
								}
								return result; //vysledek
							}else{
								eCode = 11;
								return EXIT_FAILURE;
							}
						}																							
					}
					}else{
						eCode = 11;

						return EXIT_FAILURE;
					}
			}
		} else if ( string[i] < 49 || string[i] > 57 ){
			return result;
		}
		return result;
}			


//Funkce prevede retezec na int
int intval(NODE node){
	int result = 0;
	int i = 0;
	switch( node->data.type_token) {
		case 30:
			break;
		case 31: // int
			result = node->data.c_number;
			return result;
			break;
		case 32: // double
			result = (int)node->data.d_number;
			return result;
			break;
		case 33: // bool
			if (node->data.boolean != 0) {
					result = 1;
				} else {
					result = 0;
				}
			return result;
			break;
		case 34: //null
			result = 0;
			return result;
			break;
		default: //chyba
			eCode = 13;
			return result;
			break;
	}

	char *string = node->data.string;

	while ( string[i] == ' ' || string[i] == '\n' || string[i] == '\t' || string[i] == '\r' ) { //preskoci prazdna mista
				i++;
	}
	if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
			while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
				result *= 10;
				result += (int) string[i]-48;
				i++;

			}
		}
	return result;
}
// funkce prevede datove typy na retezec
char *strval(NODE node){ //TODO uvolneni malloc dodelat
	char *result = malloc(100);
	char *nulll = "";
	char *True = "1";
	switch( node->data.type_token)
		{
			case 30: //string to string
				return node->data.string;
				break;
			case 31: //string to int
				sprintf(result, "%d", node->data.c_number);
				return &(*result);
				break;
			case 32: // string to double
				sprintf(result,"%g",node->data.d_number);
				return result;
				break;
			case 33: // string to bool
				if (node->data.boolean != 0) {
					return True;
				} else {
					return nulll;
				}
				break;
			case 34: // string to null
				return nulll;
				break;
			default: //chyba
				return result;
				break;
		}
	return result;
}

// funkce vraci delku retezce
unsigned int my_strlen (char *string)
{
    unsigned int length = 0;
    while (string[length] != '\0')
    {
        length++;
    }
    return length;
}

// funkce vraci podretezec z retezce ohraniceny indexy
char * my_get_substring (char *string, int start, int end)
{
    if (start < 0 || end < 0 || start > end || start >= (int)strlen(string) || end > (int)strlen(string))
    {
        eCode = 13;
        return NULL;
    }
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
    while ((c = getchar()) != EOF && c != '\n' && c != '\r') // cte se ze vstupu dokud se nenarazi na konec radku nebo EOF
    {
        buffer[i] = c;
        i++;
    }
    string = malloc ((i+1) * sizeof (char));
    strncpy (string, buffer, i);
    string[i] = '\0';
    return string;
}

// funkce vypisuje retezce na standartni vystup
void put_string(NODE *ptr)
{
    NODE node = *ptr;
    if (node->data.type_token == 30) printf("%s", node->data.string);
    else if (node->data.type_token == 31) printf("%i", node->data.c_number);
    else if (node->data.type_token == 32) printf("%f", node->data.d_number);
    else if (node->data.type_token == 33)
    {
        if (node->data.boolean == TRUE) printf("1");
    }
}
