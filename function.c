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

