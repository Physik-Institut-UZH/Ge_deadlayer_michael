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

#include <string.h>
#include <stdlib.h>
#include "ADL_Parser.h"

int InitializeParser(int &MAXLINELEN, int &MAXKEYWORD){
    
    MAXLINELEN = 200;  //maximum length of a line
    MAXKEYWORD = 200;  //maximum number of keywords
    
    return 0;
}

int SetupParser(char *libversion, char *setupfile){return ADL_Setup_PARSER(libversion,setupfile);}

/****************************************************************************/
/* Dynamic allocation of a KEYWORD structure								*/
struct ADL_KEYWORD *ADL_new_keyword(void){
	struct ADL_KEYWORD *returnvalue;
	returnvalue = (struct ADL_KEYWORD *) calloc(1,sizeof(struct ADL_KEYWORD));
	returnvalue->linenumber = 0;
	returnvalue->keyword = (char *) calloc(ADL_G_MAXLINELEN+1,sizeof(char));
	returnvalue->svalue  = (char *) calloc(ADL_G_MAXLINELEN+1,sizeof(char));
	return returnvalue;
	}


/****************************************************************************/
/* The obligatory setup routine												*/
int ADL_Setup_PARSER(char *lib_version,char *filename_setupfile){

    InitializeParser(ADL_G_MAXLINELEN,ADL_G_MAXKEYWORD);

	int	i,len,err=1;
	struct	ADL_KEYWORD **Kwords;

	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_MAXLINELEN")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_MAXLINELEN);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_MAXKEYWORD")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_MAXKEYWORD); 
			}
		} //end parsing file	

	if (GetADLDebug()) ADL_Status_PARSER();
	return err;
	}

int ADL_Status_PARSER(void){
	//print final values that are setup:
	printf("\nStatus PARSER:\n");
	printf("--------------\n");
	//only two parameters to setup here:
	printf("ADL_G_MAXLINELEN = %d\n",ADL_G_MAXLINELEN);
	printf("ADL_G_MAXKEYWORD = %d\n",ADL_G_MAXKEYWORD);
	return 0;
}


/****************************************************************************/
/* Removes comments and trailing white space and comments from a line       */
static int ADL_parse_strip(char *line) {
   
   int i;
   
   /* Strip comments */
   for (i = 0; i < (int)strlen(line); i++)
     if (line[i] == '!' || line[i] == '#') {
		line[i] = 0;
		break;
     }
   
   /* Remove trailing white space */
   for (i--; i >= 0; i--) if (!(line[i]==' ')) break;
   line[i+1] = 0;
 
   return 0;
}



/****************************************************************************/
/* Parse a line from the setup file											*/
static int ADL_parse_line ( FILE *fp, char *keyword, char *svalue ) {
	//*fp is the pointer to the (opened) setup file, ready to read the next line
	//returned is a string containing the keyword - the name of the parameter to change
	//svalue contains the new value (string) the keyword should obtain
	//parse_file returns the total number of lines read (int) from the starting position

	char *comment,*line;
	int status,linenum = 0;
    
	// creating the memory...
	line	= (char *) calloc(ADL_G_MAXLINELEN+1,sizeof(char));
	comment = (char *) calloc(ADL_G_MAXLINELEN+1,sizeof(char));

	// read till something usefull shows up
	while (1) { 
		/* Read a line */
		if (fgets(line, ADL_G_MAXLINELEN - 1, fp) == NULL) {
			sprintf(keyword,"%s","EOF");  //set keyword to EOF - "end of file"
			return 0;					  //exit
		}
		linenum++;

		/* Strip comments and trailing space */
		ADL_parse_strip(line);
      
		/* If the line is not empty after stripping comments and trailing space, break loop */
		if (strlen(line) > 1) break;
	}

    /* Read the parameters from the line */
    status = sscanf(line,"%s %s %s \n",keyword,svalue,comment);
    if (status < 2) { // only 1 or 0 strings read: parse error.
		sprintf(svalue,"keyword %s - svalue is missing.",keyword);
		sprintf(keyword,"%s","ERROR");
	}

    if (status > 2) { // more than 2 strings read: parse error.
		sprintf(comment,"Badly commented?. Try using #,!. keyword = %s svalue = %s",keyword,svalue);
		sprintf(keyword,"%s","ERROR");
		sprintf(svalue,"%s",comment);
	}

	
	//return number of lines read
    return linenum;
}


/****************************************************************************/
/* MAIN: Parse all keywords from the setup file								*/
struct ADL_KEYWORD **ADL_parse_file (char *filename_ext) {
	//*filename is the pointer to the (opened) setup file, ready to read the next line
	//returned is a list of strings containing the keyword and the corresponding values.
	//The first  keyword "FILENAME" contains the name of the file which is parsed
	//The second keyword "LENGTH" contains the amount of keywords passed on (in linenum)
	//after that, the list with key words follows
	//Then the keyword "ERRORS" follows containing the amount of errors retrieved (in linenum)
	//and then the list with errors follows

	struct ADL_KEYWORD *temp; 
	struct ADL_KEYWORD **kwords; 
	struct ADL_KEYWORD **errors;
	struct ADL_KEYWORD **returnval;
	int i,linenum = 0;
	int numkwords = 0;
	int numerrors = 0;
	FILE *fp;

	char* filename;
	if(filename_ext[0] == '/') filename = filename_ext;
	else if(filename_ext[0] == 'C'){
		char* prefix = getenv("ADLDIR");
                filename = (char*)malloc(strlen(prefix) + strlen(filename_ext) +1);
                strcpy( filename, prefix );
                strcat( filename, "/" );
                strcat( filename, filename_ext );
	}
	else{
		char* prefix = getenv("MAGEDIR");
		filename = (char*)malloc(strlen(prefix) + strlen(filename_ext) +1);
		strcpy( filename, prefix );
		strcat( filename, "/" );
		strcat( filename, filename_ext );	
	}
	if(GetADLDebug()) printf("DEBUG PARSER : SETUP FILE : %s \n", filename);

        InitializeParser(ADL_G_MAXLINELEN,ADL_G_MAXKEYWORD);
    
	//allocation of keywords:
	kwords = (struct ADL_KEYWORD **) calloc(ADL_G_MAXKEYWORD,sizeof(struct ADL_KEYWORD*));
	errors = (struct ADL_KEYWORD **) calloc(ADL_G_MAXKEYWORD,sizeof(struct ADL_KEYWORD*));
    
	fp = fopen(filename,"r");
	if (fp == NULL) { //opening file failed:
		// add this error to list of errors:
		numkwords = 0;
		numerrors = 1;
		errors[0] = ADL_new_keyword();
		sprintf(errors[0]->keyword,"%s","ERROR");
		sprintf(errors[0]->svalue,"file %s could not be opened",filename);
		errors[0]->linenumber = 0;
		}

	else { // read file till end of file:
		while ((numerrors < ADL_G_MAXKEYWORD)&&(numkwords < ADL_G_MAXKEYWORD)) {
			//allocate memory for next keyword:
			temp = ADL_new_keyword();
			// read next line of input from file:
			linenum += ADL_parse_line (fp,temp->keyword,temp->svalue);

            temp->linenumber = linenum;
			// break if end of file is detected:
			if (strcmp(temp->keyword,"EOF")==0) {free(temp); break;}

			// if ERROR, increase error counter,
			// and add ERROR keyword to end of list:
			if (strcmp(temp->keyword,"ERROR")==0) {
				errors[numerrors]= temp;
				numerrors++;				
				}
			else { 
			// else add KEYWORD to list
				kwords[numkwords]= temp;
				numkwords++;
				}
		
		} //while
	} //else

		//The lists with errors and keywords 
		//now will be returned:

		// allocate minimum length of keywords (4)
		returnval = (struct ADL_KEYWORD **) calloc(3+numkwords+numerrors,sizeof(struct ADL_KEYWORD*));
		// set FILENAME linenum to 0
		returnval[0]= ADL_new_keyword();
		sprintf(returnval[0]->keyword,"%s","FILENAME");
		sprintf(returnval[0]->svalue,"%s",filename);
		returnval[0]->linenumber = 3 + numkwords + numerrors;
		// set LENGTH linenum to 0
		returnval[1]= ADL_new_keyword();		
		sprintf(returnval[1]->keyword,"%s","KEYWORDS");
		sprintf(returnval[1]->svalue,"%s","----------------");
		returnval[1]->linenumber = numkwords;
		// add all keywords to list:
		for (i=0;i<numkwords;i++) returnval[2+i]=kwords[i];

		// set ERRORS linenum to amount of errors detected
		returnval[2+numkwords]= ADL_new_keyword();
		sprintf(returnval[2+numkwords]->keyword,"%s","ERRORS");
		sprintf(returnval[2+numkwords]->svalue,"%s","----------------");
		returnval[2+numkwords]->linenumber = numerrors;
		// add all errors to list:
		for (i=0;i<numerrors;i++) returnval[3+numkwords+i]=errors[i];
	
	//free memory:
	free(kwords);
	free(errors);
	fclose(fp);

	if (GetADLDebug()) {
		printf("\nParser call returned:\n");
		ADL_print_keywords(returnval);
	}

	return returnval;
}


/****************************************************************************/
/* Print all parsed keywords in list										*/
int ADL_print_keywords(struct ADL_KEYWORD **list) {
	int i,len;

	len  = list[0]->linenumber;
	if ((len < 0)||(len>=ADL_G_MAXKEYWORD)) {
		printf("\n ERROR: print_keywords: lengt of list %d unreasonable \n",len);
		return 0;
		}

	printf("\nKEYWORD LIST:\n");
	for (i=0;i<len;i++)
		printf(" %d\t %20s = %s\n",list[i]->linenumber,list[i]->keyword,list[i]->svalue);

	return len;
	}
