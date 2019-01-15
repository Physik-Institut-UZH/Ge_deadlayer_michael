//The ADL includes:
#include "ADL.h"
//*****************************************************************************************

int main(int argc, char **argv)
{
  char filename_setupfile[100];
    char StruFilename[100];
    char WpotFilename[100];
    char EpotFilename[100];
  
  int	i,len;
  struct	ADL_KEYWORD **Kwords;
  double ReadVersion=4.2;
  
  printf("%i arguments given.\n",argc);
  
  //initialize the required functions
  if(argc==1){printf("No SETUP file indicated.\n"); return 1;}
  sprintf(filename_setupfile, "%s", argv[1]);

    if(argc==6){
        if(!strcmp(argv[5],"1")){
            printf("Input for calculations.\n");
                sprintf(StruFilename, "ICOAX_Stru_%s_%s_%s.pa", argv[2], argv[3], argv[4]);
            sprintf(WpotFilename, "ICOAX_Wpot_%s_%s_%s.pa", argv[2], argv[3], argv[4]);
                sprintf(EpotFilename, "ICOAX_Epot_%s_%s_%s.pa", argv[2], argv[3], argv[4]);
        }
	else if(!strcmp(argv[5],"0")){
            printf("Input for depletion voltage.\n");
            sprintf(StruFilename, "ICOAX_Stru_%s_%s.pa", argv[3], argv[4]);
            sprintf(WpotFilename, "ICOAX_Wpot_%s_%s.pa", argv[3], argv[4]);
            sprintf(EpotFilename, "ICOAX_Epot_%s_%s.pa", argv[3], argv[4]);
        }
	else printf("Bad input parameters given.\n");
    }
    else if(argc==7){
        if(!strcmp(argv[6],"1")){
            printf("Input for calculations.\n");
            sprintf(StruFilename, "ICOAX_Stru_%s_%s_%s_%s.pa", argv[3], argv[4], argv[5], argv[2]);
            sprintf(WpotFilename, "ICOAX_Wpot_%s_%s_%s_%s.pa", argv[3], argv[4], argv[5], argv[2]);
            sprintf(EpotFilename, "ICOAX_Epot_%s_%s_%s_%s.pa", argv[3], argv[4], argv[5], argv[2]);
        }
	else if(!strcmp(argv[6],"0")){
            printf("Input for depletion voltage.\n");
            sprintf(StruFilename, "ICOAX_Stru_%s_%s_%s.pa", argv[3], argv[4], argv[5]);
            sprintf(WpotFilename, "ICOAX_Wpot_%s_%s_%s.pa", argv[3], argv[4], argv[5]);
            sprintf(EpotFilename, "ICOAX_Epot_%s_%s_%s.pa", argv[3], argv[4], argv[5]);
        }
        else printf("Bad input parameters given.\n");
    }
    else printf("No input parameters given.\n");


  printf("Setup file : %s \n",filename_setupfile);
  
  if (strlen(filename_setupfile)>1) ADLSetup(filename_setupfile);
  else return 1;
  SetADLDebug(1); // Can be 0 or 1
  
  if (ReadVersion != GetADLVersion()) printf("\nWARNING ADL (%.2lf) setup: version (%.2lf) outdated ?\n",GetADLVersion(),ReadVersion);
  
  //ADLStatus();
  SIMION_Status_GEOMETRY();
  SIMION_Status_SOLVER();

  printf("\nSetting up the structure!\n");
  struct SIMION_PA *field = GetSIMIONnewPA();
  GetSIMIONStructure(field, 1);
  if(argc>2) SIMION_WRITE_PA(field, 0, StruFilename);
  else SIMION_WRITE_PA(field, 0, 0);
  printf("\n SIMION structure written!\n");
  
  printf("\nProcessing space charge distribution!\n");
  
  GetSIMIONWfield(field, 0);
  SIMION_Solve(field);
  if(argc>2) SIMION_WRITE_PA(field, 0, WpotFilename);
  else SIMION_WRITE_PA(field, 0, 0);
  //        SIMION_WRITE_PA(field, 1, 0);
  //        SIMION_WRITE_PA(field, 2, 0);
  
  /*	struct SIMION_PA *field3D = NULL;*/
  /*	field3D = SIMION_2Dto3D_FIELD(field,0 );*/
  /*	SIMION_WRITE_PA(field3D, 0, 0);*/
  
  GetSIMIONEfield(field, 1);
  SIMION_Solve(field);
  if(argc>2) SIMION_WRITE_PA(field, 0, EpotFilename);
  else SIMION_WRITE_PA(field, 0, 0);
  //        SIMION_WRITE_PA(field, 1, 0);
  //        SIMION_WRITE_PA(field, 2, 0);
  
  /*	field3D = SIMION_2Dto3D_FIELD(field,0 );*/
  /*	SIMION_WRITE_PA(field3D, 0, 0);*/
  
  return 0;
}
