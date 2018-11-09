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
static int COAX_G_Radius;
static int COAX_G_Height;
static int COAX_G_BorHoleRadius;
static int COAX_G_BorHoleDepth;
static int COAX_G_CenContactRadius;
static int COAX_G_CenContactDepth;
static int COAX_G_OutContactRadius;
static int COAX_G_OutContactDepth;
static int COAX_G_GrooveDepth;
static int COAX_G_GrooveWidth;
static int COAX_G_GrooveInnerRadius;
static int COAX_G_PasLayThickness;
static int COAX_G_EdgeRadius;
static int COAX_G_EdgeHeight;
static int COAX_G_ExtGroundWidth;
static int COAX_G_Spacing;
static double COAX_G_ImpTop;
static double COAX_G_ImpBot;
static double COAX_G_ImpIn;
static double COAX_G_ImpOut;
static double COAX_G_Center;

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

double GetCoaxHeight();
double GetCoaxGridSize();
double GetCoaxDimension();
double GetCoaxVoltage();
double GetCoaxEpsScale();
double GetCoaxEpsExtScale();
char* GetCoaxDescription();

#endif /* SIMION_GEOMETRY_COAX_H_ */
