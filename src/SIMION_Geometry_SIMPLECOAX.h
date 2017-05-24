/*
 * AGATA Detector simulation Library (ADL)
 *
 * Marco Salathe - marco.salathe@mpi-hd.mpg.de
 *
 * MPIK, Heidelberg
 */

#ifndef SIMION_GEOMETRY_SIMPLCOAX_H_
#define SIMION_GEOMETRY_SIMPLCOAX_H_

#include "ADL.h"
#include "SIMION_Array.h"

//allocate necessary variables:
extern int SIMPLECOAX_G_InnerContactRadius;
extern int SIMPLECOAX_G_InnerContactWidth;
extern int SIMPLECOAX_G_OuterContactWidth;
extern int SIMPLECOAX_G_Width;
extern int SIMPLECOAX_G_Spacing;
extern int SIMPLECOAX_G_ExtGroundWidth;
extern int SIMPLECOAX_G_ImpTop;
extern int SIMPLECOAX_G_ImpBot;

static double SIMION_GridSize_SimpleCoax;
static int SIMION_Dimension_SimpleCoax;
static double SIMION_Voltage_SimpleCoax;
static double SIMION_EpsScale_SimpleCoax;
static double SIMION_EpsExtScale_SimpleCoax;
static char SIMION_Description_SimpleCoax[100];

//COAX routines:
int SIMION_Setup_GEOMETRY_COAX(char *);
int SIMION_Status_GEOMETRY_COAX(void);

struct SIMION_PA *SIMION_newPA_COAX(void);

int SIMION_CalcPoint_COAX(int,int,int,int);
double SIMION_CalcCharge_COAX(int,int,int,int);



#endif /* SIMION_GEOMETRY_SIMPLCOAX_H_ */
