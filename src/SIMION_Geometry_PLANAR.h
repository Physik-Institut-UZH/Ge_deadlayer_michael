/*
 * AGATA Detector simulation Library (ADL)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK, Heidelberg
 */

#ifndef SIMION_GEOMETRY_PLANAR_H_
#define SIMION_GEOMETRY_PLANAR_H_

#include "ADL.h"
#include "SIMION_Array.h"

//allocate necessary variables:
extern int PLANAR_G_TopContactDepth;
extern int PLANAR_G_Height; 
extern int PLANAR_G_BotContactDepth;
extern int PLANAR_G_Spacing;
extern int PLANAR_G_ExtGroundWidth;
extern double PLANAR_G_ImpTop;
extern double PLANAR_G_ImpBot;

static double SIMION_GridSize_Planar;
static int SIMION_Dimension_Planar;
static double SIMION_Voltage_Planar;
static double SIMION_EpsScale_Planar;
static double SIMION_EpsExtScale_Planar;
static char SIMION_Description_Planar[100];

//PLANAR routines:
int SIMION_Setup_GEOMETRY_PLANAR(char *);
int SIMION_Status_GEOMETRY_PLANAR(void);

struct SIMION_PA *SIMION_newPA_PLANAR(void);

int SIMION_CalcPoint_PLANAR(int,int,int,int);
double SIMION_CalcCharge_PLANAR(int,int,int,int);

#endif /* SIMION_GEOMETRY_PLANAR_H_ */
