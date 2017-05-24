#include "SIMION_Geometry_BEGE.h"

//setup geometry

int BEGE_G_GrooveDepth         = 30;
int BEGE_G_GrooveWidth         = 30;
int BEGE_G_PointContactRadius  = 70;
int BEGE_G_PointContactDepth   = 1;
int BEGE_G_PasLayThickness     = 0;
int BEGE_G_Radius              = 300;
int BEGE_G_Height              = 300; 
int BEGE_G_SurfaceContactDepth = 10;
int BEGE_G_GrooveInnerRadius   = 70;
int BEGE_G_EdgeRadius          = 0;
int BEGE_G_EdgeHeight          = 1;
int BEGE_G_Spacing             = 0;
int BEGE_G_ExtGroundWidth      = 0;
double  BEGE_G_ImpTop              = 1.0;
double  BEGE_G_ImpBot              = 1.0;
double  BEGE_G_Center              = 0;

int SIMION_Setup_GEOMETRY_BEGE(char *filename_setupfile) {

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
		//first extract all the SIMION parameter (needed to set up the grid parameters)
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_GridSize")==0)
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_Bege);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Dimension")==0) 
				sscanf(Kwords[2+i]->svalue,"%i",&SIMION_Dimension_Bege);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Voltage")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Voltage_Bege);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_Bege);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsExtScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsExtScale_Bege);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Description")==0) 
				sscanf(Kwords[2+i]->svalue,"%s",SIMION_Description_Bege);
		}
		//now extract the BEGE (grid) parameter
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_ImpTop")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&BEGE_G_ImpTop);
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_ImpBot")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&BEGE_G_ImpBot);
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_Radius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				BEGE_G_Radius=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_Height")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_Height=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_SurfaceContactDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_SurfaceContactDepth=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_GrooveDepth")==0){
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				BEGE_G_GrooveDepth=SIMION_grid(temp,SIMION_GridSize_Bege);
				}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_GrooveWidth")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_GrooveWidth=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_PointContactRadius")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_PointContactRadius=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_GrooveInnerRadius")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_GrooveInnerRadius=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_PointContactDepth")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_PointContactDepth=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_PasLayThickness")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_PasLayThickness=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_EdgeRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				BEGE_G_EdgeRadius=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_EdgeHeight")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_EdgeHeight=SIMION_grid(temp,SIMION_GridSize_Bege);
				if (BEGE_G_EdgeHeight<1) BEGE_G_EdgeHeight=1;
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_Spacing")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_Spacing=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
			if (strcmp(Kwords[2+i]->keyword,"BEGE_G_ExtGroundWidth")==0){
				sscanf(Kwords[2+i]->svalue,"%lf",&temp);
				BEGE_G_ExtGroundWidth=SIMION_grid(temp,SIMION_GridSize_Bege);
			}
		
		}
	}

	if(SIMION_Dimension_Bege==2) BEGE_G_Center=0;
	else BEGE_G_Center = BEGE_G_Radius + BEGE_G_Spacing + BEGE_G_ExtGroundWidth - 1.0;
	return err;
}

int SIMION_Status_GEOMETRY_BEGE(void){
	//print final values that are setup:
	printf("\nStatus SIMION_Geometry_BEGE:\n");
	printf("---------------------\n");
	printf("SIMION_G_GridSize          = %lf\n",SIMION_GridSize_Bege);
	printf("SIMION_G_EpsScale          = %lf\n",SIMION_EpsScale_Bege);
	printf("SIMION_G_EpsExtScale       = %lf\n",SIMION_EpsExtScale_Bege);
	printf("SIMION_G_Description       = %s \n",SIMION_Description_Bege);
	printf("SIMION_G_Dimension         = %i \n",SIMION_Dimension_Bege);
	printf("SIMION_G_Voltage           = %lf\n",SIMION_Voltage_Bege);
	printf("---------------------\n");
	printf("BEGE_G_Radius              = %ld\n",BEGE_G_Radius);
	printf("BEGE_G_Height              = %ld\n",BEGE_G_Height);
	printf("BEGE_G_GrooveDepth         = %ld\n",BEGE_G_GrooveDepth);
	printf("BEGE_G_GrooveWidth         = %ld\n",BEGE_G_GrooveWidth);
	printf("BEGE_G_PointContactRadius  = %ld\n",BEGE_G_PointContactRadius);
	printf("BEGE_G_GrooveInnerRadius   = %ld\n",BEGE_G_GrooveInnerRadius);
	printf("BEGE_G_PointContactDepth   = %ld\n",BEGE_G_PointContactDepth);
	printf("BEGE_G_PasLayThickness     = %ld\n",BEGE_G_PasLayThickness);
	printf("BEGE_G_EdgeRadius          = %ld\n",BEGE_G_EdgeRadius);
	printf("BEGE_G_EdgeHeight          = %ld\n",BEGE_G_EdgeHeight);
	printf("BEGE_G_SurfaceContactDepth = %ld\n",BEGE_G_SurfaceContactDepth);
	printf("BEGE_G_ImpTop              = %lf\n",BEGE_G_ImpTop);
	printf("BEGE_G_ImpBot              = %lf\n",BEGE_G_ImpBot);
	printf("BEGE_G_Center              = %lf\n",BEGE_G_Center);
	printf("BEGE_G_Spacing             = %ld\n",BEGE_G_Spacing);
	printf("BEGE_G_ExtGroundWidth      = %ld\n",BEGE_G_ExtGroundWidth);
	return 0;
}

struct SIMION_PA *SIMION_newPA_BEGE(void){
	//adding 0.5 for correct rounding

	int npot=3;
	
	int NX=1;
	int NY=1;
	int NZ=(BEGE_G_Height+2.0*BEGE_G_Spacing+2.0*BEGE_G_ExtGroundWidth);
	if (SIMION_Dimension_Bege==2){
		NX=(BEGE_G_Radius+BEGE_G_Spacing+BEGE_G_ExtGroundWidth);
		NY=1;
	}
	else{
		NX=2.0*(BEGE_G_Radius+BEGE_G_Spacing+BEGE_G_ExtGroundWidth)-1.0;
		NY=2.0*(BEGE_G_Radius+BEGE_G_Spacing+BEGE_G_ExtGroundWidth)-1.0;
	}

	struct SIMION_PA * pa = new_SIMION_PA(SIMION_Description_Bege, npot, SIMION_GridSize_Bege, NX, NY, NZ);

	return pa;
}

int SIMION_CalcPoint_BEGE(int nx, int ny, int nz, int i){ // get the radii

	if (i!=1 && i!=0){
		printf("\nWARNING in SIMION_CalcPoint_BEGE: Wrong contact number %d!\n",i);
	}

	if(SIMION_Dimension_Bege==2) BEGE_G_Center=0;
	else BEGE_G_Center = BEGE_G_Radius + BEGE_G_Spacing + BEGE_G_ExtGroundWidth-1.0;

	double radii = sqrt((double)((nx - BEGE_G_Center)*(nx - BEGE_G_Center) + (ny - BEGE_G_Center)*(ny - BEGE_G_Center)));
	double rEdge=(double)BEGE_G_EdgeRadius/(double)BEGE_G_EdgeHeight*(double)(BEGE_G_Height-nz) + (double)(BEGE_G_Radius-BEGE_G_EdgeRadius);

	if (BEGE_G_EdgeHeight==0) printf ("\nERROR in SIMION_CalcPoint_BEGE: BEGE_G_EdgeHeight cannot be equals to zero!\n");

	double dlEdge = (double)BEGE_G_EdgeRadius/(double)BEGE_G_EdgeHeight*(double)(BEGE_G_Height-nz) + (double)(BEGE_G_Radius-BEGE_G_EdgeRadius) - (double)BEGE_G_SurfaceContactDepth/(double)BEGE_G_EdgeHeight*sqrt((double)(BEGE_G_EdgeHeight*BEGE_G_EdgeHeight+BEGE_G_EdgeRadius*BEGE_G_EdgeRadius));

	//point is inside detector volume
	if(radii < rEdge && radii < BEGE_G_Radius && nz < BEGE_G_Height + BEGE_G_Spacing + BEGE_G_ExtGroundWidth && nz >= BEGE_G_Spacing + BEGE_G_ExtGroundWidth){
		
		//point is in the groove (or passivation layer)
		if(radii < BEGE_G_GrooveInnerRadius + BEGE_G_GrooveWidth && radii >= BEGE_G_GrooveInnerRadius && nz < BEGE_G_GrooveDepth+BEGE_G_Spacing + BEGE_G_ExtGroundWidth){
			
			//point is in the groove
			if (radii < BEGE_G_GrooveInnerRadius + BEGE_G_GrooveWidth - BEGE_G_PasLayThickness && radii >= BEGE_G_GrooveInnerRadius + BEGE_G_PasLayThickness && nz < BEGE_G_GrooveDepth + BEGE_G_Spacing + BEGE_G_ExtGroundWidth - BEGE_G_PasLayThickness) return GROOVE;
			//point is in the passivation layer
			else return P_LAY;
		}
		//inner contact (point contact)
		else if(radii < BEGE_G_PointContactRadius && nz < BEGE_G_PointContactDepth + BEGE_G_Spacing + BEGE_G_ExtGroundWidth){
			if (i==0) return V_CONT;
			else return Z_CONT;
		}
		//outer contact (surface contact)
		else if(nz >= BEGE_G_Height - BEGE_G_SurfaceContactDepth + BEGE_G_Spacing + BEGE_G_ExtGroundWidth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		else if (radii >= dlEdge || radii >= BEGE_G_Radius - BEGE_G_SurfaceContactDepth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		else if(radii >= BEGE_G_GrooveInnerRadius + BEGE_G_GrooveWidth && nz < BEGE_G_SurfaceContactDepth + BEGE_G_Spacing + BEGE_G_ExtGroundWidth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		else return BULK;
	}
	else if(radii >= BEGE_G_Radius + BEGE_G_Spacing || nz < BEGE_G_ExtGroundWidth || nz >= 2*BEGE_G_Spacing + BEGE_G_Height + BEGE_G_ExtGroundWidth) return EXT_GROUND;
	else return OUTSIDE;
}

double SIMION_CalcCharge_BEGE(int nx, int ny, int nz, int i){


	if(SIMION_Dimension_Bege==2) BEGE_G_Center=0;
	else BEGE_G_Center = BEGE_G_Radius + BEGE_G_Spacing + BEGE_G_ExtGroundWidth-1.0;

	if (SIMION_CalcPoint_BEGE(nx, ny, nz, i)==P_LAY) return 100;
	return BEGE_G_ImpBot+(BEGE_G_ImpTop-BEGE_G_ImpBot)*(nz-BEGE_G_Spacing-BEGE_G_ExtGroundWidth)/BEGE_G_Height;
}
