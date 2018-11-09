/*
 * AGATA Detector simulation Library (ADL)
 *
 * Bart Bruyneel - bartb@ikp.uni-koeln.de
 * Benedikt Birkenbach - bene@ikp.uni-koeln.de
 *
 * IKP Cologne
 * http://www.ikp.uni-koeln.de/agata
 *
 */

#include "ADL_Fields_SIMION.h"

//allocate necessary variables:

struct SIMION_PA *ADL_G_Epot;	 //potential array containing the electrical potential
struct SIMION_PA *ADL_G_Wpot;   //potential array containing all the weighting potentials
struct SIMION_PA *ADL_G_Stru;   //potential array containing the structure of the detector setup (geometry)

double ADL_G_SIMION_SmallPot = 1e-6; //[V] Small potential, Defines e.g. In Detector

struct SIMION_PA *GetADLEpot(){return ADL_G_Epot;}
struct SIMION_PA *GetADLWpot(){return ADL_G_Wpot;}
struct SIMION_PA *GetADLStru(){return ADL_G_Stru;}

void SetADLEpot(struct SIMION_PA *ADL_Epot){ADL_G_Epot = ADL_Epot;}
void SetADLWpot(struct SIMION_PA *ADL_Wpot){ADL_G_Wpot = ADL_Wpot;}
void SetADLStru(struct SIMION_PA *ADL_Stru){ADL_G_Stru = ADL_Stru;}

double GetSimionGridSize(){return ADL_Fields_SIMION_GridSize;}
double GetSimionCenter(){return ADL_Fields_SIMION_Center;}
double GetSimionHeight(){return ADL_Fields_SIMION_Height;}

void SetSimionGridSize(double gridsize){ADL_Fields_SIMION_GridSize = gridsize;}
void SetSimionCenter(double center){ADL_Fields_SIMION_Center = center;}
void SetSimionHeight(double height){ADL_Fields_SIMION_Height = height;}

//setup***************************************************************************

void ADL_scale_PA(struct SIMION_PA *PA, int i, double scale){
  long J,Jmax= (PA->h.nx*PA->h.ny*PA->h.nz);
  for (J=0;J<Jmax;J++) PA->Pot[i][J]*=scale;
  for (J=0;J<Jmax;J++) if (fabs(PA->Pot[i][J])>PA->h.max_voltage) PA->h.max_voltage = 2*PA->Pot[i][J];
} 


int ADL_Setup_FIELDS_SIMION(char *filename_setupfile){

  int	i,j,len,err=1;
  long length = 0;
  char ToScale[100];
  double scale=1.0;
  struct	ADL_KEYWORD **Kwords;
  
  ADL_G_Epot=NULL;
  ADL_G_Wpot=NULL;
  ADL_G_Stru=NULL;
  
  ADL_Fields_SIMION_Center = 0;
  ADL_Fields_SIMION_GridSize = 0.1;    
  
  //get list of keywords from file:
  if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
    Kwords = ADL_parse_file (filename_setupfile);
		
    //overwrite keywords with parsed values:
    //first scan for gridsize!
    len = Kwords[1]->linenumber;
    err = Kwords[len+2]->linenumber;
    int debug = 0;
    for (i=0;i<len;i++) {
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_DEBUG")==0){
	sscanf(Kwords[2+i]->svalue,"%i",&debug);
	SetADLDebug(debug);
      }
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_SIMION_GridSize")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_Fields_SIMION_GridSize);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_SIMION_SmallPot")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_SIMION_SmallPot);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_GridSize")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_Fields_SIMION_GridSize);
    }
    //using actual gridsize, read in Epot and Wpot:
    for (i=0;i<len;i++) {
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_Epot")==0) {
	if (ADL_G_Epot>(void*)NULL) free(ADL_G_Epot);
	if(GetADLDebug()) printf("\n**** Reading Electric Potential Arrays ****\n");
	ADL_G_Epot=SIMION_READ(Kwords[2+i]->svalue,ADL_Fields_SIMION_GridSize);
      }
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_Wpot")==0) {
	if (ADL_G_Wpot>(void*)NULL) free(ADL_G_Wpot);
	if(GetADLDebug()) printf("\n**** Reading Weighting Potential Arrays ****\n");
	ADL_G_Wpot=SIMION_READ(Kwords[2+i]->svalue,ADL_Fields_SIMION_GridSize);
      }
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_Stru")==0) {
	if (ADL_G_Stru>(void*)NULL) free(ADL_G_Stru);
	if(GetADLDebug()) printf("\n**** Reading Structure Potential Arrays ****\n");
	ADL_G_Stru=SIMION_READ(Kwords[2+i]->svalue,ADL_Fields_SIMION_GridSize);
      }
    }

    //At this point ADL_G_Epot and ADL_G_Wpot must be defined:
    if (ADL_G_Epot==(void*)NULL) {printf("\nERROR SETUP FIELDS SIMION: ADL_G_Epot undefined\n"); return 0;}
    if (ADL_G_Wpot==(void*)NULL) {printf("\nERROR SETUP FIELDS SIMION: ADL_G_Wpot undefined\n"); return 0;}
    if (ADL_G_Stru==(void*)NULL) {printf("\nWARNING SETUP FIELDS SIMION: ADL_G_Stru undefined\n");}

    //scan now for scaling factors for Epot:
    if(GetADLDebug()) printf("\n**** Scaling electrical potential arrays ****\n");
    for (i=0;i<len;i++) {
      for (j=0;j<ADL_G_Epot->n;j++){
	sprintf(ToScale,"ADL_SCALE_%d",j);
	if (strcmp(Kwords[2+i]->keyword,ToScale)==0) {
	  sscanf(Kwords[2+i]->svalue,"%lf",&scale);
	  ADL_scale_PA(ADL_G_Epot,j,scale);
	}
      }
    }
		
    //Finally: sum all partial Epotentials and release the potentials n>0:
    if(GetADLDebug()) printf("\nSumming all %d partial Efields in ADL_G_Epot...",ADL_G_Epot->n);
    length = (ADL_G_Epot->h.nx*ADL_G_Epot->h.ny*ADL_G_Epot->h.nz);
    for (i=1;i<ADL_G_Epot->n;i++)
      for (j=0;j<length;j++) ADL_G_Epot->Pot[0][j]+=ADL_G_Epot->Pot[i][j];
    for (i=1;i<ADL_G_Epot->n;i++) free(ADL_G_Epot->Pot[i]);
    ADL_G_Epot->n = 1;
    if(GetADLDebug()) printf("done\n");
		
    //test if ADL_G_Stru is defined, if so use this information
    if(ADL_G_Stru)  ADL_Fields_SIMION_Height = ADL_G_Stru->GridSize*ADL_G_Stru->h.nz;
    else return err;

    //now we define the center as long as it makes sense
    if ((ADL_G_Stru->h.nx > 2) && (ADL_G_Stru->h.ny == 1) && (ADL_G_Stru->h.nz > 2))
      ADL_Fields_SIMION_Center=(ADL_G_Stru->h.nx-1.0)*ADL_Fields_SIMION_GridSize;
    else if ((ADL_G_Stru->h.nx > 2) && (ADL_G_Stru->h.nz > 2) && (ADL_G_Stru->h.nx == ADL_G_Stru->h.ny))
      ADL_Fields_SIMION_Center=(ADL_G_Stru->h.nx-1.0)*ADL_Fields_SIMION_GridSize*0.5;

    //Check consistency: is detector depleted?
    //not implemented here.... could be done in setup
    // by looking for a small field inside the detector volume...
  }

  if (GetADLDebug()) ADL_Status_FIELDS_SIMION();
  return err;
}


int ADL_Status_FIELDS_SIMION(void){
  //print final values that are setup:

  printf("\nStatus FIELDS_SIMION:\n");
  printf("---------------------\n");
  printf("ADL_G_SIMION_GridSize   = %lf\n",ADL_Fields_SIMION_GridSize);
  printf("ADL_G_SIMION_SmallPot   = %lf\n",ADL_G_SIMION_SmallPot);
  if (ADL_G_Epot == NULL) 
    printf("WARNING! >>> ADL_G_Epot = NULL\n"); 
  else 
    printf("ADL_G_Epot              = %s ...n=%d\n",ADL_G_Epot->Name[0],ADL_G_Epot->n);
  if (ADL_G_Wpot == NULL) 
    printf("WARNING! >>> ADL_G_Wpot = NULL\n"); 
  else 
    printf("ADL_G_Wpot              = %s ...n=%d\n",ADL_G_Wpot->Name[0],ADL_G_Wpot->n);
  if (ADL_G_Stru == NULL) 
    printf("WARNING! >>> ADL_G_Stru = NULL\n"); 
  else
    printf("ADL_G_Stru              = %s ...n=%d\n",ADL_G_Stru->Name[0],ADL_G_Stru->n);


  if (ADL_G_Epot && ADL_G_Wpot) //both pass exist..
    if (  (ADL_G_Epot->h.mode     != ADL_G_Wpot->h.mode    )
	  ||(ADL_G_Epot->h.symmetry != ADL_G_Wpot->h.symmetry)
	  ||(ADL_G_Epot->h.mirror   != ADL_G_Wpot->h.mirror  )
	  ||(ADL_G_Epot->h.nx       != ADL_G_Wpot->h.nx      )
	  ||(ADL_G_Epot->h.ny       != ADL_G_Wpot->h.ny      )
	  ||(ADL_G_Epot->h.nz       != ADL_G_Wpot->h.nz      )
	  ||(ADL_G_Epot->GridSize   != ADL_G_Wpot->GridSize  )  ){
      printf("\nWARNING - FIELDS SIMION: ADL_G_Epot and ADL_G_Wpot have different structure!\n");
    }

  if (ADL_G_Stru) //exists
    {
      if(ADL_G_Epot)
	if (  (ADL_G_Epot->h.mode     != ADL_G_Stru->h.mode    )
	      ||(ADL_G_Epot->h.symmetry != ADL_G_Stru->h.symmetry)
	      ||(ADL_G_Epot->h.mirror   != ADL_G_Stru->h.mirror  )
	      ||(ADL_G_Epot->h.nx       != ADL_G_Stru->h.nx      )
	      ||(ADL_G_Epot->h.ny       != ADL_G_Stru->h.ny      )
	      ||(ADL_G_Epot->h.nz       != ADL_G_Stru->h.nz      )
	      ||(ADL_G_Epot->GridSize   != ADL_G_Stru->GridSize  )  ){
	  printf("\nWARNING - FIELDS SIMION: ADL_G_Stru and ADL_G_Epot have different structure!\n");
	}
      if(ADL_G_Wpot)
	if (  (ADL_G_Wpot->h.mode     != ADL_G_Stru->h.mode    )
	      ||(ADL_G_Wpot->h.symmetry != ADL_G_Stru->h.symmetry)
	      ||(ADL_G_Wpot->h.mirror   != ADL_G_Stru->h.mirror  )
	      ||(ADL_G_Wpot->h.nx       != ADL_G_Stru->h.nx      )
	      ||(ADL_G_Wpot->h.ny       != ADL_G_Stru->h.ny      )
	      ||(ADL_G_Wpot->h.nz       != ADL_G_Stru->h.nz      )
	      ||(ADL_G_Wpot->GridSize   != ADL_G_Stru->GridSize  )  ){
	  printf("\nWARNING - FIELDS SIMION: ADL_G_Stru and ADL_G_Wpot have different structure!\n");
	}
    }

  if (ADL_G_Wpot) 
    if (GetNSEG() != ADL_G_Wpot->n) 
      printf("WARNING: nSeg = %d, but %d weightingfields defined!\n",GetNSEG(),ADL_G_Wpot->n);
  return 0;
}

//*******************************************************************************************

static	double SIMION_Potential(double *point, struct SIMION_HEADER *h, double GridSize, double dx, double dy, double dz){
  //calculates the potential at coordinate (X,Y,Z) by interpolation
  long i,j,k;
  double schatting;

  if ((h->nx > 2) && (h->ny == 1) && (h->nz > 2))
    {
      //transform position into gridunits for 2D cylindric array
      double center=(h->nx-1.0)*GridSize;
      dx = (sqrt((dx-center)*(dx-center)+(dy-center)*(dy-center))/GridSize); i = (long)floor(dx);
      dy = 0; j = 0;
      dz = (dz/GridSize); k = (long)floor(dz);

      // if point outside matrix : potential unknown : return 0;
      if ((dx>=(h->nx-1))||(dz>=(h->nz-1))||(dx<0)||(dz<0)) { return 0.0; }

      //linear interpolation within the cube of surrounding nodes:
      double alfa = dx - floor(dx);
      double gamma = dz - floor(dz);
      schatting  = (1.0-alfa)*(1.0-gamma)*point[SIMION_node(h,i,j,k)];
      schatting += alfa      *(1.0-gamma)*point[SIMION_node(h,(i+1),j,k)];
      schatting += (1.0-alfa)*gamma      *point[SIMION_node(h,i,j,(k+1))];
      schatting += alfa      *gamma      *point[SIMION_node(h,(i+1),j,(k+1))];

      return schatting;
    }
  else if ((h->nx > 2)||(h->ny > 2)||(h->nz > 2)) 
    {
      //transform position into gridunits for 3D array
      dx = (dx/GridSize); i = (long)floor(dx);
      dy = (dy/GridSize); j = (long)floor(dy);
      dz = (dz/GridSize); k = (long)floor(dz);

      // if point outside matrix : potential unknown : return 0;
      if ((dx>=(h->nx-1))||(dy>=(h->ny-1))||(dz>=(h->nz-1))||(dx<0)||(dy<0)||(dz<0)) { return 0.0; }

      //linear interpolation within the cube of surrounding nodes:
      double alfa = dx - floor(dx);
      double beta = dy - floor(dy);
      double gamma = dz - floor(dz);
      schatting  = (1.0-alfa)*(1.0-beta)*(1.0-gamma)*point[SIMION_node(h,i,j,k)];
      schatting += alfa      *(1.0-beta)*(1.0-gamma)*point[SIMION_node(h,(i+1),j,k)];
      schatting += (1.0-alfa)*beta      *(1.0-gamma)*point[SIMION_node(h,i,(j+1),k)];
      schatting += (1.0-alfa)*(1.0-beta)*gamma      *point[SIMION_node(h,i,j,(k+1))];
      schatting += alfa      *beta      *(1.0-gamma)*point[SIMION_node(h,(i+1),(j+1),k)];
      schatting += alfa      *(1.0-beta)*gamma      *point[SIMION_node(h,(i+1),j,(k+1))];
      schatting += (1.0-alfa)*beta      *gamma      *point[SIMION_node(h,i,(j+1),(k+1))];
      schatting += alfa      *beta      *gamma      *point[SIMION_node(h,(i+1),(j+1),(k+1))];

      return schatting;
    }
  else { return 0; }
}


double ADL_Weight_SIMION(int SegNr, double *P)	{
  // returns the weighting potential of electrode (arg1) at position (arg 2)
  //**************************************************************
  //** This version makes a Segmentation line error correction. **
  //**************************************************************

  double Weight;
  int i;
  if (ADL_G_Wpot==NULL) return 0; 
  if (SegNr >= ADL_G_Wpot->n) return 0;
  //	if (ADL_G_Wpot->Pot[SegNr]==NULL) return 0; 
  //	if (ADL_G_Wpot->Pot[0]==NULL) return 0; 
  //If the core weightingpotential nonzero, no segmentation line error occurs:
  if (SIMION_Potential(ADL_G_Wpot->Pot[0],&(ADL_G_Wpot->h),ADL_G_Wpot->GridSize,P[1],P[2],P[3])>0) 
    return SIMION_Potential(ADL_G_Wpot->Pot[SegNr],&(ADL_G_Wpot->h),ADL_G_Wpot->GridSize,P[1],P[2],P[3]);
  //Else, (Core weight =0), the segment weight should be 0 or 1. If not so, we have a seg line error. so we have to round the value:
  Weight = SIMION_Potential(ADL_G_Wpot->Pot[SegNr],&(ADL_G_Wpot->h),ADL_G_Wpot->GridSize,P[1],P[2],P[3]);
  if (Weight >0.5) return 1.0;      //no other segment can be bigger;
  if (Weight <0.1) return 0.0;      //for sure another segment is bigger;
  for (i=1;i<GetNSEG();i++)  //otherwise search over all segments to find a bigger one:
    if (SIMION_Potential(ADL_G_Wpot->Pot[i],&(ADL_G_Wpot->h),ADL_G_Wpot->GridSize,P[1],P[2],P[3]) > Weight) 
      return 0;
  return 1.0; //no other segment weighting potential was bigger if you get here...
}

//*******************************************************************************************

int ADL_DetectorPos_SIMION(double *P)
//In general: In Detector = weighting potential core in [0...1]
{
  int dx,dy,dz;
    
  if ((ADL_G_Epot->h.nx > 2) && (ADL_G_Epot->h.ny == 1) && (ADL_G_Epot->h.nz > 2))
    {
      //transform position into gridunits for 2D cylindric array
      dx = (sqrt((P[1]-ADL_Fields_SIMION_Center)*(P[1]-ADL_Fields_SIMION_Center)+(P[2]-ADL_Fields_SIMION_Center)*(P[2]-ADL_Fields_SIMION_Center))/ADL_Fields_SIMION_GridSize);
      dy = 0;
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else if ((ADL_G_Epot->h.nx > 2)||(ADL_G_Epot->h.ny > 2)||(ADL_G_Epot->h.nz > 2)) 
    {
      //transform position into gridunits for 3D array
      dx = (P[1]/ADL_Fields_SIMION_GridSize);
      dy = (P[2]/ADL_Fields_SIMION_GridSize);
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else return 10;
  
  
  //this is added so that P can be an event point
  if (dx >=  ADL_G_Epot->h.nx|| dy >= ADL_G_Epot->h.ny || dz >= ADL_G_Epot->h.nz || dx < 0 || dy < 0 || dz < 0)
    return 11;
  
  //if ADL_G_Stru is defined return true if the in bulk
  if(ADL_G_Stru)
    return ADL_G_Stru->Pot[0][SIMION_node(&(ADL_G_Stru->h),dx,dy,dz)];
  else
    return 666;
}

//*******************************************************************************************

int ADL_InDetector_SIMION(double *P)
//In general: In Detector = weighting potential core in [0...1]
{
  int dx,dy,dz;
    
if ((ADL_G_Epot->h.nx > 2) && (ADL_G_Epot->h.ny == 1) && (ADL_G_Epot->h.nz > 2))
    {
      //transform position into gridunits for 2D cylindric array
      dx = (sqrt((P[1]-ADL_Fields_SIMION_Center)*(P[1]-ADL_Fields_SIMION_Center)+(P[2]-ADL_Fields_SIMION_Center)*(P[2]-ADL_Fields_SIMION_Center))/ADL_Fields_SIMION_GridSize);
      dy = 0;
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else if ((ADL_G_Epot->h.nx > 2)||(ADL_G_Epot->h.ny > 2)||(ADL_G_Epot->h.nz > 2)) 
    {
      //transform position into gridunits for 3D array
      dx = (P[1]/ADL_Fields_SIMION_GridSize);
      dy = (P[2]/ADL_Fields_SIMION_GridSize);
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else { ADL_Fields_SIMION_InDetector = 0; return ADL_Fields_SIMION_InDetector;}

  //this is added so that P can be an event point
  if (dx >=  ADL_G_Epot->h.nx|| dy >= ADL_G_Epot->h.ny || dz >= ADL_G_Epot->h.nz || dx < 0 || dy < 0 || dz < 0)
    {ADL_Fields_SIMION_InDetector = 0; return ADL_Fields_SIMION_InDetector;}
    
  //if ADL_G_Stru is defined return true if the in bulk
  if(ADL_G_Stru){
    if(ADL_G_Stru->Pot[0][SIMION_node(&(ADL_G_Stru->h),dx,dy,dz)] == BULK || ADL_G_Stru->Pot[0][SIMION_node(&(ADL_G_Stru->h),dx,dy,dz)] == P_LAY)        ADL_Fields_SIMION_InDetector = 1;
    else ADL_Fields_SIMION_InDetector = 0;
    return ADL_Fields_SIMION_InDetector;
  }
    
  //if ADL_G_Stru is not defined then use the weighting field
  double CoreWeight = ADL_Weight_SIMION(0,P);
  ADL_Fields_SIMION_InDetector = ((CoreWeight > ADL_G_SIMION_SmallPot) && (CoreWeight < (1-ADL_G_SIMION_SmallPot)));
  return ADL_Fields_SIMION_InDetector;
}

//*******************************************************************************************

double ADL_EPot(double *P){
  return ((double)SIMION_Potential(ADL_G_Epot->Pot[0],&(ADL_G_Epot->h),ADL_G_Epot->GridSize,P[1],P[2],P[3]));
}

static	double ADL_CalculateEx(int x, double *P, int *DiffType){
  // simple field calculation from potential given in a cubic lattice.

  double Step;
  double Pplus[4],Pmin[4];
	
  //Initialization:
  Step = ADL_G_Epot->GridSize/2;
  Pplus[1] = Pmin[1] = P[1];
  Pplus[2] = Pmin[2] = P[2];
  Pplus[3] = Pmin[3] = P[3];
  Pplus[x] += Step; 
  Pmin[x] -= Step;
  //The StepSize is the minimum value that assures that 
  //Pplus and Pmin ly in DIFFERENT grid cells.
  //Otherwise, Ex would be calculated as partially constant within a gridcel!!!
  //Check which of the points are inside the active volume.
  //This defines which type of formula can be used to calculate Ex.
  *DiffType = 2*ADL_InDetector_SIMION(Pplus)+ADL_InDetector_SIMION(Pmin);

  switch(*DiffType){
  case 3 : //simplest case : Pplus and Pmin OK : 
    return (ADL_EPot(Pmin)-ADL_EPot(Pplus))/ADL_G_Epot->GridSize; 
	
  case 2 : //only Pplus in active volume, lets try to extrapolate in that direction
    return (ADL_EPot(P)-ADL_EPot(Pplus))/Step;
	
  case 1 : //only Pplus in active volume, lets try to extrapolate in that direction
    return (ADL_EPot(Pmin)-ADL_EPot(P))/Step;
	
  case 0 : //very rare... only thing possible is this:
    return (ADL_EPot(Pmin)-ADL_EPot(Pplus))/ADL_G_Epot->GridSize;
	
  default : printf("\nERROR ADL FIELDS SIMION:CalculateEx for position %le %le %le",P[1],P[2],P[3]);
  }
  return 0.0; //only default (=error) comes here...
}

int ADL_Efield_SIMION(double *P, double *E) {
  //returns values in [V/cm] !!!
  int	DiffType = 0;
  int returnvalue = 0;	

  if (!ADL_InDetector_SIMION(P)) {
    E[1] = E[2] = E[3] = 0.0; //define zero field inside electrodes and outside potential array
    return 0;				  //exit with remark that point outside detector volume.
  }

  E[1] =  ADL_CalculateEx(1,P,&DiffType); returnvalue += DiffType; returnvalue *=10;
  E[2] =  ADL_CalculateEx(2,P,&DiffType); returnvalue += DiffType; returnvalue *=10;
  E[3] =  ADL_CalculateEx(3,P,&DiffType); returnvalue += DiffType;

  return returnvalue; // returns value "xyz" with x,y,z = DiffType used in evaluation of Ex,Ey,Ez
  // problematic could be when x or y or z = 0 (but does not have to be).
}

//*******************************************************************************************

static	double ADL_CalculateWx(int x, int n, double *P, int *DiffType){
  // simple field calculation from potential given in a cubic lattice.
  // assumes the electric potential in this lattice is interpolated by:
  // **************************************************

  // **************************************************
  double Step;
  double Pplus[4],Pmin[4];
	
  //Initialization:
  Step = ADL_G_Wpot->GridSize/2;
  Pplus[1] = Pmin[1] = P[1];
  Pplus[2] = Pmin[2] = P[2];
  Pplus[3] = Pmin[3] = P[3];
  Pplus[x] += Step; 
  Pmin[x] -= Step;
  //The StepSize is the minimum value that assures that 
  //Pplus and Pmin ly in DIFFERENT grid cells.
  //Otherwise, Ex would be calculated as partially constant within a gridcel!!!
  //Check which of the points are inside the active volume.
  //This defines which type of formula can be used to calculate Ex.
  *DiffType = 2*ADL_InDetector_SIMION(Pplus)+ADL_InDetector_SIMION(Pmin);

  switch(*DiffType){
  case 3 : //simplest case : Pplus and Pmin OK : 
    return (ADL_Weight_SIMION(n,Pmin)-ADL_Weight_SIMION(n,Pplus))/ADL_G_Wpot->GridSize; 
	
  case 2 : //only Pplus in active volume, lets try to extrapolate in that direction
    return (ADL_Weight_SIMION(n,P)-ADL_Weight_SIMION(n,Pplus))/Step;
	
  case 1 : //only Pplus in active volume, lets try to extrapolate in that direction
    return (ADL_Weight_SIMION(n,Pmin)-ADL_Weight_SIMION(n,P))/Step;
	
  case 0 : //very rare... only thing possible is this:
    return (ADL_Weight_SIMION(n,Pmin)-ADL_Weight_SIMION(n,Pplus))/ADL_G_Wpot->GridSize;
	
  default : printf("\nERROR ADL FIELDS SIMION:CalculateWx for position %le %le %le",P[1],P[2],P[3]);
  }
  return 0.0; //only default (=error) comes here...
}

int ADL_Wfield_SIMION(int SegNr,double *P, double *W) {
  //returns values in [V/cm] !!!
  int	DiffType = 0;
  int returnvalue = 0;	

  if (!ADL_InDetector_SIMION(P)) {
    W[1] = W[2] = W[3] = 0.0; //define zero field inside electrodes and outside potential array
    return 0;				  //exit with remark that point outside detector volume.
  }

  W[1] =  ADL_CalculateWx(1,SegNr,P,&DiffType); returnvalue += DiffType; returnvalue *=10;
  W[2] =  ADL_CalculateWx(2,SegNr,P,&DiffType); returnvalue += DiffType; returnvalue *=10;
  W[3] =  ADL_CalculateWx(3,SegNr,P,&DiffType); returnvalue += DiffType;

  return returnvalue; // returns value "xyz" with x,y,z = DiffType used in evaluation of Ex,Ey,Ez
  // problematic could be when x or y or z = 0 (but does not have to be).
}


//HERE P is a three vector (comming from the event structure)
int ADL_Geo_SIMION(double *P){
  int dx,dy,dz;

  if ((ADL_G_Epot->h.nx > 2) && (ADL_G_Epot->h.ny == 1) && (ADL_G_Epot->h.nz > 2))
    {
      //transform position into gridunits for 2D cylindric array
      dx = (sqrt((P[1]-ADL_Fields_SIMION_Center)*(P[1]-ADL_Fields_SIMION_Center)+(P[2]-ADL_Fields_SIMION_Center)*(P[2]-ADL_Fields_SIMION_Center))/ADL_Fields_SIMION_GridSize);
      dy = 0;
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else if ((ADL_G_Epot->h.nx > 2)||(ADL_G_Epot->h.ny > 2)||(ADL_G_Epot->h.nz > 2)) 
    {
      //transform position into gridunits for 3D array
      dx = (P[1]/ADL_Fields_SIMION_GridSize);
      dy = (P[2]/ADL_Fields_SIMION_GridSize);
      dz = (P[3]/ADL_Fields_SIMION_GridSize);
    }
  else { return 0; }

  //this is added so that P can be an event point
  if (dx >= ADL_G_Epot->h.nx || dy >= ADL_G_Epot->h.ny || dz >= ADL_G_Epot->h.nz || dx < 0 || dy < 0 || dz < 0)
    return 0;

  return ADL_G_Wpot->Geo[SIMION_node(&(ADL_G_Epot->h),dx,dy,dz)];
}

//*******************************************************************************************
