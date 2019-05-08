//The ADL includes:
#include "ADL.h"
//*****************************************************************************************/

int main(int argc, char **argv)
{
  char filename_setupfile[100];
    char StruFilename[100];
    char WpotFilename[100];
    char EpotFilename[100];

  int	i,len;
  struct	ADL_KEYWORD **Kwords;
  double ReadVersion=4.2;

  //initialize the required functions
  if(argc==1){printf("No SETUP file indicated.\n"); return 1;}
  else if(argc>2){
	// Store parameters in the output filename - useful for ICPC scans
	char *tmpStru = StruFilename;
	char *tmpWpot = WpotFilename;
	char *tmpEpot = EpotFilename;

	int arg;

//	tmpStru += sprintf(tmpStru, "%s" ,argv[2]);
//	tmpWpot += sprintf(tmpWpot, "%s" ,argv[2]);
//	tmpEpot += sprintf(tmpEpot, "%s" ,argv[2]);

	tmpStru += sprintf(tmpStru, "%s" ,"ICOAX_Stru");
	tmpWpot += sprintf(tmpWpot, "%s" ,"ICOAX_Wpot");
	tmpEpot += sprintf(tmpEpot, "%s" ,"ICOAX_Epot");

	for(arg = 2;arg<argc;arg++){
		tmpStru += sprintf(tmpStru, "_%s", argv[arg]);
		tmpWpot += sprintf(tmpWpot, "_%s", argv[arg]);
		tmpEpot += sprintf(tmpEpot, "_%s", argv[arg]);
	}

	tmpStru += sprintf(tmpStru, "%s" ,".pa");
	tmpWpot += sprintf(tmpWpot, "%s" ,".pa");
	tmpEpot += sprintf(tmpEpot, "%s" ,".pa");

	printf("Input files: %s \n",StruFilename);
	printf("Input files: %s \n",WpotFilename);
	printf("Input files: %s \n",EpotFilename);
  }


  sprintf(filename_setupfile, "%s", argv[1]);

  if (strlen(filename_setupfile)>1) ADLSetup(filename_setupfile);
  else return 1;
  //SetADLDebug(0); // Can be 0 or 1

  if(GetADLDebug()) printf("Setup file : %s \n",filename_setupfile);

//  if (ReadVersion != GetADLVersion()) printf("\nWARNING ADL (%.2lf) setup: version (%.2lf) outdated ?\n",GetADLVersion(),ReadVersion);

/////////////////////////////
//  READOUT INFORMATION    //
/////////////////////////////
/*
  ADLStatus();
  ADL_Status_CONVL_DUMMY();
  ADL_Status_DRIFT_GE();
  ADL_Status_EVENT();
  ADL_Status_FIELDS_SIMION();
  ADL_Status_PARSER();
  ADL_Status_READWRITE();
  ADL_Status_TIME_FAST();
  ADL_Status_TRACES_NUMRES();
  ADL_Status_TRAPPING();

  SIMION_Status_ARRAY();
  SIMION_Status_READWRITE();

  SIMION_Status_GEOMETRY();
  SIMION_Status_SOLVER();
*/
/////////////////////////////

  if(GetADLDebug()) printf("\nSetting up the structure!\n");
  struct SIMION_PA *field = GetSIMIONnewPA();
  GetSIMIONStructure(field, 1);
  if(argc>2) SIMION_WRITE_PA(field, 0, StruFilename);
  else SIMION_WRITE_PA(field, 0, 0);
  if(GetADLDebug()) printf("\n SIMION structure written!\n");

  if(GetADLDebug()) printf("\nProcessing space charge distribution!\n");

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
