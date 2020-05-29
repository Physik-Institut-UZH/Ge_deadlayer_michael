/*
 * AGATA Detector simulation Library (ADL)
 *
 * Bart Bruyneel - bartb@ikp.uni-koeln.de
 * Benedikt Birkenbach - bene@ikp.uni-koeln.de
 *
 * IKP Cologne
 * http://www.ikp.uni-koeln.de/agata
 *
 */

#ifndef ADL_DRIFT_H_
#define ADL_DRIFT_H_

#include "ADL.h"
#include "ADL_Parser.h"
//The different library versions:
#include "ADL_Drift_GE.h"

void GetElectronVelocity(double*,double*);
void GetHoleVelocity(double*,double*);

//POINTERS TO THE ACTIVE ROUTINES
//OF THE DRIFT ROUTINES
static int (*ADL_G_Setup_DRIFT)(char *);
static int (*ADL_G_Status_DRIFT)();
static void (*ADL_G_CalculateElectronVelocity)(double *, double *);
static void (*ADL_G_CalculateHoleVelocity)(double *, double *);

int SetupDrift(char*,char*);

//the obligatory setup file:
int ADL_Setup_DRIFT(char *, char *);
int ADL_Status_DRIFT(void);

//Specific versions of this library follow:

#endif /* ADL_DRIFT_H_ */
