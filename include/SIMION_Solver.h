/*
 * AGATA Detector simulation Library (SIMION)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK Heidelberg
 * http://www.mpi-hd.mpg.de/gerda
 *
 */

#ifndef SIMION_SOLVERS_H_
#define SIMION_SOLVERS_H_

#include "ADL.h"
#include "SIMION_Array.h"

#include "SIMION_Solver_POISSON.h"
#include "SIMION_Solver_LAPLACE.h"
#include "SIMION_Solver_INHOMOG.h"

int InitializeSolver();
int SetupSolver(char*,char*);

void SIMION_Solve(struct SIMION_PA *);
void SetSIMIONRhoScale(double);
void SetSIMIONTol(double);
void SetSIMIONDebug(int);
double GetSIMIONRhoScale();
double GetSIMIONTol();
int GetSIMIONDebug();

//allocate necessary variables:
static double SIMION_GridSize_SOLVER;		//GridSize [cm/gridunit] (sets common dimension of above arrays)
static double SIMION_EpsScale_SOLVER;		//factor to scale relative Epsilon

static double SIMION_G_RhoScale;		//factor to scale Rho (1 if Rho.pa in 10^10/cm^3)
static double SIMION_G_Tol;				//[V] Error tolerance of solution
static int SIMION_G_DEBUG;			//set for extra debugging info

static double SIMION_RhoScale;		//factor to scale Rho (1 if Rho.pa in 10^10/cm^3)
static double SIMION_Tol;				//[V] Error tolerance of solution
static int SIMION_DEBUG;			//set for extra debugging info

//Solve routines
static int (*SIMION_G_Setup_SOLVER)(char *);
static int (*SIMION_G_Status_SOLVER)(void);
int SIMION_Setup_SOLVER(char *,char *);
int SIMION_Status_SOLVER(void);

static int SIMION_G_Solve(struct SIMION_PA *);
static int (*SIMION_Solve3D)(struct SIMION_PA *);
static int (*SIMION_Solve2D)(struct SIMION_PA *);


#endif /* SIMION_SOLVER */




