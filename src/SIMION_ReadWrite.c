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
//****************************************************************************

#include "SIMION_ReadWrite.h"

/****************************************************************************/
/* The obligatory setup routine                                             */
int SIMION_Setup_READWRITE(char *lib_version,char *filename_setupfile){
  //nothing to setup...
  if (GetADLDebug()) SIMION_Status_READWRITE();
  return 0;
}


int SIMION_Status_READWRITE(void){
  //print final values that are setup:
  printf("\nStatus READWRITE:\n");
  printf("-----------------\n");
  //only two parameters to setup here:
  printf("(no variables)");
  return 0;
}

//************************************************************************************************************

double *SIMION_read_in_pot_array(char *naam,struct SIMION_HEADER *hh){
  // reads in a SIMION potential array
  FILE *ifile;
  double *point;
  if ((ifile = fopen(naam,"rb"))==NULL) {
    if (GetADLDebug()) printf("Error: %s can not be opened\n",naam); 
    return NULL;
  }
  else {
    if (GetADLDebug()) printf("%s: ",naam);
    fread(hh,sizeof(struct SIMION_HEADER),1,ifile);
    if (GetADLDebug()) printf("x%ld y%ld z%ld, ",hh->nx,hh->ny,hh->nz);
    point=(double *)calloc(hh->nx*hh->ny*hh->nz,sizeof(double));
    fread(point,sizeof(double),hh->nx*hh->ny*hh->nz,ifile);
    if (GetADLDebug()) printf("symm: x%ld y%ld z%ld, ",SIMION_X_MIRRORED(hh),SIMION_Y_MIRRORED(hh),SIMION_Z_MIRRORED(hh));
    fclose(ifile);
    if (GetADLDebug()) printf("maxv: %lf\n",hh->max_voltage);
  }
  return point;
}

static void SIMION_remove_max_voltage(double *pot, struct SIMION_HEADER *hh) {
  //removes the "max_voltage" out of the array
  //these values normally mark the electrodes
  long i,length = hh->nx*hh->ny*hh->nz;
  for (i=0;i<length;++i) if (pot[i]>(hh->max_voltage)) pot[i] -= 2.0*(hh->max_voltage);
}

void SIMION_set_array_boundary(int *geo, struct SIMION_HEADER *hh) {
  long i,j,k,X;
  long Xdir = 1;					//in x direction 
  long Ydir = (hh->nx);			//in y direction
  long Zdir =	(hh->nx*hh->ny);	//in z direction

  for (i=0;i<(hh->nx);i++) for (j=0;j<(hh->ny);j++) {X=SIMION_node(hh,i,j,0); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,i,j,(hh->nz-1)); if (geo[X]==0) geo[X]=1;} //z=constant tagged
  if(hh->ny>2){
    for (i=0;i<(hh->nx);i++) for (k=0;k<(hh->nz);k++) {X=SIMION_node(hh,i,0,k); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,i,(hh->ny-1),k); if (geo[X]==0) geo[X]=1;} //y=constant tagged
  }
  for (j=0;j<(hh->ny);j++) for (k=0;k<(hh->nz);k++) {X=SIMION_node(hh,0,j,k); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,(hh->nx-1),j,k); if (geo[X]==0) geo[X]=1;} //x=constant tagged
}

static	int *SIMION_make_geo_array(double *pot,struct SIMION_HEADER *hh){
  int *geo;
  long i,length = hh->nx*hh->ny*hh->nz;

  geo=(int *)calloc(length,sizeof(int));
  for (i=0;i<length;i++) geo[i] = 0; //initialization
  for (i=0;i<length;i++) if (pot[i] > (hh->max_voltage)) geo[i] = 2;
  SIMION_set_array_boundary(geo,hh); //set geo = 1 on boundaries;

  return geo;
}

struct SIMION_PA * SIMION_READ(char *FilenameMask,double gridsize){
  struct SIMION_PA *returnvalue;
  struct SIMION_HEADER h;
  char* filename;
  FILE *temp;
  int i,n;

   char buffer = (char) FilenameMask[0];
   if(buffer != 'C'){
     char* prefix = getenv("MAGETOADLDIR");
     filename = (char*)malloc(strlen(prefix) + strlen(FilenameMask) +1);
     strcpy( filename, prefix );
     strcat( filename, "/" );
     strcat( filename, FilenameMask);
   }
   else{
     filename = (char*)malloc(strlen(FilenameMask) +1);
     strcpy( filename, FilenameMask);
   }
   
  //first check number of files we can find:
  for (n=0;;n++){
    temp = fopen(filename,"rb");
    if (temp==NULL) break; 
    else{
      fclose(temp);
      //if filename == Mask, Mask did not contain a "%d" symbol:
      //if (strcmp(filename,FilenameMask)==0){
      n++;
      break; 
    }
  }
  if (GetADLDebug()) printf("\nSIMION_READ detected %d files for mask %s\n",n,FilenameMask);
  if (n<1) return NULL;
	
  //now create the appropriate PA structure:
  returnvalue = new_PA(n);
  returnvalue->GridSize = gridsize;
	
  //now fill this PA structure:
  //The first one is special: need to create geometry and header:
  //	sprintf(returnvalue->Name[0],FilenameMask,0);
  sprintf(returnvalue->Name[0],filename,0);
  returnvalue->Pot[0]=SIMION_read_in_pot_array(returnvalue->Name[0],&(returnvalue->h));
  returnvalue->Geo=SIMION_make_geo_array(returnvalue->Pot[0],&(returnvalue->h));
  SIMION_remove_max_voltage(returnvalue->Pot[0],&(returnvalue->h));
  returnvalue->ArrayLength = (returnvalue->h.nx*returnvalue->h.ny*returnvalue->h.nz);
  //Read in the rest of the pas, and check if headers are consistant:
  for (i=1;i<n;i++){
    //		sprintf(returnvalue->Name[i],FilenameMask,i);
    sprintf(returnvalue->Name[i],filename,i);
    returnvalue->Pot[i]=SIMION_read_in_pot_array(returnvalue->Name[i],&h);
    SIMION_remove_max_voltage(returnvalue->Pot[i],&h);
    //compare h to returnvalue->h:
    if (h.max_voltage > returnvalue->h.max_voltage) returnvalue->h.max_voltage=h.max_voltage;
    if (  (h.mode     != returnvalue->h.mode    )
	  ||(h.symmetry != returnvalue->h.symmetry)
	  ||(h.mirror   != returnvalue->h.mirror  )
	  ||(h.nx       != returnvalue->h.nx      )
	  ||(h.nx       != returnvalue->h.nx      )
	  ||(h.nx       != returnvalue->h.nx      ) ){
      printf("\nERROR - SIMION READ: \nfile %s and %s have different structure!\n",returnvalue->Name[0],returnvalue->Name[i]);
      returnvalue->n = i;
      free(returnvalue->Pot[i]);
    }
  }
	
  return returnvalue;
}

//************************************************************************************************************

static	void SIMION_write_pot_array(double *point, char *filename, struct SIMION_HEADER *hh, int *geo){
  // writes a potential
  FILE *ifile;
  long i,length = (hh->nx*hh->ny*hh->nz);

  //add max-voltage on electrode points
  for (i=0;i<length;++i) if (geo[i]>1) point[i] += 2.0*(hh->max_voltage);

  if ((ifile = fopen(filename,"wb"))==NULL) printf("\nERROR SIMION WRITE: %s can not be opened\n",filename);
  else if (GetADLDebug()) printf("Writing %s: ",filename);
  if (GetADLDebug()) printf("check %d -",fwrite(hh,sizeof(struct SIMION_HEADER),1,ifile));
  if (GetADLDebug()) printf("symm: x%ld y%ld z%ld, ",hh->nx,hh->ny,hh->nz);
  if (GetADLDebug()) printf("length: %ld, ",length);
  fwrite(point,sizeof(double),length,ifile);
  SIMION_remove_max_voltage(point,hh); 
  if (GetADLDebug()) printf("maxv: %lf\n",hh->max_voltage);
  fclose(ifile);
}

int SIMION_WRITE(struct SIMION_PA *PA){
  int i;
  for (i=0;i<PA->n;i++) SIMION_write_pot_array(PA->Pot[i],PA->Name[i],&(PA->h),PA->Geo);
  return 0;
}

int SIMION_WRITE_PA(struct SIMION_PA *PA, int pa_number, char *filename){
  if (filename) {
    if(strcmp(filename, "") == 0)
      SIMION_write_pot_array(PA->Pot[pa_number],PA->Name[pa_number],&(PA->h),PA->Geo);
    else SIMION_write_pot_array(PA->Pot[pa_number],filename,&(PA->h),PA->Geo);
  }
  else SIMION_write_pot_array(PA->Pot[pa_number],PA->Name[pa_number],&(PA->h),PA->Geo);
  return 0;
}

//************************************************************************************************************


