#ifndef ADL_H
#define	ADL_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ADL_Convl.h"
#include "ADL_Drift.h"
#include "ADL_Event.h"
#include "ADL_Fields.h"
#include "ADL_ReadWrite.h"
#include "ADL_Parser.h"
#include "ADL_Time.h"
#include "ADL_Traces.h"
#include "ADL_Trapping.h"

//Include the simion routines
#include "SIMION_ReadWrite.h"
#include "SIMION_Array.h"
#include "SIMION_Geometry.h"
#include "SIMION_Solver.h"

void SetADLDebug(int);
void SetADLVersion(double);
void ADLStatus();
void ADLSetup(char*);

int GetADLDebug();
int GetADLVersion();

static int ADL_G_DEBUG;
static double  ADL_G_VERSION;

//the obligatory setup file:
static int ADL_Setup(char *);
static int ADL_Status(void);

#endif /* ADL_H */
