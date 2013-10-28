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

		if(node->data.type_token != 30){
			fprintf(stderr, "Token neni tzpu string");
			}
		char *string = node->data.string;
		printf("%s\n",string);
		int i = 0;
		double result = 0.0;
		int exponent = 0;
		while ( string[i] == ' ' || string[i] == '\n' || string[i] == '\t' || string[i] == '\r' ) { //preskoci prazdna mista
				i++;
			}

		if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
			while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
				result *= 10;
				result += (double) string[i]-48;
				i++;

			}
			if ( string[i] == '+' ){ //exponent
				i++;
				if ( string[i] == 'E' || string[i] == 'e' ){
					i++;
				}else{
					fprintf(stderr,"Chyba spatny format cisla");
				}
				if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
					while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
						exponent *= 10;
						exponent += (double) string[i]-48;
						i++;
					}
					for (int i = 1; i <exponent;i++ ){
						result*=10;
					}
					return result; // odelslani vysledku Dodelat!!!!!!!!!!!!!!!
				}else{
					fprintf(stderr,"Chyba spatny format cisla");
				}


			}else if ( string[i] == '-' ){
				i++;
				if ( string[i] == 'E' || string[i] == 'e' ){
					i++;
				}else{
					fprintf(stderr,"Chyba spatny format cisla");
				}
				if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
					while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
						exponent *= 10;
						exponent += (double) string[i]-48;
						i++;
					}
					for (int i = 1; i <=exponent;i++ ){
						result/=10;
					}
					return result; // odelslani vysledku Dodelat!!!!!!!!!!!!!!!
				}else{
					fprintf(stderr,"Chyba spatny format cisla");
				}
			} else if ( string[i] == 'E' || string[i] == 'e' ){
				i++;
				if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
					while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
						exponent *= 10;
						exponent += (double) string[i]-48;
						i++;
					}
					for (int i = 1; i <=exponent;i++ ){
						result*=10;
					}
					return result; // odelslani vysledku Dodelat!!!!!!!!!!!!!!!
				}else{
					fprintf(stderr,"Chyba spatny format cisla");
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

					if ( string[i] == '+' ){
							i++;
							if ( string[i] == 'E' || string[i] == 'e' ){
								i++;
							}else{
								fprintf(stderr,"Chyba spatny format cisla");
							}
							if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
								while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
									exponent *= 10;
									exponent += (double) string[i]-48;
									i++;
								}
								for (int i = 1; i <=exponent;i++ ){
									result*=10;
								}
								return result;	// odelslani vysledku Dodelat!!!!!!!!!!!!!!!

							}else{
								fprintf(stderr,"Chyba spatny format cisla");
							}


						}else if ( string[i] == '-' ){
							i++;
							if ( string[i] == 'E' || string[i] == 'e' ){
								i++;
							}else{
								fprintf(stderr,"Chyba spatny format cisla");
							}
							if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
								while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
									exponent *= 10;
									exponent += (double) string[i]-48;
									i++;
								}
								for (int i = 1; i <=exponent;i++ ){
									result/=10;
								}
								return result; // odelslani vysledku Dodelat!!!!!!!!!!!!!!!
							}else{
								fprintf(stderr,"Chyba spatny format cisla");
							}
						} else if ( string[i] == 'E' || string[i] == 'e' ){
							i++;
							if ( string[i] >= 48 && string[i] <= 57 ){ // znak je cislo
								while ( string[i] >= 48 && string[i] <= 57 ){	//konvertuje retezcova cisla na cisla double
									exponent *= 10;
									exponent += (double) string[i]-48;
									i++;
								}
								for (int i = 1; i <=exponent;i++ ){
									result*=10;
								}
								return result;	// odelslani vysledku Dodelat!!!!!!!!!!!!!!!
							}else{
								fprintf(stderr,"Chyba spatny format cisla");
							}
						}
					}else{
						fprintf(stderr,"Chyba spatny format cisla");
					}
			}


		} else if ( string[i] < 49 || string[i] > 57 ){
			return result;
		}
		return result;
}

// funkce vraci delku retezce
unsigned int my_strlen (char *string)
{
    int i = 0;
    unsigned int length = 0;
    while (string[i] != '\0')
    {
        length++;
    }
    return length;
}

// funkce vraci podretezec z retezce ohraniceny indexy
char * my_get_substring (char *string, int start, int end)
{
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
    while ((c = getchar()) != EOF || c != '\n' || c != '\r') // cte se ze vstupu dokud se nenarazi na konec radku nebo EOF
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
void put_string(char *string, ...)
{
    va_list strings; int c = 0;
    va_start(strings, string);
    printf("%s", string);
    while ((c = vprintf( "%s", strings)) > 0)
    {
        // teoreticky by tady nic byt nemelo
    }
    va_end(strings);
}
