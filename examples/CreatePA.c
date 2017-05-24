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
	if(argc==1){printf("No SETUP file indicated.\n"); return 1;};
	sprintf(filename_setupfile, "%s", argv[1]);
    
    ADLSetup(filename_setupfile);
    
	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
        printf("\n Parse setup file ... \n");
        Kwords = ADL_parse_file(filename_setupfile);
        printf("\n ... done !\n");

		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_DEBUG")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_DEBUG);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_VERSION")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ReadVersion);
			//GEOMETRY:
			if ( (strcmp(Kwords[2+i]->keyword,"SIMION_GEOMETRY_PLANAR")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"SIMION_GEOMETRY_BEGE")==0)
		           ||(strcmp(Kwords[2+i]->keyword,"SIMION_GEOMETRY_ICOAX")==0)
               		   ||(strcmp(Kwords[2+i]->keyword,"SIMION_GEOMETRY_COAX")==0) )
			   printf("%-20.20s < %-35.35s %d\n",Kwords[2+i]->keyword,Kwords[2+i]->svalue,SIMION_Setup_GEOMETRY(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
			if ( (strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_POISSON")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_LAPLACE")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_INHOMOG")==0))
			   printf("%-20.20s < %-35.35s %d\n",Kwords[2+i]->keyword,Kwords[2+i]->svalue,SIMION_Setup_SOLVER(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
		}
	} //end parsing file	

	if (ReadVersion != GetADLVersion()) printf("\nWARNING ADL (%.2lf) setup: version (%.2lf) outdated ?\n",GetADLVersion(),ReadVersion);

	SetADLDebug(ADL_G_DEBUG);

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
