/*
 * AGATA Detector simulation Library (ADL)
 *
 * Bart Bruyneel - bartb@ikp.uni-koeln.de
 * Benedikt Birkenbach - bene@ikp.uni-koeln.de
 * Yoann Kermaidic - yoann.kermaidic@mpi-hd.mpg.de
 *
 * IKP Cologne
 * http://www.ikp.uni-koeln.de/agata
 *
 * MPIK Heidelberg
 * http://www.mpi-hd.mpg.de/gerda/home
 *
 */


#include "ADL.h"

#define Nlib 11                             //number of libraries in ADL

char    ADL_G_ACTIVE_LIBRARY [Nlib][100];   //Here we store the active libraries that were setup (alphabetically)

void SetADLDebug(int Debug){ ADL_G_DEBUG = Debug;}
void SetADLVersion(double Version){ ADL_G_VERSION = Version;}
void ADLSetup(char* buffer){ ADL_Setup(buffer);}
void ADLStatus(){ ADL_Status();}

int GetADLDebug(){return ADL_G_DEBUG;}
int GetADLVersion(){return ADL_G_VERSION;}

/****************************************************************************/
/* The obligatory setup routine												*/
int ADL_Setup(char *filename_setupfile){

	int	i,len;
	struct	ADL_KEYWORD **Kwords;
	double ReadVersion=4.10;

	//We mark first all libraries as not initialized:
	//remark that this is not true if setup is repeatedly called!!!
	for (i=0;i<Nlib;i++) sprintf(ADL_G_ACTIVE_LIBRARY[i],"%s","not initialized");

	//GEOMETRY:
	SetupGeometry(filename_setupfile);

	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_DEBUG")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_DEBUG);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_VERSION")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ReadVersion);
			//CONVL:*
			if ( (strcmp(Kwords[2+i]->keyword,"ADL_CONVL_1EVNT")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"ADL_CONVL_ARRAY")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"ADL_CONVL_DUMMY")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"ADL_CONVL_SAMPLES")==0) )
				sprintf(ADL_G_ACTIVE_LIBRARY [0],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                        SetupConv(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
			//DRIFT:*
			if ( (strcmp(Kwords[2+i]->keyword,"ADL_DRIFT_CONST")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"ADL_DRIFT_GE")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"ADL_DRIFT_ISOTR")==0) )
			   sprintf(ADL_G_ACTIVE_LIBRARY [1],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                       SetupDrift(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
			//EVENT:*
			if (strcmp(Kwords[2+i]->keyword,"ADL_EVENT")==0)
				sprintf(ADL_G_ACTIVE_LIBRARY [2],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                        SetupEvent(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
			//FIELDS:*
//			if ( (strcmp(Kwords[2+i]->keyword,"ADL_FIELDS_COAX")==0)
//			   ||(strcmp(Kwords[2+i]->keyword,"ADL_FIELDS_PLANAR")==0)
//                ||(strcmp(Kwords[2+i]->keyword,"ADL_FIELDS_SIMION")==0) ){
//                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL FIELDS ... ");
//			   sprintf(ADL_G_ACTIVE_LIBRARY [3],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
//                       SetupFields(Kwords[2+i]->svalue));
//                if(GetADLDebug()) printf("done \n");
//            }
			//PARSER:*
            if (strcmp(Kwords[2+i]->keyword,"ADL_PARSER")==0) {
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL PARSER ... ");
                sprintf(ADL_G_ACTIVE_LIBRARY [4],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                        SetupParser(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
           }
			//READWRITE:*
            if (strcmp(Kwords[2+i]->keyword,"ADL_READWRITE")==0){
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL READWRITE ... ");
				sprintf(ADL_G_ACTIVE_LIBRARY [5],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,SetupReadWrite(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
            }
			//TIME:*
            if (strcmp(Kwords[2+i]->keyword,"ADL_TIME_FAST")==0){
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL TIME ... ");
				sprintf(ADL_G_ACTIVE_LIBRARY [6],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                        SetupTime(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
            }
			//TRACES:*
			if ( (strcmp(Kwords[2+i]->keyword,"ADL_TRACES_NUMRES")==0)
                ||(strcmp(Kwords[2+i]->keyword,"ADL_TRACES_PULSER")==0) ){
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL TRACES ... ");
			   sprintf(ADL_G_ACTIVE_LIBRARY [7],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,
                       SetupTraces(Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
            }
			//TRAPPING:*
            if (strcmp(Kwords[2+i]->keyword,"ADL_TRAPPING")==0){
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL TRAPPING ... ");
			   sprintf(ADL_G_ACTIVE_LIBRARY [8],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,SetupTrapping(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
            }
			//SOLVER:*
			if ( (strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_POISSON")==0)
			   ||(strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_LAPLACE")==0)
                ||(strcmp(Kwords[2+i]->keyword,"SIMION_SOLVER_INHOMOG")==0)){
                if(GetADLDebug()) printf("\n DEBUG : SETUP ADL SOLVER ... ");
			   sprintf(ADL_G_ACTIVE_LIBRARY [10],"%-20.20s < %-35.35s %d",Kwords[2+i]->keyword,Kwords[2+i]->svalue,SetupSolver(Kwords[2+i]->keyword,Kwords[2+i]->svalue));
                if(GetADLDebug()) printf("done \n");
            }
		}
	} //end parsing file	

//	if (GetADLDebug() && ReadVersion != GetADLVersion()) printf("\nWARNING ADL (%.2lf) setup: version (%.2lf) outdated ?\n", (double) GetADLVersion(), (double) ReadVersion);
	if (GetADLDebug()) ADL_Status();
	return 1;
	}

int ADL_Status(void){
	//print final values that are setup:
	printf("\nStatus ADL:      Active Library         Parsed File                    Errors\n");
	printf("-----------------------------------------------------------------------------\n");
	printf("ADL_Convl        -> %s\n",ADL_G_ACTIVE_LIBRARY [0]);
	printf("ADL_Drift        -> %s\n",ADL_G_ACTIVE_LIBRARY [1]);
	printf("ADL_Event        -> %s\n",ADL_G_ACTIVE_LIBRARY [2]);
	printf("ADL_Fields       -> %s\n",ADL_G_ACTIVE_LIBRARY [3]);
	printf("ADL_Parser       -> %s\n",ADL_G_ACTIVE_LIBRARY [4]);
	printf("ADL_ReadWrite    -> %s\n",ADL_G_ACTIVE_LIBRARY [5]);
	printf("ADL_Time         -> %s\n",ADL_G_ACTIVE_LIBRARY [6]);
	printf("ADL_Traces       -> %s\n",ADL_G_ACTIVE_LIBRARY [7]);
	printf("ADL_Trapping     -> %s\n",ADL_G_ACTIVE_LIBRARY [8]);
	printf("SIMION_Geometry  -> %s\n",ADL_G_ACTIVE_LIBRARY [9]);
	printf("SIMION_Solver    -> %s\n",ADL_G_ACTIVE_LIBRARY [10]);
	return 0;
}
