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

#include "SIMION_Array.h"
#include "math.h"
#include "string.h"

/****************************************************************************/
/* Global initial values for variables                                      */
int InitializeArray(double &GridSize_Init,int &Dimension_Init,double &Voltage_Init,double &EpsScale_Init,double &EpsExtScale_Init,char* DefaultGeom_Init){
    
    GridSize_Init       = 1;
    Dimension_Init     = 2;
    Voltage_Init        = 1;
    EpsScale_Init       = 16.0;
    EpsExtScale_Init    = 1.0;
    DefaultGeom_Init = "GEOM";

    return 0;
}

/****************************************************************************/
/* The obligatory setup routine                                             */
int SIMION_Setup_ARRAY(char *lib_version,char *filename_setupfile){
	//nothing to setup...
	if (GetADLDebug()) SIMION_Status_ARRAY();
	return 0;
	}
int SIMION_Status_ARRAY(void){
	//print final values that are setup:
	printf("\nStatus ARRAY:\n");
	printf("-------------\n");
	//only two parameters to setup here:
	printf("(no variables)");
	return 0;
}

/****************************************************************************/
/* With this you can allocate an empty pa structure with n potentials       */
struct SIMION_PA *new_PA(int n){
	int i;
	struct SIMION_PA *returnPA=NULL;
	returnPA = (struct SIMION_PA *)calloc(1,sizeof(struct SIMION_PA));
	if (!returnPA) {printf("\nSIMION_ARRAY: new_PA allocation failure \n"); return NULL; }

	returnPA->n = n;
	returnPA->GridSize = 1.0;
	returnPA->ArrayLength = 0;	

	returnPA->Pot = (double **) calloc(n,sizeof(double *));
	if (!returnPA->Pot) {printf("\nSIMION_ARRAY: new_PA allocation failure \n"); return NULL; }

	returnPA->Name = (char **) calloc(n,sizeof(char *));
	if (!returnPA->Name) {printf("\nSIMION_ARRAY: new_PA allocation failure \n"); return NULL; }

	for (i=0;i<n;i++) returnPA->Name[i] = (char *)calloc(100,sizeof(char));
	for (i=0;i<n;i++) if (!returnPA->Name[i]) {printf("\nSIMION_ARRAY: new_PA allocation failure \n"); return NULL; }

	return returnPA;
}

/****************************************************************************/
/*Here, additional to new_PA(int n),                                    */
/*you also allocate memory for the pa's and initialize them                 */
struct SIMION_PA *new_SIMION_PA(char *NAME, int n, double GRIDSIZE, int NX, int NY, int NZ){
	int i;
	unsigned long p; 
	struct SIMION_PA * pa; 
	
	//only 3D arrays are implemented:
	//NX,Y,Z < 3 is not allowed, due to Neumann condition on boundaries !
	if ((NX < 3)||(NZ < 3)){//}||(NZ < 3)) {
		printf("\n WARNING new_SIMION_PA: dimension increased to minimum 3! \n");
		if (NX <3) NX = 3;
	//	if (NY <3) NY = 3;
		if (NZ <3) NZ = 3;
	}
	
	//Filling in the information about the grid:
	pa				= new_PA(n); 
	pa->n			= n;
	pa->GridSize	= GRIDSIZE; 
	pa->h.mode = -1;				// Fixed value (see SIMION manual) 
	pa->h.symmetry = 1;				// Fixed value (see SIMION manual)
	pa->h.max_voltage = 100000;		// Fixed value (see SIMION manual) 
	pa->h.nx = NX;
	pa->h.ny = NY;
	pa->h.nz = NZ;
	pa->h.mirror = 1600;			// Fixed value (see SIMION manual)
	pa->ArrayLength = pa->h.nx * pa->h.ny * pa->h.nz;

	//Allocating memory for the arrays:
	pa->Pot = (double**)calloc(pa->n, sizeof(double*)); 	
	pa->Geo = (int*)calloc(pa->ArrayLength, sizeof(int));
	pa->Name = (char**)calloc(pa->n, sizeof(char*)); 
	for (i=0; i<n; i++) pa->Name[i] = (char*)calloc(64, sizeof(char)); 	
	for (i=0; i<n; i++) pa->Pot[i]  = (double*)calloc(pa->ArrayLength, sizeof(double)); 	
	if (!pa->Pot[n-1]) {printf("\n ERROR new_SIMION_PA: allocation failure \n"); return NULL; }
	
	//Initializing the memory:
	for (i=0; i<n; i++) sprintf(pa->Name[i], "%s.pa%d", NAME, i);
	for (p = 0; p < pa->ArrayLength; p++) pa->Geo[p] = 0;
	for (i=0; i<n; i++) for(p = 0; p < pa->ArrayLength; p++) pa->Pot[i][p] = 0.0;
	
	return pa;
}

/****************************************************************************/
/* Delete method for simion_pa structure objects                            */
void free_SIMION_PA(struct SIMION_PA *pa)
  {
    int size = pa->n;
    int i;
    for(i=0;i<size;i++){
    free(pa->Name[i]);
    free(pa->Pot[i]);
  }
  free(pa->Geo);
  free(pa->Name);
  free(pa->Pot);
};


/****************************************************************************/
/* HEADERS                                                                  */
/****************************************************************************/


struct SIMION_HEADER *new_SIMION_HEADER(int NX, int NY, int NZ){
/* With this you can allocate a header structure and fill in the dimension of the grid */	
	// Some Information about our Grid 
	struct SIMION_HEADER * header; 
	header = (struct SIMION_HEADER*)malloc(sizeof(struct SIMION_HEADER)); 
	if (!header) {printf("\n ERROR new_SIMION_HEADER: allocation failure \n"); return NULL; }
	header->mode = -1;				// Fixed value (see SIMION manual) 
	header->symmetry = 1;			// Fixed value (see SIMION manual)
	header->max_voltage = 100000;	// Fixed value (see SIMION manual) 
	header->nx = NX;
	header->ny = NY;
	header->nz = NZ;
	header->mirror = 1600;			// Fixed value (see SIMION manual)
	return header;
}

/****************************************************************************/
/* With this you can cope the header of one structure into another          */
int SIMION_COPY_HEADER(struct SIMION_HEADER *Original,struct SIMION_HEADER *Duplicate){
	Duplicate->mode			= Original->mode;
	Duplicate->symmetry		= Original->symmetry;
	Duplicate->max_voltage	= Original->max_voltage;
	Duplicate->nx			= Original->nx;
	Duplicate->ny			= Original->ny;
	Duplicate->nz			= Original->nz;
	Duplicate->mirror		= Original->mirror;
	return 0;
}

/****************************************************************************/
/* General functions for handling                                           */
/****************************************************************************/

void SIMION_adapt_boundaries(double *rho,struct SIMION_HEADER *hh,int *geo)
{
	long i,j,k;

	// borders with z=constant
	for (i=0;i<(hh->nx);i++){
		for (j=0;j<(hh->ny);j++){
			if (geo[SIMION_node(hh,i,j,0)]==1) rho[SIMION_node(hh,i,j,0)]=rho[SIMION_node(hh,i,j,1)];
			if (geo[SIMION_node(hh,i,j,(hh->nz-1))]==1) rho[SIMION_node(hh,i,j,(hh->nz-1))] = rho[SIMION_node(hh,i,j,(hh->nz-2))];
		};
	};

	// borders with y=constant
	if(hh->ny>2){
		for (i=0;i<(hh->nx);i++){
			for (k=0;k<(hh->nz);k++){
				if (geo[SIMION_node(hh,i,0,k)]==1) rho[SIMION_node(hh,i,0,k)]=rho[SIMION_node(hh,i,1,k)];
				if (geo[SIMION_node(hh,i,(hh->ny-1),k)]==1) rho[SIMION_node(hh,i,(hh->ny-1),k)] = rho[SIMION_node(hh,i,(hh->ny-2),k)];
			};
		};
	};
	
	// borders with x=constant
	for (j=0;j<(hh->ny);j++){
		for (k=0;k<(hh->nz);k++){
			if (geo[SIMION_node(hh,0,j,k)]==1)	rho[SIMION_node(hh,0,j,k)]=rho[SIMION_node(hh,1,j,k)];
			if (geo[SIMION_node(hh,(hh->nx-1),j,k)]==1) rho[SIMION_node(hh,(hh->nx-1),j,k)] = rho[SIMION_node(hh,(hh->nx-2),j,k)];
		};
	};
}

void SIMION_adapt_geometry(int *geo, struct SIMION_HEADER *hh)
{
	long i,j,k,X;
	for (i=0;i<(hh->nx);i++) for (j=0;j<(hh->ny);j++) {X=SIMION_node(hh,i,j,0); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,i,j,(hh->nz-1)); if (geo[X]==0) geo[X]=1;} //z=constant tagged
	if(hh->ny>2){
		for (i=0;i<(hh->nx);i++) for (k=0;k<(hh->nz);k++) {X=SIMION_node(hh,i,0,k); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,i,(hh->ny-1),k); if (geo[X]==0) geo[X]=1;} //y=constant tagged
	}
	for (j=0;j<(hh->ny);j++) for (k=0;k<(hh->nz);k++) {X=SIMION_node(hh,0,j,k); if (geo[X]==0) geo[X]=1; X=SIMION_node(hh,(hh->nx-1),j,k); if (geo[X]==0) geo[X]=1;} //x=constant tagged

}

void SIMION_remove_geometry(int *geo, struct SIMION_HEADER *hh)
{
	long i;
	long length = (hh->nx*hh->ny*hh->nz);
		for (i=0; i<length; ++i)
			if (geo[i]==1); geo[i]=0;
}


struct SIMION_PA* SIMION_2Dto3D_PA(struct SIMION_PA *PA2D)
{
	int x,y,z,i;
	double center=PA2D->h.nx-0.5, r, alpha;
	printf("\n Writing 3D structure from 2D files!\n");
	struct SIMION_PA * PA3D = new_SIMION_PA(PA2D->Name[0], PA2D->n, PA2D->GridSize, PA2D->h.nx*2, PA2D->h.nx*2, PA2D->h.nz);
	for (x=0; x<PA3D->h.nx;++x){
		for (y=0; y<PA3D->h.ny;++y){
			for (z=0; z<PA3D->h.nz;++z){
				r=sqrt(((double)x-center)*((double)x-center)+((double)y-center)*((double)y-center))-0.5;
				alpha=r-floor(r);
				if(r>=PA2D->h.nx-1){
					r=PA2D->h.nx-1;
 					PA3D->Pot[0][SIMION_node((&PA3D->h),x,y,z)]=PA2D->Pot[0][SIMION_node((&PA2D->h),PA2D->h.nx-1,0,z)];
				}
				else PA3D->Pot[0][SIMION_node((&PA3D->h),x,y,z)]=(1-alpha)*PA2D->Pot[0][SIMION_node((&PA2D->h),(int)floor(r),0,z)]+alpha*PA2D->Pot[0][SIMION_node((&PA2D->h),(int)ceil(r),0,z)];
				
				PA3D->Pot[1][SIMION_node((&PA3D->h),x,y,z)]=PA2D->Pot[1][SIMION_node((&PA2D->h),(int)floor(r),0,z)];
				PA3D->Pot[2][SIMION_node((&PA3D->h),x,y,z)]=PA2D->Pot[2][SIMION_node((&PA2D->h),(int)floor(r),0,z)];
				PA3D->Geo[SIMION_node((&PA3D->h),x,y,z)]=PA2D->Geo[SIMION_node((&PA2D->h),(int)floor(r),0,z)];
			}
		}
	}
	for (i=0; i<PA2D->n; i++) 	sprintf(PA3D->Name[i], "%.*s_3D.pa", strlen(PA2D->Name[i])-3, PA2D->Name[i]);;
	return PA3D;
}

struct SIMION_PA* SIMION_2Dto3D_FIELD(struct SIMION_PA *PA2D, int npot)
{
	int x,y,z,i;
	double center=PA2D->h.nx-0.5, r, alpha;
	printf("\n Writing 3D structure from 2D files!\n");
	if (npot>=PA2D->n){
		printf("\n Error in SIMION_2Dto3D_FIELD: potential number is to large.\n");
		return 0;
	}
	
	struct SIMION_PA * PA3D = new_SIMION_PA(PA2D->Name[npot], 1, PA2D->GridSize, PA2D->h.nx*2, PA2D->h.nx*2, PA2D->h.nz);

	for (x=0; x<PA3D->h.nx;++x){
		for (y=0; y<PA3D->h.ny;++y){
			for (z=0; z<PA3D->h.nz;++z){
				r=sqrt(((double)x-center)*((double)x-center)+((double)y-center)*((double)y-center))-0.5;
				alpha=r-floor(r);
				if(r>=PA2D->h.nx-1) PA3D->Pot[0][SIMION_node((&PA3D->h),x,y,z)]=PA2D->Pot[npot][SIMION_node((&PA2D->h),PA2D->h.nx-1,0,z)];
				else PA3D->Pot[0][SIMION_node((&PA3D->h),x,y,z)]=(1-alpha)*PA2D->Pot[npot][SIMION_node((&PA2D->h),(int)floor(r),0,z)]+alpha*PA2D->Pot[npot][SIMION_node((&PA2D->h),(int)ceil(r),0,z)];
				PA3D->Geo[SIMION_node((&PA3D->h),x,y,z)]=PA2D->Geo[SIMION_node((&PA2D->h),(int)floor(r),0,z)];
			}
		}
	}
	
	sprintf(PA3D->Name[0], "%.*s_3D.pa", strlen(PA2D->Name[npot])-3, PA2D->Name[npot]);

	return PA3D;
}



