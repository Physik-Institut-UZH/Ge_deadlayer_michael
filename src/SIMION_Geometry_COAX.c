#include "SIMION_Geometry_COAX.h"


double GetCoaxGridSize(){return SIMION_GridSize_Coax;}
double GetCoaxHeight(){return SIMION_GridSize_Coax*double(COAX_G_Height);}
double GetCoaxDimension(){return SIMION_Dimension_Coax;}
double GetCoaxVoltage(){return SIMION_Voltage_Coax;}
double GetCoaxEpsScale(){return SIMION_EpsScale_Coax;}
double GetCoaxEpsExtScale(){return SIMION_EpsExtScale_Coax;}
char* GetCoaxDescription(){return SIMION_Description_Coax;}

void InitializeCOAX(){

//setup geometry

  COAX_G_Radius              = 310;
  COAX_G_Height              = 310; 
  COAX_G_BorHoleRadius       = 100;
  COAX_G_BorHoleDepth        = 100;
  COAX_G_CenContactRadius    = 75;
  COAX_G_CenContactDepth     = 10;
  COAX_G_OutContactRadius    = 10;
  COAX_G_OutContactDepth     = 10;
  COAX_G_GrooveDepth         = 30;
  COAX_G_GrooveWidth         = 30;
  COAX_G_GrooveInnerRadius   = 75;
  COAX_G_PasLayThickness     = 10;
  COAX_G_EdgeRadius          = 0;
  COAX_G_EdgeHeight          = 1;
  COAX_G_Spacing             = 0;
  COAX_G_ExtGroundWidth      = 0;
  COAX_G_ImpTop               = 1.0;
  COAX_G_ImpBot               = 1.0;
  COAX_G_Center               = 0.0;
}

int SIMION_Setup_GEOMETRY_COAX(char *filename_setupfile) {

  InitializeCOAX();
  InitializeArray(SIMION_GridSize_Coax,SIMION_Dimension_Coax,SIMION_Voltage_Coax,SIMION_EpsScale_Coax,SIMION_EpsExtScale_Coax,SIMION_Description_Coax);

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
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_Coax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Dimension")==0) 
				sscanf(Kwords[2+i]->svalue,"%i",&SIMION_Dimension_Coax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Voltage")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Voltage_Coax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_Coax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsExtScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsExtScale_Coax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Description")==0) 
				sscanf(Kwords[2+i]->svalue,"%s",SIMION_Description_Coax);
		}
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_Radius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_Radius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_Height")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_Height=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_BorHoleDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_BorHoleDepth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_BorHoleRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_BorHoleRadius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_CenContactDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_CenContactDepth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_CenContactRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_CenContactRadius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_OutContactDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_OutContactDepth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_OutContactRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_OutContactRadius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_GrooveInnerRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_GrooveInnerRadius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_GrooveDepth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_GrooveDepth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_GrooveWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_GrooveWidth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_PasLayThickness")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_PasLayThickness=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_EdgeRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_EdgeRadius=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_EdgeHeight")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_EdgeHeight=SIMION_grid(temp,SIMION_GridSize_Coax);
				if (COAX_G_EdgeHeight<1) COAX_G_EdgeHeight=1;
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_ExtGroundWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_ExtGroundWidth=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_Spacing")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				COAX_G_Spacing=SIMION_grid(temp,SIMION_GridSize_Coax);
			}
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_ImpTop")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&COAX_G_ImpTop);
			if (strcmp(Kwords[2+i]->keyword,"COAX_G_ImpBot")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&COAX_G_ImpBot);
		}
	}

	if(SIMION_Dimension_Coax==2) COAX_G_Center=0;
	else COAX_G_Center = COAX_G_Radius + COAX_G_Spacing + COAX_G_ExtGroundWidth-1.0;

	if (COAX_G_CenContactRadius > COAX_G_GrooveInnerRadius) printf("\nWARNING in SIMION_Setup_GEOMETRY_COAX: Wrong geometry: COAX_G_GrooveInnerRadius > COAX_G_CenContactRadius\n");
	if (COAX_G_OutContactRadius < COAX_G_GrooveInnerRadius + COAX_G_GrooveWidth) printf("\nWARNING in SIMION_Setup_GEOMETRY_COAX: Wrong geometry: COAX_G_OutContactRadius < COAX_G_GrooveInnerRadius + COAX_G_GrooveWidth\n");
	//if (COAX_G_BorHoleRadius > COAX_G_GrooveInnerRadius) printf("\nWARINING: COAX_G_BorHoleRadius > COAX_G_GrooveInnerRadius\n");

	return err;
}

int SIMION_Status_GEOMETRY_COAX(void){
	//print final values that are setup:
	printf("\nStatus SIMION_Geometry_COAX:\n");
	printf("---------------------\n");
	printf("SIMION_G_GridSize          = %lf\n",SIMION_GridSize_Coax);
	printf("SIMION_G_EpsScale          = %lf\n",SIMION_EpsScale_Coax);
	printf("SIMION_G_EpsExtScale       = %lf\n",SIMION_EpsExtScale_Coax);
	printf("SIMION_G_Description       = %s \n",SIMION_Description_Coax);
	printf("SIMION_G_Dimension         = %i \n",SIMION_Dimension_Coax);
	printf("SIMION_G_Voltage           = %lf\n",SIMION_Voltage_Coax);
	printf("---------------------\n");
	printf("COAX_G_Radius              = %ld\n",COAX_G_Radius);
	printf("COAX_G_Height              = %ld\n",COAX_G_Height);
	printf("COAX_G_BorHoleDepth        = %ld\n",COAX_G_BorHoleDepth);
	printf("COAX_G_BorHoleRadius       = %ld\n",COAX_G_BorHoleRadius);
	printf("COAX_G_CenContactRadius    = %ld\n",COAX_G_CenContactRadius);
	printf("COAX_G_CenContactDepth     = %ld\n",COAX_G_CenContactDepth);
	printf("COAX_G_OutContactRadius    = %ld\n",COAX_G_OutContactRadius);
	printf("COAX_G_OutContactDepth     = %ld\n",COAX_G_OutContactDepth);
	printf("COAX_G_GrooveInnerRadius   = %ld\n",COAX_G_GrooveInnerRadius);
	printf("COAX_G_GrooveDepth         = %ld\n",COAX_G_GrooveDepth);
	printf("COAX_G_GrooveWidth         = %ld\n",COAX_G_GrooveWidth);
	printf("COAX_G_PasLayThickness     = %ld\n",COAX_G_PasLayThickness);
	printf("COAX_G_EdgeRadius          = %ld\n",COAX_G_EdgeRadius);
	printf("COAX_G_EdgeHeight          = %ld\n",COAX_G_EdgeHeight);
	printf("COAX_G_Spacing             = %ld\n",COAX_G_Spacing);
	printf("COAX_G_ExtGroundWidth      = %ld\n",COAX_G_ExtGroundWidth);
	printf("COAX_G_ImpTop              = %lf\n",COAX_G_ImpTop);
	printf("COAX_G_ImpBot              = %lf\n",COAX_G_ImpBot);

	return 0;
}

struct SIMION_PA *SIMION_newPA_COAX(void){
	//adding 0.5 for correct rounding
	
	int npot=3;
	
	int NX=1;
	int NY=1;
	int NZ=(COAX_G_Height+2.0*COAX_G_Spacing+2.0*COAX_G_ExtGroundWidth);
	if (SIMION_Dimension_Coax==2){
		NX=(COAX_G_Radius+COAX_G_Spacing+COAX_G_ExtGroundWidth);
		NY=1;
		}
	else{
		NX=2.0*(COAX_G_Radius+COAX_G_Spacing+COAX_G_ExtGroundWidth)-1.0;
		NY=2.0*(COAX_G_Radius+COAX_G_Spacing+COAX_G_ExtGroundWidth)-1.0;
	}

	struct SIMION_PA * pa = new_SIMION_PA(SIMION_Description_Coax, npot, SIMION_GridSize_Coax, NX, NY, NZ);

	return pa;
}

int SIMION_CalcPoint_COAX(int nx, int ny, int nz, int i){ // get the radii

	if (i!=1 && i!=0){
		printf("\nWARNING in SIMION_CalcPoint_COAX: Wrong contact number %d!\n",i);
	}

	if(SIMION_Dimension_Coax==2) COAX_G_Center=0;
	else COAX_G_Center = COAX_G_Radius + COAX_G_Spacing + COAX_G_ExtGroundWidth-1.0;

	double radii = sqrt((double)((nx - COAX_G_Center)*(nx - COAX_G_Center) + (ny - COAX_G_Center)*(ny - COAX_G_Center)));
	double rEdge=(double)(COAX_G_Radius-COAX_G_EdgeRadius)/(double)COAX_G_EdgeHeight*(double)(nz-COAX_G_Spacing-COAX_G_ExtGroundWidth) + (double)(COAX_G_EdgeRadius);
	
	if (COAX_G_EdgeHeight==0) printf ("\nERROR: COAX_G_EdgeHeight cannot be equals to zero!\n");
	
	double dlEdge = rEdge-(double)COAX_G_OutContactDepth/(double)COAX_G_EdgeHeight*sqrt((double)(COAX_G_EdgeHeight*COAX_G_EdgeHeight+(COAX_G_Radius-COAX_G_EdgeRadius)*(COAX_G_Radius-COAX_G_EdgeRadius)));

	//Point is inside detector volume
	if(radii < rEdge && radii < COAX_G_Radius && nz < COAX_G_Height + COAX_G_Spacing + COAX_G_ExtGroundWidth && nz >= COAX_G_Spacing + COAX_G_ExtGroundWidth){

		//Point is in the groove (or passivation layer)
		if(radii < COAX_G_GrooveInnerRadius + COAX_G_GrooveWidth && radii >= COAX_G_GrooveInnerRadius && nz < COAX_G_GrooveDepth+COAX_G_Spacing + COAX_G_ExtGroundWidth){
			
			//Point is in the groove
			if (radii < COAX_G_GrooveInnerRadius + COAX_G_GrooveWidth - COAX_G_PasLayThickness && radii >= COAX_G_GrooveInnerRadius + COAX_G_PasLayThickness && nz < COAX_G_GrooveDepth + COAX_G_Spacing + COAX_G_ExtGroundWidth - COAX_G_PasLayThickness) return GROOVE;
			//Point is in the passivation layer
			else return P_LAY;
		}

		//Point is in the bore hole or central contact
		else if(radii < COAX_G_BorHoleRadius + COAX_G_CenContactDepth && nz < COAX_G_Spacing + COAX_G_ExtGroundWidth + COAX_G_BorHoleDepth + COAX_G_CenContactDepth){
			//Point is in the bore hole
			if(radii < COAX_G_BorHoleRadius && nz < COAX_G_Spacing + COAX_G_ExtGroundWidth + COAX_G_BorHoleDepth) return OUTSIDE;
			//Point is central contact
			else {
				if (i==0) return V_CONT;
				else return Z_CONT;
			}
		}
		//wrap around of central contact
		else if(radii < COAX_G_CenContactRadius && nz < COAX_G_CenContactDepth + COAX_G_Spacing + COAX_G_ExtGroundWidth){
			if (i==0) return V_CONT;
			else return Z_CONT;
		}
		//outer contact (Out contact)
		else if(nz >= COAX_G_Height - COAX_G_OutContactDepth + COAX_G_Spacing + COAX_G_ExtGroundWidth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		else if (radii >= dlEdge || radii >= COAX_G_Radius - COAX_G_OutContactDepth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		else if(radii >= COAX_G_OutContactRadius && nz < COAX_G_OutContactDepth + COAX_G_Spacing + COAX_G_ExtGroundWidth){
			if (i==1) return V_CONT;
			else return Z_CONT;
		}
		
		else return BULK;
	}
	else if(radii >= COAX_G_Radius + COAX_G_Spacing || nz < COAX_G_ExtGroundWidth || nz >= 2*COAX_G_Spacing + COAX_G_Height + COAX_G_ExtGroundWidth) return EXT_GROUND;
	else return OUTSIDE;
}

double SIMION_CalcCharge_COAX(int nx, int ny, int nz, int i){

	if(SIMION_Dimension_Coax==2) COAX_G_Center=0;
	else COAX_G_Center = COAX_G_Radius + COAX_G_Spacing + COAX_G_ExtGroundWidth-1.0;

	return COAX_G_ImpBot+(COAX_G_ImpTop-COAX_G_ImpBot)*(nz-COAX_G_ExtGroundWidth-COAX_G_Spacing)/COAX_G_Height;
}
