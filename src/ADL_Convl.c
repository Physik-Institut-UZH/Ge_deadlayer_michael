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

#include "ADL_Convl.h"

int SetupConv(char *libversion, char *setupfile){return ADL_Setup_CONVL(libversion,setupfile);}

void ApplyConvolution(struct ADL_TRACES *trace){
    ADL_G_Convolute(trace);
}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int ADL_Setup_CONVL(char *libversion, char *setupfile){
    int		i,len,err=1;
    struct	ADL_KEYWORD **Kwords;

    ADL_G_INVERSE       = NULL;
    ADL_G_Setup_CONVL  = ADL_Setup_CONVL_DUMMY;
    ADL_G_Status_CONVL = ADL_Status_CONVL_DUMMY;
    ADL_G_Convolute = ADL_Convolute_DUMMY;
    
    return ADL_Setup_CONVL_DUMMY(setupfile);

}

int ADL_Status_CONVL(void){
	//print final values that are setup:
	printf("\nStatus CONVL:\n");
	printf("------------\n");
	printf("ADL_G_Setup_CONVL  -> ADL_Setup_CONVL_DUMMY\n");
	printf("ADL_G_Status_CONVL -> ADL_Status_CONVL_DUMMY\n");
	printf("ADL_G_Convolute    -> ADL_Convolute_DUMMY\n");
	
	ADL_G_Status_CONVL();
	return 0;
}



