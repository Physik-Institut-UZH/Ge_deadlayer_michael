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

#ifndef SIMION_ARRAY_H
#define SIMION_ARRAY_H

#include "ADL.h"

//With this you can check the symmetry of the potential arrays header h
#define SIMION_X_MIRRORED(h) (h->mirror%2)    //array symmetry around x=0
#define SIMION_Y_MIRRORED(h) (h->mirror/2%2)  //array symmetry around y=0
#define SIMION_Z_MIRRORED(h) (h->mirror/4%2)  //array symmetry around z=0
#define SIMION_PLANAR(h)     (h->symmetry)    //0 for cylinder symmetry
#define SIMION_D3(h)         (h->nz>1)        //three dimensional array

//Define some values that are useful to set up the geometry (Marco)
#ifndef GEO_T
#define GEO_T
#define PI          3.1415926535
#define V_CONT      0
#define Z_CONT      1
#define BULK        2
#define GROOVE      3
#define OUTSIDE     4
#define P_LAY       5
#define EXT_GROUND  6
#endif

//Define some variables that are useful for setting up the geometry (Marco)
/*
static double SIMION_G_GridSize;
static int SIMION_G_Dimension;
static double SIMION_G_Voltage;
static double SIMION_G_EpsScale;
static double SIMION_G_EpsExtScale;
static char SIMION_G_Description[100];
*/

// header SIMION FILES
#ifndef SIMION_HEADER_
#define SIMION_HEADER_
struct SIMION_HEADER     //sizeof(int)=4 is needed here!!! (windows: __int32)
	{
	int mode;             //(forgot...should check SIMION manual)
	int symmetry;         //a number indicating which symmetries apply (planar or cylindrical)
	double max_voltage;   //a limit voltage used to store information on electrode points in the arrays
	int nx;               //number of gridpoints in x-direction
	int ny;               //number of gridpoints in y-direction
	int nz;               //number of gridpoints in z-direction
	int mirror;           //a number indicating which mirror symmetries apply (see above)
	};
#endif /* SIMION_HEADER_ */

#ifndef SIMION_PA_
#define SIMION_PA_
struct SIMION_PA
    {
	int n;                 //number of potential arrays stored in this structure
	double  GridSize;          //[cm/gridunit] stores the actual scale of the array
	int ArrayLength;       //common length of the pa arrays
    struct  SIMION_HEADER h;   //common header file to all the arrays in this structure
	char    **Name;            //array of strings containing the (file) name of the "n" arrays 
    double  **Pot;             //array of pointers to the "n" potential arrays
    int *Geo;              //geometry file common to all potential arrays in this structure
    };
#endif /* SIMION_PA_ */

int InitializeArray(double&,int&,double&,double&,double&,char*);

int SIMION_Setup_ARRAY(char *, char *);
int SIMION_Status_ARRAY(void);
// With this you can allocate an empty pa structure with n potentials:
struct SIMION_PA *new_PA(int /*n*/); 
//Here, additional to new_PA(int n),                                    
//you also allocate memory for the pa's and initialize them                 
// "name": string with the name of the pa - the filenames will be "name.pax"
// "n"   : number of pa's in the array
// "GridSize": gridsize in cm
//"NX,Y,Z"   : array length in X,Y and Z direction
struct SIMION_PA *new_SIMION_PA(char * /*name*/, int /*n*/, double /*GridSize*/, int /*NX*/, int /*NY*/, int /*NZ*/);
void free_SIMION_PA(struct SIMION_PA *pa); //free the space allocated to a simion_pa

//To allocate a header structure and fill in the dimension of the grid:
struct SIMION_HEADER *new_SIMION_HEADER(int /*NX*/, int /*NY*/, int /*NZ*/);
// Copy the header of one structure into another (probably not so usefull any more after above):
int SIMION_COPY_HEADER(struct SIMION_HEADER *, struct SIMION_HEADER *);
//Some functions to handle PA's the correct way
void SIMION_adapt_boundaries(double *,struct SIMION_HEADER *,int *);
void SIMION_adapt_geometry(int *, struct SIMION_HEADER *);
void SIMION_remove_geometry(int *, struct SIMION_HEADER *);
extern struct SIMION_PA* SIMION_2Dto3D_PA(struct SIMION_PA *PA2D);
extern struct SIMION_PA* SIMION_2Dto3D_FIELD(struct SIMION_PA *, int);

/****************************************************************************/
/* Take a double and change it into a correctly rounded integer             */
static inline int SIMION_grid(double in, double SIMION_GridSize_Array){
	int out=(int)(in/SIMION_GridSize_Array+0.5);
	if(out==0) out=1;
	if (in<0.0) out=1;
	if(in==0.0) out=0;
	return out;
}

/****************************************************************************/
/*Take an integer and change it to double value                             */
/*for the integer position [xc,yc,zc] (in grid units)                       */
static inline double SIMION_G_RealSize(int i, double SIMION_GridSize_Array){
	return i*SIMION_GridSize_Array;
}

/****************************************************************************/
/*Calculates the node in the array                                          */
/*for the integer position [xc,yc,zc] (in grid units)                       */
static inline long int SIMION_node(struct SIMION_HEADER *h, int xc, int yc,int zc){
	return ((zc)*(h->ny)*(h->nx)+(yc)*(h->nx)+(xc));
}


#endif /* SIMION_ARRAY_H */
