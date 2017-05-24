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

#include "ADL_ReadWrite.h"

int SetupReadWrite(char *libversion, char *setupfile){return ADL_Setup_READWRITE(libversion,setupfile);}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int ADL_Setup_READWRITE(char *libversion_ignored, char *filename_setupfile){

    ADL_G_ReadFileHeader = ADL_ReadFileHeader_TXT;
    ADL_G_ReadEvent      = ADL_ReadEvent_TXT;
    ADL_G_ReadEventHP    = ADL_ReadEventHP_TXT;
    ADL_G_ReadEventTD    = ADL_ReadEventTD_TXT;
    
    ADL_G_WriteFileHeader = ADL_WriteFileHeader_TXT;
    ADL_G_WriteEvent      = ADL_WriteEvent_TXT;
    ADL_G_WriteEventHP    = ADL_WriteEventHP_TXT;
    ADL_G_WriteEventTD    = ADL_WriteEventTD_TXT;
    
	if (GetADLDebug()) ADL_Status_READWRITE();
	return 1;
}

int ADL_Status_READWRITE(void){
	//prints values that are setup:
	printf("\nSetup READWRITE:\n");
	printf("----------------\n");
	printf("ADL_G_ReadEvent  -> ADL_ReadEvent_TXT\n");
    printf("ADL_G_WriteEvent -> ADL_WriteEvent_TXT\n");
	return 0;
}
