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

#include "ADL_Fields.h"

double GetHeight(){return ADL_Fields_Height;}
int IsInDetector(double* P){return ADL_G_InDetector(P);}
int GetDetectorPos(double* P){return ADL_G_DetectorPos(P);}
double* GetEField(double* P){
    double E[4];
    ADL_G_Efield(P,E);
    return E;
}
double GetWeight(int j,double* P){ return ADL_G_Weight(j,P);}

int SetupFields(char *setupfile){return ADL_Setup_FIELDS(setupfile);}
void StatusFields(){ADL_Status_FIELDS();}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int ADL_Setup_FIELDS(char *setupfile){

    ADL_G_Setup_FIELDS               = ADL_Setup_FIELDS_SIMION;
    ADL_G_Status_FIELDS              = ADL_Status_FIELDS_SIMION;
    ADL_G_Weight                     = ADL_Weight_SIMION;
    ADL_G_DetectorPos                = ADL_DetectorPos_SIMION;
    ADL_G_InDetector                 = ADL_InDetector_SIMION;
    ADL_G_Efield                     = ADL_Efield_SIMION;
    ADL_G_Wfield                     = ADL_Wfield_SIMION;
    ADL_G_Geo                        = ADL_Geo_SIMION;
    ADL_Fields_Height = GetSimionHeight();
    return ADL_Setup_FIELDS_SIMION(setupfile);
}


static int ADL_Status_FIELDS(void){
  //print final values that are setup:
  printf("\nStatus FIELDS:\n");
  printf("--------------\n");
  printf("ADL_G_Setup_FIELDS  -> ADL_Setup_FIELDS_SIMION\n");
  printf("ADL_G_Status_FIELDS -> ADL_Status_FIELDS_SIMION\n");
  printf("ADL_G_Weight        -> ADL_Weight_SIMION\n");
  printf("ADL_G_DetectorPos   -> ADL_DetectorPos_SIMION\n");
  printf("ADL_G_InDetector    -> ADL_InDetector_SIMION\n");
  printf("ADL_G_Efield        -> ADL_Efield_SIMION\n");
  printf("ADL_G_Wfield        -> ADL_Wfield_SIMION\n");
  printf("ADL_G_Geo           -> ADL_Geo_SIMION\n");

  ADL_G_Status_FIELDS();
  return 0;
}



