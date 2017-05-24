/*
 * AGATA Detector simulation Library (ADL)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK, Heidelberg
 */

#ifndef SIMION_GEOMETRY_COAX_H_
#define SIMION_GEOMETRY_COAX_H_

#include "ADL.h"
#include "SIMION_Array.h"

//allocate necessary variables:
extern int COAX_G_Radius;
extern int COAX_G_Height; 
extern int COAX_G_BorHoleRadius;
extern int COAX_G_BorHoleDepth;
extern int COAX_G_CenContactRadius;
extern int COAX_G_CenContactDepth;
extern int COAX_G_OutContactRadius;
extern int COAX_G_OutContactDepth;
extern int COAX_G_GrooveDepth;
extern int COAX_G_GrooveWidth;
extern int COAX_G_GrooveInnerRadius;
extern int COAX_G_PasLayThickness;
extern int COAX_G_EdgeRadius;
extern int COAX_G_EdgeHeight;
extern int COAX_G_ExtGroundWidth;
extern int COAX_G_Spacing;
extern double COAX_G_ImpTop;
extern double COAX_G_ImpBot;
extern double COAX_G_Center;

static double SIMION_GridSize_Coax;
static int SIMION_Dimension_Coax;
static double SIMION_Voltage_Coax;
static double SIMION_EpsScale_Coax;
static double SIMION_EpsExtScale_Coax;
static char SIMION_Description_Coax[100];

//COAX routines:
int SIMION_Setup_GEOMETRY_COAX(char *);
int SIMION_Status_GEOMETRY_COAX(void);

struct SIMION_PA *SIMION_newPA_COAX(void);

int SIMION_CalcPoint_COAX(int,int,int,int);
double SIMION_CalcCharge_COAX(int,int,int,int);

#endif /* SIMION_GEOMETRY_COAX_H_ */
