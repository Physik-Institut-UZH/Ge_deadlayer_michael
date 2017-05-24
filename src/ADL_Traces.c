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

#include "ADL.h"
//include different library versions here:
#include"ADL_Traces_NUMRES.h"

void CalculateTraces(struct ADL_EVENT *event){ADL_G_CalculateTraces(event);}
void StatusTraces(struct ADL_EVENT *evt){ADL_Status_TRACES(evt);}
int SetupTraces(char *setupfile){return ADL_Setup_TRACES(setupfile);}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
static int ADL_Setup_TRACES(char *setupfile){

    ADL_G_Setup_TRACES    = ADL_Setup_TRACES_NUMRES;
    ADL_G_Status_TRACES   = ADL_Status_TRACES_NUMRES;
    ADL_G_CalculateTraces = ADL_CalculateTraces_NUMRES;
    
    return ADL_Setup_TRACES_NUMRES(setupfile);
}


//int ADL_Status_TRACES(void){
static int ADL_Status_TRACES(struct ADL_EVENT *evt){

  //print final values that are setup:
  printf("\nStatus TRACES:\n");
  printf("--------------\n");
  printf("ADL_G_Setup_TRACES    -> ADL_Setup_TRACES_NUMRES\n");
  printf("ADL_G_Status_TRACES   -> ADL_Status_TRACES_NUMRES\n");
  printf("ADL_G_CalculateTraces -> ADL_CalculateTraces_NUMRES\n");
  
  ADL_G_Status_TRACES();
  
    return 0;
}



