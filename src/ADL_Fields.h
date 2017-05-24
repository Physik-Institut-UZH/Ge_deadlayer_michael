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

#ifndef ADL_FIELDS_H_
#define ADL_FIELDS_H_

#include "ADL.h"
#include "ADL_Parser.h"
//The different version of the libray:
#include "ADL_Fields_SIMION.h"

static double ADL_Fields_Height;

double GetHeight();
double* GetEField(double*);
double GetWeight(int,double*);
int GetDetectorPos(double*);
int IsInDetector(double*);

int SetupFields(char*);
void StatusFields();

//POINTERS TO THE ACTIVE ROUTINES
//OF THE FIELDS ROUTINES
static int (*ADL_G_Setup_FIELDS)(char *);
static int (*ADL_G_Status_FIELDS)();
static double (*ADL_G_Weight)(int, double *);
static int (*ADL_G_DetectorPos)(double *);
static int (*ADL_G_InDetector)(double *);
static int (*ADL_G_Efield)(double *, double *);
static int (*ADL_G_Wfield)(int , double *, double *);
static int (*ADL_G_Geo)(double *);

//the obligatory setup file:
int ADL_Setup_FIELDS(char *);
static int ADL_Status_FIELDS(void);
#endif /* ADL_FIELDS_H_ */
