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

#include "ADL_Drift.h"


int SetupDrift(char *libversion, char *setupfile){return ADL_Setup_DRIFT(libversion,setupfile);}
void GetElectronVelocity(double*E,double* velocity){ADL_G_CalculateElectronVelocity(E,velocity);}
void GetHoleVelocity(double*E,double *velocity){ADL_G_CalculateHoleVelocity(E,velocity);}


//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int ADL_Setup_DRIFT(char *libversion, char *setupfile){
    
    ADL_G_Setup_DRIFT               = ADL_Setup_DRIFT_GE;
    ADL_G_Status_DRIFT              = ADL_Status_DRIFT_GE;
    ADL_G_CalculateElectronVelocity = ADL_CalculateElectronVelocity_GE;
    ADL_G_CalculateHoleVelocity     = ADL_CalculateHoleVelocity_GE;

    return ADL_Setup_DRIFT_GE(setupfile);
}

int ADL_Status_DRIFT(void){
	//print final values that are setup:
	printf("\nStatus DRIFT:\n");
	printf("------------\n");
	printf("ADL_G_Setup_DRIFT               -> ADL_Setup_DRIFT_GE\n");
	printf("ADL_G_Status_DRIFT              -> ADL_Status_DRIFT_GE\n");
	printf("ADL_G_CalculateElectronVelocity -> ADL_CalculateElectronVelocity_GE\n");
	printf("ADL_G_CalculateHoleVelocity     -> ADL_CalculateHoleVelocity_GE\n");

    ADL_G_Status_DRIFT();
	return 0;
}



