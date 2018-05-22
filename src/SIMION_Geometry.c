/*
 * AGATA Detector simulation Library (SIMION)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK Heidelberg
 * http://www.mpi-hd.mpg.de/gerda
 *
 */

#include "SIMION_Geometry.h"

//struct SIMION_PA *(*SIMION_G_newPA)(void)          = SIMION_newPA_PLANAR;

//int InitializeGeometry(int (*Setup_GEOMETRY)(char *),
//                       int (*Status_GEOMETRY)(),
//                       int (*CalcPoint)(int, int, int, int),
//                       double (*CalcCharge)(int, int, int, int)/*,
//                       struct SIMION_PA *(*newPA)(void)*/){

void SetVoltage(double voltage){SIMION_Voltage_Geom = voltage;}
double GetVoltage(){return SIMION_Voltage_Geom;}

int InitializeGeometry(){

    SIMION_G_Setup_GEOMETRY  = SIMION_Setup_GEOMETRY_PLANAR;
    SIMION_G_Status_GEOMETRY = SIMION_Status_GEOMETRY_PLANAR;
    SIMION_G_CalcPoint       = SIMION_CalcPoint_PLANAR;
    SIMION_G_CalcCharge      = SIMION_CalcCharge_PLANAR;
    SIMION_G_newPA           = SIMION_newPA_PLANAR;
                           
    return 0;
}

int SetupGeometry(char *libversion, char *setupfile){return SIMION_Setup_GEOMETRY(libversion,setupfile);}

struct SIMION_PA * GetSIMIONnewPA(){return SIMION_G_newPA();}
void GetSIMIONStructure(struct SIMION_PA *field,int i){SIMION_G_Structure(field, i);}
void GetSIMIONWfield(struct SIMION_PA *field,int i){SIMION_G_WField(field, i);}
void GetSIMIONEfield(struct SIMION_PA *field,int i){SIMION_G_EField(field, i);}

// set function pointers to right version:
static void SIMION_Set_F_GEOMETRY(char *libversion) 
{
  if (strcmp(libversion,"SIMION_GEOMETRY_PLANAR")==0) {
    SIMION_G_Setup_GEOMETRY  = SIMION_Setup_GEOMETRY_PLANAR;
    SIMION_G_Status_GEOMETRY = SIMION_Status_GEOMETRY_PLANAR;
    SIMION_G_CalcPoint       = SIMION_CalcPoint_PLANAR;
    SIMION_G_CalcCharge      = SIMION_CalcCharge_PLANAR;
    SIMION_G_newPA           = SIMION_newPA_PLANAR;
    return;
  }
  if (strcmp(libversion,"SIMION_GEOMETRY_BEGE")==0) {
    SIMION_G_Setup_GEOMETRY  = SIMION_Setup_GEOMETRY_BEGE;
    SIMION_G_Status_GEOMETRY = SIMION_Status_GEOMETRY_BEGE;
    SIMION_G_CalcPoint       = SIMION_CalcPoint_BEGE;
    SIMION_G_CalcCharge      = SIMION_CalcCharge_BEGE;
    SIMION_G_newPA           = SIMION_newPA_BEGE;
    return;
  }
  if (strcmp(libversion,"SIMION_GEOMETRY_ICOAX")==0) {
    SIMION_G_Setup_GEOMETRY  = SIMION_Setup_GEOMETRY_ICOAX;
    SIMION_G_Status_GEOMETRY = SIMION_Status_GEOMETRY_ICOAX;
    SIMION_G_CalcPoint       = SIMION_CalcPoint_ICOAX;
    SIMION_G_CalcCharge      = SIMION_CalcCharge_ICOAX;
    SIMION_G_newPA           = SIMION_newPA_ICOAX;
    return;
  }
  if (strcmp(libversion,"SIMION_GEOMETRY_COAX")==0) {
    SIMION_G_Setup_GEOMETRY  = SIMION_Setup_GEOMETRY_COAX;
    SIMION_G_Status_GEOMETRY = SIMION_Status_GEOMETRY_COAX;
    SIMION_G_CalcPoint       = SIMION_CalcPoint_COAX;
    SIMION_G_CalcCharge      = SIMION_CalcCharge_COAX;
    SIMION_G_newPA           = SIMION_newPA_COAX;
    return;
  }
  // if you get here, libversion is not recognized:
  if (SIMION_G_DEBUG) printf("\nERROR: SIMION GEOMETRY - lib %s not recognized\n",libversion);
  if (SIMION_G_DEBUG) printf("       valid versions are: SIMION_GEOMETRY_PLANAR SIMION_GEOMETRY_COAX SIMION_GEOMETRY_BEGE SIMION_GEOMETRY_ICOAX\n",libversion);
	}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int SIMION_Setup_GEOMETRY(char *libversion, char *setupfile){
    
//    InitializeGeometry(SIMION_G_Setup_GEOMETRY,SIMION_G_Status_GEOMETRY,SIMION_G_CalcPoint,SIMION_G_CalcCharge/*,SIMION_G_newPA*/);
    
    InitializeGeometry();
    
    if (strcmp(libversion,"SIMION_GEOMETRY_PLANAR")==0) {
      SIMION_Set_F_GEOMETRY(libversion);
      SIMION_Voltage_Geom = SIMION_Voltage_Planar;
      SIMION_EpsExtScale_Geom = SIMION_EpsExtScale_Planar;
      SIMION_EpsExtScale_Geom = SIMION_EpsScale_Planar;
      SIMION_Description_Geom = SIMION_Description_Planar;
      return SIMION_Setup_GEOMETRY_PLANAR(setupfile);
    }
    if (strcmp(libversion,"SIMION_GEOMETRY_BEGE")==0) {
      SIMION_Set_F_GEOMETRY(libversion);
      SIMION_Voltage_Geom = SIMION_Voltage_Bege;
      SIMION_EpsExtScale_Geom = SIMION_EpsExtScale_Bege;
      SIMION_EpsExtScale_Geom = SIMION_EpsScale_Bege;
      SIMION_Description_Geom = SIMION_Description_Bege;
      return SIMION_Setup_GEOMETRY_BEGE(setupfile);
    }
    if (strcmp(libversion,"SIMION_GEOMETRY_ICOAX")==0) {
      SIMION_Set_F_GEOMETRY(libversion);
      SIMION_Voltage_Geom = SIMION_Voltage_ICoax;
      SIMION_EpsExtScale_Geom = SIMION_EpsExtScale_ICoax;
      SIMION_EpsExtScale_Geom = SIMION_EpsScale_ICoax;
      SIMION_Description_Geom = SIMION_Description_ICoax;
      return SIMION_Setup_GEOMETRY_ICOAX(setupfile);
    }
    if (strcmp(libversion,"SIMION_GEOMETRY_COAX")==0) {
      SIMION_Set_F_GEOMETRY(libversion);
      SIMION_Voltage_Geom = SIMION_Voltage_Coax;
      SIMION_EpsExtScale_Geom = SIMION_EpsExtScale_Coax;
      SIMION_EpsExtScale_Geom = SIMION_EpsScale_Coax;
      SIMION_Description_Geom = SIMION_Description_Coax;
      return SIMION_Setup_GEOMETRY_COAX(setupfile);
    }
    // if you get here, libversion is not recognized:
    if (SIMION_G_DEBUG) printf("\nERROR: SIMION Setup GEOMETRY - lib %s not recognized\n",libversion);
    if (SIMION_G_DEBUG) printf("       valid versions are: SIMION_GEOMETRY_PLANAR \n",libversion);
    return 1;
}

int SIMION_Status_GEOMETRY(void){
  //print final values that are setup:
  printf("\nStatus GEOMETRY:\n");
  printf("------------\n");
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_PLANAR)  printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_PLANAR\n");
  if (SIMION_G_Status_GEOMETRY == SIMION_Status_GEOMETRY_PLANAR) printf("SIMION_G_Status_GEOMETRY -> SIMION_Status_GEOMETRY_PLANAR\n");
  if (SIMION_G_CalcPoint       == SIMION_CalcPoint_PLANAR)       printf("SIMION_G_CalcPoint       -> SIMION_CalcPoint_PLANAR\n");
  if (SIMION_G_CalcCharge      == SIMION_CalcCharge_PLANAR)      printf("SIMION_G_CalcCharge      -> SIMION_CalcCharge_PLANAR\n");
  if (SIMION_G_newPA           == SIMION_newPA_PLANAR)           printf("SIMION_G_newPA           -> SIMION_newPA_PLANAR\n");
  
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_BEGE)  printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_BEGE\n");
  if (SIMION_G_Status_GEOMETRY == SIMION_Status_GEOMETRY_BEGE) printf("SIMION_G_Status_GEOMETRY -> SIMION_Status_GEOMETRY_BEGE\n");
  if (SIMION_G_CalcPoint       == SIMION_CalcPoint_BEGE)       printf("SIMION_G_CalcPoint       -> SIMION_CalcPoint_BEGE\n");
  if (SIMION_G_CalcCharge      == SIMION_CalcCharge_BEGE)      printf("SIMION_G_CalcCharge      -> SIMION_CalcCharge_BEGE\n");
  if (SIMION_G_newPA           == SIMION_newPA_BEGE)           printf("SIMION_G_newPA           -> SIMION_newPA_BEGE\n");
  
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_ICOAX){
    printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_ICOAX\n");
    SIMION_Height  = GetICoaxHeight();
    SIMION_Voltage_Geom  = GetICoaxVoltage();
    SIMION_EpsScale_Geom  = GetICoaxEpsScale();
    SIMION_EpsExtScale_Geom  = GetICoaxEpsExtScale();
    SIMION_Description_Geom  = GetICoaxDescription();
  }
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_BEGE){
    printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_BEGE\n");
    SIMION_Height  = GetBegeHeight();
    SIMION_Voltage_Geom  = GetBegeVoltage();
    SIMION_EpsScale_Geom  = GetBegeEpsScale();
    SIMION_EpsExtScale_Geom  = GetBegeEpsExtScale();
    SIMION_Description_Geom  = GetBegeDescription();
  }
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_COAX){
    printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_COAX\n");
    SIMION_Height  = GetCoaxHeight();
    SIMION_Voltage_Geom  = GetCoaxVoltage();
    SIMION_EpsScale_Geom  = GetCoaxEpsScale();
    SIMION_EpsExtScale_Geom  = GetCoaxEpsExtScale();
    SIMION_Description_Geom  = GetCoaxDescription();
  }
  if (SIMION_G_Status_GEOMETRY == SIMION_Status_GEOMETRY_ICOAX) printf("SIMION_G_Status_GEOMETRY -> SIMION_Status_GEOMETRY_ICOAX\n");
  if (SIMION_G_CalcPoint       == SIMION_CalcPoint_ICOAX)       printf("SIMION_G_CalcPoint       -> SIMION_CalcPoint_ICOAX\n");
  if (SIMION_G_CalcCharge      == SIMION_CalcCharge_ICOAX)      printf("SIMION_G_CalcCharge      -> SIMION_CalcCharge_ICOAX\n");
  if (SIMION_G_newPA           == SIMION_newPA_ICOAX)           printf("SIMION_G_newPA           -> SIMION_newPA_ICOAX\n");
  
  if (SIMION_G_Setup_GEOMETRY  == SIMION_Setup_GEOMETRY_COAX)  printf("SIMION_G_Setup_GEOMETRY  -> SIMION_Setup_GEOMETRY_COAX\n");
  if (SIMION_G_Status_GEOMETRY == SIMION_Status_GEOMETRY_COAX) printf("SIMION_G_Status_GEOMETRY -> SIMION_Status_GEOMETRY_COAX\n");
  if (SIMION_G_CalcPoint       == SIMION_CalcPoint_COAX)       printf("SIMION_G_CalcPoint       -> SIMION_CalcPoint_COAX\n");
  if (SIMION_G_CalcCharge      == SIMION_CalcCharge_COAX)      printf("SIMION_G_CalcCharge      -> SIMION_CalcCharge_COAX\n");
  if (SIMION_G_newPA           == SIMION_newPA_COAX)           printf("SIMION_G_newPA           -> SIMION_newPA_COAX\n");
  
  SIMION_G_Status_GEOMETRY();
  
  return 0;
}

//++++++++++++++++++++++++++++++++++++++++++
// GEOMETRY DEFINITON FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

int SIMION_G_Structure(struct SIMION_PA *pa, int contact){

	struct SIMION_HEADER *hh= &(pa->h);

	//change the geometry, this is important
	SIMION_Geometry(pa);

	int x, y, z;

	for(x = 0; x < hh->nx; x++){
		for(y = 0; y < hh->ny; y++){
			for(z = 0; z < hh->nz; z++){
				pa->Pot[0][SIMION_node(hh,x,y,z)]=SIMION_G_CalcPoint(x,y,z,contact);
			}
		}
	}

	//attempt to name them correctly
	SIMION_G_Naming(pa, "Stru", 0);
	return 0;
}

int SIMION_G_WField(struct SIMION_PA *pa, int contact){

	struct SIMION_HEADER *hh= &(pa->h);

	printf("Weighting potential size : %i %i %i \n", hh->nx,hh->ny,hh->nz);

	int x, y, z;
	double nz=0;

	//change the geometry, this is important
	SIMION_Geometry(pa);

	for(x = 0; x < hh->nx; x++){
		for(y = 0; y < hh->ny; y++){
			for(z = 0; z < hh->nz; z++){
				int geo = SIMION_G_CalcPoint(x,y,z,contact);
				if(geo == V_CONT)
					pa->Pot[0][SIMION_node(hh,x,y,z)] = 1.0;
				else if(geo == Z_CONT)
					pa->Pot[0][SIMION_node(hh,x,y,z)] = 0.0;

				if(geo == BULK || geo == V_CONT || geo == Z_CONT)
					pa->Pot[2][SIMION_node(hh,x,y,z)] = SIMION_EpsScale_Geom;
				else pa->Pot[2][SIMION_node(hh,x,y,z)] = SIMION_EpsExtScale_Geom;
				pa->Pot[1][SIMION_node(hh,x,y,z)] = 0;
			}
		}
	}

	//attempt to name them correctly
	SIMION_G_Naming(pa, "Wpot", 0);
	SIMION_G_Naming(pa, "Wspc", 1);
	SIMION_G_Naming(pa, "Weps", 2);

	return 0;
}

int SIMION_G_EField(struct SIMION_PA *pa, int contact){

	struct SIMION_HEADER *hh= &(pa->h);

	int x, y, z;

	//change the geometry, this is important
	SIMION_Geometry(pa);

	for(x = 0; x < hh->nx; x++){
		for(y = 0; y < hh->ny; y++){
			for(z = 0; z < hh->nz; z++){
				int geo = SIMION_G_CalcPoint(x,y,z,contact);
				if(geo == V_CONT)
					pa->Pot[0][SIMION_node(hh,x,y,z)] = SIMION_Voltage_Geom;
				else if(geo == Z_CONT)
					pa->Pot[0][SIMION_node(hh,x,y,z)] = 0.0;

				if(geo == BULK || geo == P_LAY){// the contacts are not depleted || geo == V_CONT || geo == Z_CONT){
					pa->Pot[1][SIMION_node(hh,x,y,z)] = SIMION_G_CalcCharge(x,y,z,contact);
					}
				else pa->Pot[1][SIMION_node(hh,x,y,z)] = 0.0;

				if(geo == BULK || geo == V_CONT || geo == Z_CONT)
					pa->Pot[2][SIMION_node(hh,x,y,z)]  = SIMION_EpsScale_Geom;
				else pa->Pot[2][SIMION_node(hh,x,y,z)] = SIMION_EpsExtScale_Geom;
			}
		}
	}

	//attempt to name them correctly
	SIMION_G_Naming(pa, "Epot", 0);
	SIMION_G_Naming(pa, "Espc", 1);
	SIMION_G_Naming(pa, "Eeps", 2);
	
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++
// NECESSARY ROUTINES DEFINITON FUNCTION	
//++++++++++++++++++++++++++++++++++++++++++

int SIMION_Geometry(struct SIMION_PA *pa){

	int x, y, z;
	struct SIMION_HEADER *hh= &(pa->h);

	for(x = 0; x < hh->nx; x++){
		for(y = 0; y < hh->ny; y++){
			for(z = 0; z < hh->nz; z++){
				int geo = SIMION_G_CalcPoint(x,y,z, 0);
				if(geo == V_CONT || geo == EXT_GROUND || geo == Z_CONT)
					pa->Geo[SIMION_node(hh,x,y,z)] = 2.0;
				else pa->Geo[SIMION_node(hh,x,y,z)] = 0.0;
			}
		}
	}

	return 0;
}

int SIMION_G_Naming(struct SIMION_PA * pa, char* label, int i){
	sprintf(pa->Name[i],"%s_%s.pa", SIMION_Description_Geom, label);
	return 0;
}

