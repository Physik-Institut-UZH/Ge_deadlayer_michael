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

#include "SIMION_Solver_INHOMOG.h"
#include <math.h>

//Physics constants:
#ifndef q_epsion0
#define q_epsilon0	1.80951274e4	//1.602/8.85*1e5 [elementary charge/epsilon0*1e2] so that everything is in cm (GridSize is in cm)
#endif

//setup***************************************************************************
int SIMION_Setup_SOLVER_INHOMOG(char *filename_setupfile){

	int	i,len,err=1;
	struct	ADL_KEYWORD **Kwords;
	
	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		//first scan for gridsize!
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_DEBUG")==0) 
				sscanf(Kwords[2+i]->svalue,"%d" ,&SIMION_DEBUG);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_GridSize")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_INHOMOG);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_INHOMOG);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_RhoScale")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_RhoScale);
			if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Tol")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Tol);
		}
	}

    SetSIMIONDebug(SIMION_DEBUG);
    SetSIMIONTol(SIMION_Tol);
    SetSIMIONRhoScale(SIMION_RhoScale);
    
	if (SIMION_DEBUG) SIMION_Status_SOLVER_INHOMOG();
	return err;
}

int SIMION_Status_SOLVER_INHOMOG(void){
	//print final values that are setup:

	printf("\nStatus SIMION_Solver_INHOMOG:\n");
	printf("---------------------\n");
	printf("SIMION_G_GridSize   = %lf\n",SIMION_GridSize_INHOMOG);
	printf("SIMION_G_EpsScale   = %lf\n",SIMION_EpsScale_INHOMOG);
	printf("SIMION_G_RhoScale   = %lf\n",GetSIMIONRhoScale());
	printf("SIMION_G_Tol        = %lf\n",GetSIMIONTol());
	return 0;
}

//solve Poisson***************************************************************************

#ifndef FMAX  
static	double maxarg1,maxarg2;
#define FMAX(a,b)	(maxarg1=(a),maxarg2=(b),(maxarg1)>(maxarg2) ? (maxarg1) : (maxarg2))
#endif

//*****************************************************************************************

int SIMION_Solve3D_INHOMOG(struct SIMION_PA *SPA)
{
	double new_value, max_change = 1.0;
	double last_print_value = 1.0;
	double max_overrelax = 0.40;
	double RhoScale = (GetSIMIONRhoScale()*q_epsilon0*SIMION_GridSize_INHOMOG*SIMION_GridSize_INHOMOG);
	long   i, iter=0;

	double  *pot = NULL;
	double  *rho = NULL;
	double  *eps = NULL;
	int *geo = NULL;
	struct SIMION_HEADER *hh = NULL;
	
	if (!SPA)			{printf("\nERROR in SIMION_Solve3D_INHOMOG : SIMION_PA does not exist. \n"); return 0;}
	if (SPA->n < 3)		{printf("\nERROR in SIMION_Solve3D_INHOMOG : Expected at least 3 pa's on input. \n"); return 0;}
	if (SPA->n > 3)		printf("\nWARNING in SIMION_Solve3D_INHOMOG : There are more than 3 pa's on input. \n");
	pot = SPA->Pot[0];	if(GetADLDebug()) printf("\n Potential array to refine : %s",SPA->Name[0]);
	rho = SPA->Pot[1];	if(GetADLDebug()) printf("\n Space charge array set to : %s",SPA->Name[1]);
	eps = SPA->Pot[2];      if(GetADLDebug()) printf("\n Epsilon array set to      : %s",SPA->Name[2]);
	hh = &(SPA->h);
	geo = SPA->Geo;

	if ((hh->nx < 3)||(hh->ny < 3)||(hh->nz < 3)) {printf("\nERROR in SIMION_Solve3D_INHOMOG : A 3D structure is expected (X,Y,Z). \n"); return 0;}

	long Xdir = 1;					//in x direction 
	long Ydir = (hh->nx);			//in y direction
	long Zdir = (hh->nx*hh->ny);	//in z direction
	long length = (hh->nx*hh->ny*hh->nz);
	
	//Scale rho to to rho*deltaX^2/epsilon
	for (i=0;i<length;i++) rho[i] *= RhoScale;
	
	SIMION_adapt_geometry(geo,hh);
	SIMION_adapt_boundaries(pot,hh,geo);

	if(GetADLDebug()) printf("\n refining ");

	while (max_change > GetSIMIONTol())
	{
		max_change = 0.0;

		for (i=0;i<length;++i)
			if (geo[i]==0) {//means:skip boundary points
				new_value = (pot[i+Xdir]*(eps[i+Xdir]+eps[i]) + 
				             pot[i-Xdir]*(eps[i-Xdir]+eps[i]) + 
				             pot[i+Ydir]*(eps[i+Ydir]+eps[i]) + 
				             pot[i-Ydir]*(eps[i-Ydir]+eps[i]) + 
				             pot[i+Zdir]*(eps[i+Zdir]+eps[i]) + 
				             pot[i-Zdir]*(eps[i-Zdir]+eps[i]) + 
				             rho[i]*2.0) / 
				             (eps[i+Xdir]+eps[i-Xdir]+eps[i+Ydir]+eps[i-Ydir]+eps[i+Zdir]+eps[i-Zdir]+6.0*eps[i]);
				max_change = FMAX(max_change,fabs(pot[i]-new_value));
				pot[i] = new_value + max_overrelax*(new_value-pot[i]);
		}

		SIMION_adapt_boundaries(pot,hh,geo);

		//update overrelaxation:
		if (iter <10) max_overrelax = 0.67; else max_overrelax = 0.90;

		//Show some progress:
		if (max_change < 0.5*last_print_value) {last_print_value = max_change; if(GetADLDebug()) printf("*"); fflush(stdout);} 
		iter ++;

		//Limit maximum number of iterations:
		//if (iter > 20000) break;
	}


	//Undo scaling:
	for (i=0;i<length;i++) rho[i] /= RhoScale;
	SIMION_remove_geometry(geo,hh);

	if(GetADLDebug()) printf("| refinement completed in %ld iter\n",iter);
	return (int) iter;
}

int SIMION_Solve2D_INHOMOG(struct SIMION_PA *SPA)
{
	double new_value, max_change = 1.0;
	double last_print_value = 1.0;
	double max_overrelax = 0.40;
	double RhoScale = (GetSIMIONRhoScale()*q_epsilon0*SIMION_GridSize_INHOMOG*SIMION_GridSize_INHOMOG);
	long   i, r ,z , iter=0;
	double min;

	double *pot           = NULL;
	double *rho           = NULL;
	double *eps           = NULL;
	int *geo          = NULL;
	struct SIMION_HEADER *hh = NULL;

	if (!SPA)			{printf("\nERROR in SIMION_Solve2D_INHOMOG : SIMION_PA does not exist. \n"); return 0;}
	if (SPA->n < 3)		{printf("\nERROR in SIMION_Solve2D_INHOMOG : Expected at least 3 pa's on input. \n"); return 0;}
	if (SPA->n > 3)		printf("\nWARNING in SIMION_Solve2D_INHOMOG : There are more than 3 pa's on input. \n");
	pot = SPA->Pot[0];  if(GetADLDebug()) printf("\n Potential array to refine : %s",SPA->Name[0]);
	rho = SPA->Pot[1];  if(GetADLDebug()) printf("\n Space charge array set to : %s",SPA->Name[1]);
	eps = SPA->Pot[2];  if(GetADLDebug()) printf("\n Epsilon array set to      : %s",SPA->Name[2]);
	hh = &(SPA->h);
	geo = SPA->Geo;

	if ((hh->nx < 3)||(hh->ny != 1)||(hh->nz < 3)) {printf("\nERROR in SIMION_Solve2D_INHOMOG : A 2D structure is expected (R,1,Z). \n"); return 0;}

	long Rdir = 1;				//in r direction 
	long Zdir = (hh->nx*hh->ny);		//in z direction
	long length = (hh->nx*hh->ny*hh->nz);

	//build the radius correction matrices
	double rplus[length];
	double rminus[length];
	for(z=0; z<hh->nz; z++){
		rplus[SIMION_node(hh,0,0,z)]  = 2.0;
		rminus[SIMION_node(hh,0,0,z)] = 0.0;
		for (r=1; r<hh->nx; r++){
			rplus[SIMION_node(hh,r,0,z)]  = 1.0 + 0.5/(double)(r);
			rminus[SIMION_node(hh,r,0,z)] = 1.0 - 0.5/(double)(r);
		}
	}

	//Scale rho to to rho*deltaX^2/epsilon
	for (i=0;i<length;i++) rho[i] *= RhoScale*2.0;

	SIMION_adapt_geometry(geo,hh);
	SIMION_adapt_boundaries(pot,hh,geo);

	if(GetADLDebug()) printf("\n refining ");

	while (max_change > GetSIMIONTol())
	{
		max_change = 0.0;
		for (i=0;i<length;++i)
			if (geo[i]==0) {//means:skip boundary points
				//the actual finite difference algorithm
				new_value  = (rplus[i]*pot[i+Rdir]*(eps[i+Rdir]+eps[i]) + 
				             rminus[i]*pot[i-Rdir]*(eps[i-Rdir]+eps[i]) + 
				                       pot[i+Zdir]*(eps[i+Zdir]+eps[i]) + 
				                       pot[i-Zdir]*(eps[i-Zdir]+eps[i]) + 
				                       rho[i])/
				                       ((eps[i+Rdir]+eps[i])*rplus[i]+(eps[i-Rdir]+eps[i])*rminus[i]+eps[i+Zdir]+eps[i-Zdir]+2.0*eps[i]);
				max_change = FMAX(max_change,fabs(pot[i]-new_value));
				pot[i] = new_value + max_overrelax*(new_value-pot[i]);
			}

		SIMION_adapt_boundaries(pot,hh,geo);

		//update overrelaxation:
		if (iter <10) max_overrelax = 0.67; else max_overrelax = 0.90;

		//Show some progress:
		if (max_change < 0.5*last_print_value) {last_print_value = max_change; if(GetADLDebug()){ printf("*"); fflush(stdout);} }
		iter ++;

		//Limit maximum number of iterations:
		//if (iter > 20000) break;
	}
	
	//Undo scaling:
	for (i=0;i<length;i++) rho[i] /= (RhoScale*2.0);
	SIMION_remove_geometry(geo,hh);

	if(GetADLDebug()) printf("| refinement completed in %ld iter\n",iter);

	return (int) iter;
}
