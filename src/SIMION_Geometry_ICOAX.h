/*
 * AGATA Detector simulation Library (ADL)
 *
 * Yoann Kermaidic - yoann.kermaidic@mpi-hd.mpg.de
 *
 * MPIK, Heidelberg
 */

#ifndef SIMION_GEOMETRY_ICOAX_H_
#define SIMION_GEOMETRY_ICOAX_H_

#include "ADL.h"
#include "SIMION_Array.h"

//allocate necessary variables:
static int ICOAX_G_Radius;
static int ICOAX_G_Height;
static int ICOAX_G_WellRadius;
static int ICOAX_G_WellRadiusTop;
static int ICOAX_G_WellDepth;
static int ICOAX_G_WellRounding;
static int ICOAX_G_PointContactRadius;
static int ICOAX_G_PointContactDepth;
static int ICOAX_G_SurfaceContactDepth;
static int ICOAX_G_GrooveDepth;
static int ICOAX_G_GrooveWidth;
static int ICOAX_G_GrooveInnerRadius;
static int ICOAX_G_PasLayThickness;
static int ICOAX_G_EdgeRadius;
static int ICOAX_G_EdgeHeight;
static int ICOAX_G_ExtGroundWidth;
static int ICOAX_G_Spacing;
static double ICOAX_G_ImpTop;
static double ICOAX_G_ImpBot;
static double ICOAX_G_ImpIn;
static double ICOAX_G_ImpOut;
static double ICOAX_G_Center;

static double SIMION_GridSize_ICoax;
static int SIMION_Dimension_ICoax;
static double SIMION_Voltage_ICoax;
static double SIMION_EpsScale_ICoax;
static double SIMION_EpsExtScale_ICoax;
static char SIMION_Description_ICoax[100];

//ICOAX routines:
int SIMION_Setup_GEOMETRY_ICOAX(char *);
int SIMION_Status_GEOMETRY_ICOAX(void);

struct SIMION_PA *SIMION_newPA_ICOAX(void);

int SIMION_CalcPoint_ICOAX(int,int,int,int);
double SIMION_CalcCharge_ICOAX(int,int,int,int);

double GetICoaxHeight();
double GetICoaxGridSize();
double GetICoaxDimension();
double GetICoaxVoltage();
double GetICoaxEpsScale();
double GetICoaxEpsExtScale();
char* GetICoaxDescription();

#endif /* SIMION_GEOMETRY_ICOAX_H_ */
