/*
 * AGATA Detector simulation Library (SIMION)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK Heidelberg
 * http://www.mpi-hd.mpg.de/gerda
 *
 */


#ifndef SIMION_GEOMETRY_H
#define SIMION_GEOMETRY_H

#include "ADL.h"
//The different versions of this library:
#include "SIMION_Geometry_PLANAR.h"
#include "SIMION_Geometry_BEGE.h"
#include "SIMION_Geometry_ICOAX.h"
#include "SIMION_Geometry_COAX.h"
#include "SIMION_Array.h"

static double SIMION_Height;
static double SIMION_Voltage_Geom;
static double SIMION_EpsScale_Geom;
static double SIMION_EpsExtScale_Geom;
static char* SIMION_Description_Geom;

//int InitializeGeometry(int(char*), int, int(int, int, int, int),double(int, int, int, int)/*,struct SIMION_PA * (void)*/);
void SetVoltage(double);
double GetVoltage();
int InitializeGeometry();
int SetupGeometry(char*);
struct SIMION_PA *GetSIMIONnewPA();
void GetSIMIONStructure(struct SIMION_PA *,int);
void GetSIMIONWfield(struct SIMION_PA *,int);
void GetSIMIONEfield(struct SIMION_PA *,int);

//GEOMETRY routines
static int (*SIMION_G_Setup_GEOMETRY)(char *);
static int (*SIMION_G_Status_GEOMETRY)(void);
int SIMION_Setup_GEOMETRY(char *);
int SIMION_Status_GEOMETRY(void);

static int (*SIMION_G_CalcPoint)(int,int,int,int);
static double (*SIMION_G_CalcCharge)(int,int,int,int);
static struct SIMION_PA *(*SIMION_G_newPA)(void);

static int SIMION_G_Structure(struct SIMION_PA *, int);
static int SIMION_G_WField(struct SIMION_PA *,int);
static int SIMION_G_EField(struct SIMION_PA *,int);

static int SIMION_G_Naming(struct SIMION_PA *,char*,int);
int SIMION_Geometry(struct SIMION_PA *);

#endif /* SIMION_GEOMETRY */
