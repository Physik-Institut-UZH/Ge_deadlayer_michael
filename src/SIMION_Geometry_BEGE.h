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

static int BEGE_G_GrooveDepth;
static int BEGE_G_GrooveWidth;
static int BEGE_G_PointContactRadius;
static int BEGE_G_PointContactDepth;
static int BEGE_G_PasLayThickness;
static int BEGE_G_Radius;
static int BEGE_G_Height;
static int BEGE_G_SurfaceContactDepth;
static int BEGE_G_GrooveInnerRadius;
static int BEGE_G_EdgeRadius;
static int BEGE_G_EdgeHeight;
static int BEGE_G_Spacing;
static int BEGE_G_ExtGroundWidth;
static double BEGE_G_ImpTop;
static double BEGE_G_ImpBot;
static double BEGE_G_ImpIn;
static double BEGE_G_ImpOut;
static double BEGE_G_Center;

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

double GetBegeHeight();
double GetBegeGridSize();
double GetBegeDimension();
double GetBegeVoltage();
double GetBegeEpsScale();
double GetBegeEpsExtScale();
char* GetBegeDescription();

#endif /* SIMION_GEOMETRY_BEGE_H_ */
