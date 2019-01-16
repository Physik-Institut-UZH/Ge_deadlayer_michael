#include "SIMION_Geometry_PLANAR.h"

//setup geometry
int PLANAR_G_TopContactDepth = 1;
int PLANAR_G_Height          = 100; 
int PLANAR_G_BotContactDepth = 1;
int PLANAR_G_Spacing         = 0;
int PLANAR_G_ExtGroundWidth  = 0;
double PLANAR_G_ImpTop           = 1;
double PLANAR_G_ImpBot           = 1;

int SIMION_Setup_GEOMETRY_PLANAR(char *filename_setupfile) {

	int	i,len, err=1;
	double temp=0;
	struct	ADL_KEYWORD **Kwords;
	
	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		//first scan for gridsize!
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_GridSize")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_Planar);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Dimension")==0) 
				sscanf(Kwords[2+i]->svalue,"%i",&SIMION_Dimension_Planar);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Voltage")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Voltage_Planar);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Description")==0) 
				sscanf(Kwords[2+i]->svalue,"%s",SIMION_Description_Planar);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_Planar);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsExtScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsExtScale_Planar);
		}
		for (i=0;i<len;i++) {		
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_BotContactDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				PLANAR_G_BotContactDepth=SIMION_grid(temp,SIMION_GridSize_Planar);
			}
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_Height")==0){
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				PLANAR_G_Height=SIMION_grid(temp,SIMION_GridSize_Planar);
			}
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_TopContactDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				PLANAR_G_TopContactDepth=SIMION_grid(temp,SIMION_GridSize_Planar);
			}
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_Spacing")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				PLANAR_G_Spacing=SIMION_grid(temp,SIMION_GridSize_Planar);
			}
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_ExtGroundWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				PLANAR_G_ExtGroundWidth=SIMION_grid(temp,SIMION_GridSize_Planar);
			}
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_ImpTop")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&PLANAR_G_ImpTop);
			if (strcmp(Kwords[2+i]->keyword,"PLANAR_G_ImpBot")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&PLANAR_G_ImpBot);
		}
	}
	return err;
}

int SIMION_Status_GEOMETRY_PLANAR(void){
	//print final values that are setup:
	printf("\nStatus SIMION_Geometry_PLANAR:\n");
	printf("---------------------\n");
	printf("SIMION_G_GridSize        = %lf\n",SIMION_GridSize_Planar);
	printf("SIMION_G_EpsScale        = %lf\n",SIMION_EpsScale_Planar);
	printf("SIMION_G_EpsExtScale     = %lf\n",SIMION_EpsExtScale_Planar);
	printf("SIMION_G_Description     = %s \n",SIMION_Description_Planar);
	printf("SIMION_G_Dimension       = %i \n",SIMION_Dimension_Planar);
	printf("SIMION_G_Voltage         = %lf\n",SIMION_Voltage_Planar);
	printf("---------------------\n");
	printf("PLANAR_G_BotContactDepth = %ld\n", (long int) PLANAR_G_BotContactDepth); // fixed warnings
	printf("PLANAR_G_TopContactDepth = %ld\n", (long int) PLANAR_G_TopContactDepth);
	printf("PLANAR_G_Height          = %ld\n", (long int) PLANAR_G_Height);
	printf("PLANAR_G_Spacing         = %ld\n", (long int) PLANAR_G_Spacing);
	printf("PLANAR_G_ExtGroundWidth  = %ld\n", (long int) PLANAR_G_ExtGroundWidth);
	printf("PLANAR_G_ImpTop          = %lf\n", PLANAR_G_ImpTop);
	printf("PLANAR_G_ImpBot          = %lf\n", PLANAR_G_ImpBot);

	return 0;
}

struct SIMION_PA *SIMION_newPA_PLANAR(void){
	//adding 1.0 for taking care of indices (NX>imax) and 0.5 for correct rounding
	int npot=3;
	int NZ=2*PLANAR_G_ExtGroundWidth + 2*PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth + 1.0;
	struct SIMION_PA * pa = new_SIMION_PA(SIMION_Description_Planar, npot, SIMION_GridSize_Planar, 3, 3, NZ);
	return pa;
}

int SIMION_CalcPoint_PLANAR(int nx, int ny, int nz, int i){ // get the radii

	if (i!=1 && i!=0){
		printf("\nWARNING in SIMION_CalcPoint_PLANAR: Wrong contact number %d!\n",i);
	}

	if(     nz < PLANAR_G_ExtGroundWidth) return EXT_GROUND;
	else if(nz >= PLANAR_G_ExtGroundWidth  && 
	        nz < PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing)
		return OUTSIDE;
	else if(nz >= PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing && 
	        nz < PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing + PLANAR_G_BotContactDepth){
		if (i==0) return V_CONT;
		else return Z_CONT;
	}
	else if(nz >= PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing + PLANAR_G_BotContactDepth && 
	        nz < PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height)
		return BULK;
	else if(nz >= PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height && 
	        nz <= PLANAR_G_ExtGroundWidth + PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth){
		if (i==1) return V_CONT;
		else return Z_CONT;
	}
	else if(nz >= PLANAR_G_ExtGroundWidth +   PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth && 
	        nz < PLANAR_G_ExtGroundWidth + 2*PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth)
		return OUTSIDE;
	else if(nz >= PLANAR_G_ExtGroundWidth + 2*PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth &&
	        nz < 2*PLANAR_G_ExtGroundWidth + 2*PLANAR_G_Spacing + PLANAR_G_BotContactDepth + PLANAR_G_Height + PLANAR_G_TopContactDepth)
		return EXT_GROUND;
}

double SIMION_CalcCharge_PLANAR(int nx, int ny, int nz, int i){ // get the radii

	return PLANAR_G_ImpBot+(PLANAR_G_ImpTop-PLANAR_G_ImpBot)*((nz-PLANAR_G_Spacing-PLANAR_G_BotContactDepth-PLANAR_G_ExtGroundWidth)/PLANAR_G_Height);
}
