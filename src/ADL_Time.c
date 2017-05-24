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


#include "ADL_Time.h"

double GetTime(struct ADL_EVENT *evt){return ADL_G_TIME(evt);}
int SetupTime(char *libversion, char *setupfile){return ADL_Setup_TIME(libversion,setupfile);}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int ADL_Setup_TIME(char *libversion, char *setupfile){
    
    ADL_G_Setup_TIME  = ADL_Setup_TIME_FAST;
    ADL_G_Status_TIME = ADL_Status_TIME_FAST;
    ADL_G_TIME        = ADL_TIME_FAST;

	return ADL_Setup_TIME_FAST(setupfile);
}


int ADL_Status_TIME(void){
	//print final values that are setup:
	printf("\nStatus TIME:\n");
	printf("------------\n");
	printf("ADL_G_Setup_TIME  -> ADL_Setup_TIME_FAST\n");
	printf("ADL_G_Status_TIME -> ADL_Status_TIME_FAST\n");
	printf("ADL_G_TIME        -> ADL_TIME_FAST\n");
		
	ADL_G_Status_TIME();
	return 0;
}

