#include "SIMION_Geometry_COAX.h"

//setup geometry
int SIMPLECOAX_G_InnerContactRadius = 10;
int SIMPLECOAX_G_InnerContactWidth  = 1;
int SIMPLECOAX_G_OuterContactWidth  = 1;
int SIMPLECOAX_G_Width              = 100;
double SIMPLECOAX_G_ImpTop              = 1;
double SIMPLECOAX_G_ImpBot              = 1;


int SIMION_Setup_GEOMETRY_COAX(char *filename_setupfile) {

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
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_SimpleCoax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Dimension")==0) 
				sscanf(Kwords[2+i]->svalue,"%i",&SIMION_Dimension_SimpleCoax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Voltage")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Voltage_SimpleCoax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_SimpleCoax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsExtScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsExtScale_SimpleCoax);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Description")==0) 
				sscanf(Kwords[2+i]->svalue,"%s",SIMION_Description_SimpleCoax);
		}
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_InnerContactRadius")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_InnerContactRadius=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_InnerContactWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_InnerContactWidth=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_OuterContactWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_OuterContactWidth=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_Width")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_Width=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_Spacing")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_Spacing=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_ExtGroundWidth")==0){ 
				sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
				SIMPLECOAX_G_ExtGroundWidth=SIMION_grid(temp,SIMION_GridSize_SimpleCoax);
			}
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_ImpTop")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMPLECOAX_G_ImpTop);
			if (strcmp(Kwords[2+i]->keyword,"SIMPLECOAX_G_ImpBot")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMPLECOAX_G_ImpBot);
		}
	}
	return err;
}

int SIMION_Status_GEOMETRY_COAX(void){
	//print final values that are setup:
	printf("\nStatus SIMION_Geometry_COAX:\n");
	printf("---------------------\n");
	printf("SIMION_G_GridSize         = %lf\n",SIMION_GridSize_SimpleCoax);
	printf("SIMION_G_EpsScale         = %lf\n",SIMION_EpsScale_SimpleCoax);
	printf("SIMION_G_EpsExtScale      = %lf\n",SIMION_EpsExtScale_SimpleCoax);
	printf("SIMION_G_Description      = %s \n",SIMION_Description_SimpleCoax);
	printf("SIMION_G_Dimension        = %i \n",SIMION_Dimension_SimpleCoax);
	printf("SIMION_G_Voltage          = %lf\n",SIMION_Voltage_SimpleCoax);
	printf("---------------------\n");
	printf("SIMPLECOAX_G_InnerContactRadius = %ld\n",SIMPLECOAX_G_InnerContactRadius);
	printf("SIMPLECOAX_G_InnerContactWidth  = %ld\n",SIMPLECOAX_G_InnerContactWidth);
	printf("SIMPLECOAX_G_OuterContactWidth  = %ld\n",SIMPLECOAX_G_OuterContactWidth);
	printf("SIMPLECOAX_G_Width              = %ld\n",SIMPLECOAX_G_Width);
	printf("SIMION_G_Spacing                = %ld\n",SIMION_G_Spacing);
	printf("SIMPLECOAX_G_ExtGroundWidth     = %ld\n",SIMPLECOAX_G_ExtGroundWidth);
	printf("SIMPLECOAX_G_ImpTop             = %lf\n",SIMPLECOAX_G_ImpTop);
	printf("SIMPLECOAX_G_ImpBot             = %lf\n",SIMPLECOAX_G_ImpBot);

	return 0;
}

struct SIMION_PA *SIMION_newPA_COAX(void){
	//adding 1.0 for taking care of indices (NX>imax) and 0.5 for correct rounding
	int npot=3;
	int NX=2.0*(SIMPLECOAX_G_ExtGroundWidth + SIMPLECOAX_G_Spacing + SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_Width + SIMPLECOAX_G_OuterContactWidth)-1.0;
 	int NY=2.0*(SIMPLECOAX_G_ExtGroundWidth + SIMION_G_Spacing + SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_Width + SIMPLECOAX_G_OuterContactWidth)-1.0;
	int NZ=3;
	struct SIMION_PA * pa = new_SIMION_PA(SIMION_Description_SimpleCoax, npot, SIMION_GridSize_SimpleCoax, NX, NY, NZ);
	return pa;
}

int SIMION_CalcPoint_COAX(int nx, int ny, int nz, int i){

	if (i!=1 && i!=0){
		printf("\nWARNING in SIMION_CalcPoint_SIMPLECOAX: Wrong contact number %d!\n",i);
	}

	// center
	int center = SIMPLECOAX_G_ExtGroundWidth + SIMION_G_Spacing + SIMPLECOAX_G_OuterContactWidth + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_InnerContactRadius-1.0;

	// get the radii
	double radii = sqrt(pow((nx - center),2) + pow((ny - center),2));

	//point is inside detector volume
	if(     radii >=  SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_OuterContactWidth + SIMION_G_Spacing) return EXT_GROUND;
	else if(radii >= SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_OuterContactWidth && radii < SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_OuterContactWidth + SIMION_G_Spacing) return OUTSIDE;
	else if(radii < SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_OuterContactWidth && radii > SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth){
		if (i==0) return V_CONT;
		else return Z_CONT;
	}
	else if(radii < SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth && radii >= SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_InnerContactWidth) return BULK; 
	else if(radii < SIMPLECOAX_G_InnerContactRadius + SIMPLECOAX_G_InnerContactWidth && radii >= SIMPLECOAX_G_InnerContactRadius){
		if (i==1) return V_CONT;
		else return Z_CONT;
	}
	else if(radii < SIMPLECOAX_G_InnerContactRadius){return OUTSIDE;}
}

double SIMION_CalcCharge_COAX(int nx, int ny, int nz, int i){

	// center
	int center = SIMPLECOAX_G_ExtGroundWidth + SIMION_G_Spacing + SIMPLECOAX_G_OuterContactWidth + SIMPLECOAX_G_Width + SIMPLECOAX_G_InnerContactWidth + SIMPLECOAX_G_InnerContactRadius-1.0;

	// get the radii
	int radii = sqrt(pow((nx - center),2) + pow((ny - center),2));
	return SIMPLECOAX_G_ImpBot+(SIMPLECOAX_G_ImpTop-SIMPLECOAX_G_ImpBot)*((radii-SIMPLECOAX_G_InnerContactWidth)/SIMPLECOAX_G_Width);
}
