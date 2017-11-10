//The ADL includes:
#include "ADL.h"
//*****************************************************************************************

int main(int argc, char **argv)
{
  char filename_setupfile[100];
  
  int	i,len;
  struct	ADL_KEYWORD **Kwords;
  double ReadVersion=4.2;
  
  printf("%i arguments given.\n",argc);
  
  //initialize the required functions
  if(argc==1){printf("No SETUP file indicated.\n"); return 1;}
  sprintf(filename_setupfile, "%s", argv[1]);

  printf("Setup file : %s \n",filename_setupfile);
  SetADLDebug(0); // Can be 0 or 1
  
  if (strlen(filename_setupfile)>1) ADLSetup(filename_setupfile);
  else return 1;
  
  if (ReadVersion != GetADLVersion()) printf("\nWARNING ADL (%.2lf) setup: version (%.2lf) outdated ?\n",GetADLVersion(),ReadVersion);
  
  ADLStatus();
  SIMION_Status_GEOMETRY();
  SIMION_Status_SOLVER();

  printf("\nSetting up the structure!\n");
  struct SIMION_PA *field = GetSIMIONnewPA();
  GetSIMIONStructure(field, 1);
  SIMION_WRITE_PA(field, 0, 0);
  printf("\n SIMION structure written!\n");
  
  printf("\nProcessing space charge distribution!\n");
  
  GetSIMIONWfield(field, 0);
  SIMION_Solve(field);
  SIMION_WRITE_PA(field, 0, 0);
  //        SIMION_WRITE_PA(field, 1, 0);
  //        SIMION_WRITE_PA(field, 2, 0);
  
  /*	struct SIMION_PA *field3D = NULL;*/
  /*	field3D = SIMION_2Dto3D_FIELD(field,0 );*/
  /*	SIMION_WRITE_PA(field3D, 0, 0);*/
  
  GetSIMIONEfield(field, 1);
  SIMION_Solve(field);
  SIMION_WRITE_PA(field, 0, 0);
  //        SIMION_WRITE_PA(field, 1, 0);
  //        SIMION_WRITE_PA(field, 2, 0);
  
  /*	field3D = SIMION_2Dto3D_FIELD(field,0 );*/
  /*	SIMION_WRITE_PA(field3D, 0, 0);*/
  
  return 0;
}
