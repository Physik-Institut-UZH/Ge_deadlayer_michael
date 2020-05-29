/*
 * AGATA Detector simulation Library (ADL)
 *
 * Bart Bruyneel - bartb@ikp.uni-koeln.de
 * Benedikt Birkenbach - bene@ikp.uni-koeln.de
 *
 * IKP Cologne
 * http://www.ikp.uni-koeln.de/agata
 *
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "ADL.h"
//Global defined parameters for this library:
static int ADL_G_MAXLINELEN;	//maximum length of a line
static int ADL_G_MAXKEYWORD;	//maximum number of keywords

int InitializeParser(int&, int&);
int SetupParser(char*,char*);

//DEFINITION: the ADL_KEYWORD structure:
#ifndef	ADL_KEYWORD_
#define ADL_KEYWORD_
struct ADL_KEYWORD {
	int	linenumber;	// contains the line number (*) at which the keyword was found
	char *keyword;	// a string containing the keyword read from file
	char *svalue;	 // a string containing the value of the keyword - needs cast from string to int,double,...
	char *svalue2; // a string containing a 2nd value of the keyword - needs cast from string to int,double,...
  // (*) exception is the keywords "FILENAME","KEYWORDS" and "ERRORS"
	//     for these keywords, the linenumber is replaced with respectively
	//     the total lenght of the keyword array, the number of keywords found, the number of errors found 
};
#endif /* ADL_KEYWORD_ */

//Declaration of the library member functions:
int ADL_Setup_PARSER(char *,char *);
int ADL_Status_PARSER(void);
struct ADL_KEYWORD **ADL_parse_file (char *);
int ADL_print_keywords(struct ADL_KEYWORD **); 

#endif /* SETUP_PARSER_H_ */

