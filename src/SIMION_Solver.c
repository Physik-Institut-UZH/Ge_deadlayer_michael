/*
 * AGATA Detector simulation Library (SIMION)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK Heidelberg
 * http://www.mpi-hd.mpg.de/gerda
 *
 */

#include "SIMION_Solver.h"
#include <math.h>

//struct SIMION_PA *SIMION_G_PA=NULL;	 //potential arrays


int InitializeSolver(){
    SIMION_G_RhoScale    = 1.0;  //factor to scale Rho (1 if Rho.pa in 10^10/cm^3)
    SIMION_G_Tol         = 1e-6; //[V] Error tolerance of solution
    SIMION_G_DEBUG      = 1;    //set for extra debugging
    
    SIMION_G_Setup_SOLVER   = SIMION_Setup_SOLVER_POISSON;
    SIMION_G_Status_SOLVER  = SIMION_Status_SOLVER_POISSON;
    SIMION_Solve3D  		= SIMION_Solve3D_POISSON;
    SIMION_Solve2D  		= SIMION_Solve2D_POISSON;
}

void SIMION_Solve(struct SIMION_PA * field){SIMION_G_Solve(field);}
int SetupSolver(char *libversion, char *setupfile){return SIMION_Setup_SOLVER(libversion,setupfile);}
void SetSIMIONRhoScale(double RhoScale){SIMION_G_RhoScale = RhoScale;}
void SetSIMIONTol(double Tol){SIMION_G_Tol = Tol;}
void SetSIMIONDebug(int DEBUG){SIMION_G_DEBUG = DEBUG;}
double GetSIMIONRhoScale(){return SIMION_G_RhoScale;}
double GetSIMIONTol(){return SIMION_G_Tol;}
int GetSIMIONDebug(){return SIMION_G_DEBUG;}

// set function pointers to right version:
static void SIMION_Set_F_SOLVER(char *libversion) 
	{
	if (strcmp(libversion,"SIMION_SOLVER_POISSON")==0) {
		SIMION_G_Setup_SOLVER  = SIMION_Setup_SOLVER_POISSON;
		SIMION_G_Status_SOLVER = SIMION_Status_SOLVER_POISSON;
		SIMION_Solve3D         = SIMION_Solve3D_POISSON;
		SIMION_Solve2D         = SIMION_Solve2D_POISSON;
		return;
	}
	if (strcmp(libversion,"SIMION_SOLVER_LAPLACE")==0) {
		SIMION_G_Setup_SOLVER  = SIMION_Setup_SOLVER_LAPLACE;
		SIMION_G_Status_SOLVER = SIMION_Status_SOLVER_LAPLACE;
		SIMION_Solve3D         = SIMION_Solve3D_LAPLACE;
		SIMION_Solve2D         = SIMION_Solve2D_LAPLACE;
		return;
	}
	if (strcmp(libversion,"SIMION_SOLVER_INHOMOG")==0) {
		SIMION_G_Setup_SOLVER  = SIMION_Setup_SOLVER_INHOMOG;
		SIMION_G_Status_SOLVER = SIMION_Status_SOLVER_INHOMOG;
		SIMION_Solve3D         = SIMION_Solve3D_INHOMOG;
		SIMION_Solve2D         = SIMION_Solve2D_INHOMOG;
		return;
	}
	// if you get here, libversion is not recognized:
	if (GetSIMIONDebug()) printf("\nERROR: SIMION SOLVER - lib %s not recognized \n",libversion);
	if (GetSIMIONDebug()) printf("       valid versions are: SIMION_SOLVER_LAPLACE SIMION_SOLVER_POISSON SIMION_SOLVER_INHOMOG\n",libversion);
	}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

//Here the setup file for this library version:
int SIMION_Setup_SOLVER(char *libversion, char *setupfile){
	if (strcmp(libversion,"SIMION_SOLVER_POISSON")==0) {
		SIMION_Set_F_SOLVER(libversion);
		return SIMION_Setup_SOLVER_POISSON(setupfile);
	}
	if (strcmp(libversion,"SIMION_SOLVER_LAPLACE")==0) {
		SIMION_Set_F_SOLVER(libversion);
		return SIMION_Setup_SOLVER_LAPLACE(setupfile);
	}
	if (strcmp(libversion,"SIMION_SOLVER_INHOMOG")==0) {
		SIMION_Set_F_SOLVER(libversion);
		return SIMION_Setup_SOLVER_INHOMOG(setupfile);
	}
	// if you get here, libversion is not recognized:
	if (GetSIMIONDebug()) printf("\nERROR: SIMION Setup SOLVER - lib %s not recognized \n",libversion);
	if (GetSIMIONDebug()) printf("       valid versions are: SIMION_SOLVER_POISSON \n",libversion);
	return 1;
}

int SIMION_Status_SOLVER(void){
	//print final values that are setup:
	printf("\nStatus SOLVER:\n");
	printf("------------\n");
	if (SIMION_G_Setup_SOLVER  == SIMION_Setup_SOLVER_POISSON)  printf("SIMION_G_Setup_SOLVER  -> SIMION_Setup_SOLVER_POISSON\n");
	if (SIMION_G_Status_SOLVER == SIMION_Status_SOLVER_POISSON) printf("SIMION_G_Status_SOLVER -> SIMION_Status_SOLVER_POISSON\n");
	if (SIMION_Solve2D         == SIMION_Solve2D_POISSON)       printf("SIMION_Solve2D         -> SIMION_Solve2D_POISSON\n");
	if (SIMION_Solve3D         == SIMION_Solve3D_POISSON)       printf("SIMION_Solve3D         -> SIMION_Solve3D_POISSON\n");

	if (SIMION_G_Setup_SOLVER  == SIMION_Setup_SOLVER_LAPLACE)  printf("SIMION_G_Setup_SOLVER  -> SIMION_Setup_SOLVER_LAPLACE\n");
	if (SIMION_G_Status_SOLVER == SIMION_Status_SOLVER_LAPLACE) printf("SIMION_G_Status_SOLVER -> SIMION_Status_SOLVER_LAPLACE\n");
	if (SIMION_Solve2D         == SIMION_Solve2D_LAPLACE)       printf("SIMION_Solve2D         -> SIMION_Solve2D_LAPLACE\n");
	if (SIMION_Solve3D         == SIMION_Solve3D_LAPLACE)       printf("SIMION_Solve3D         -> SIMION_Solve3D_LAPLACE\n");

	if (SIMION_G_Setup_SOLVER  == SIMION_Setup_SOLVER_INHOMOG)  printf("SIMION_G_Setup_SOLVER  -> SIMION_Setup_SOLVER_INHOMOG\n");
	if (SIMION_G_Status_SOLVER == SIMION_Status_SOLVER_INHOMOG) printf("SIMION_G_Status_SOLVER -> SIMION_Status_SOLVER_INHOMOG\n");
	if (SIMION_Solve2D         == SIMION_Solve2D_INHOMOG)       printf("SIMION_Solve2D         -> SIMION_Solve2D_INHOMOG\n");
	if (SIMION_Solve3D         == SIMION_Solve3D_INHOMOG)       printf("SIMION_Solve3D         -> SIMION_Solve3D_INHOMOG\n");


	SIMION_G_Status_SOLVER();
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++
// SOLVER DEFINITON FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

int SIMION_G_Solve(struct SIMION_PA *SPA){
	struct SIMION_HEADER *hh = &(SPA->h);
	if ((hh->nx >= 3) && (hh->ny == 1) && (hh->nz >= 3)) {printf("\n2D structure will be soved (R,1,Z). \n"); SIMION_Solve2D(SPA);}
	else if ((hh->nx >= 3)||(hh->ny >=3)||(hh->nz >= 3)) {printf("\n3D structure will be soved (X,Y,Z). \n"); SIMION_Solve3D(SPA);}
	else {printf("\nUnknown pa structure. It won't be solved. \n"); return 1;};
	return 0;
}
