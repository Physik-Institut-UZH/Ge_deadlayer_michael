/*
 * AGATA Detector simulation Library (ADL)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK, Heidelberg
 */

#ifndef SIMION_GEOMETRY_BEGE_H_
#define SIMION_GEOMETRY_BEGE_H_

#include "ADL.h"
#include "SIMION_Array.h"

extern int BEGE_G_GrooveDepth;
extern int BEGE_G_GrooveWidth;
extern int BEGE_G_PointContactRadius;
extern int BEGE_G_PointContactDepth;
extern int BEGE_G_PasLayThickness;
extern int BEGE_G_Radius;
extern int BEGE_G_Height; 
extern int BEGE_G_SurfaceContactDepth;
extern int BEGE_G_GrooveInnerRadius;
extern int BEGE_G_EdgeRadius;
extern int BEGE_G_EdgeHeight;
extern int BEGE_G_Spacing;
extern int BEGE_G_ExtGroundWidth;
extern double BEGE_G_ImpTop;
extern double BEGE_G_ImpBot;
extern double BEGE_G_Center;

static double SIMION_GridSize_Bege;
static int SIMION_Dimension_Bege;
static double SIMION_Voltage_Bege;
static double SIMION_EpsScale_Bege;
static double SIMION_EpsExtScale_Bege;
static char SIMION_Description_Bege[100];

//BEGE routines:
int SIMION_Setup_GEOMETRY_BEGE(char *);
int SIMION_Status_GEOMETRY_BEGE(void);

struct SIMION_PA *SIMION_newPA_BEGE(void);

int SIMION_CalcPoint_BEGE(int,int,int,int);
double SIMION_CalcCharge_BEGE(int,int,int,int);

#endif /* SIMION_GEOMETRY_BEGE_H_ */
