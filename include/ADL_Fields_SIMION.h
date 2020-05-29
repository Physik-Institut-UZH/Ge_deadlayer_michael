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

#ifndef ADL_FIELDS_SIMION_H_
#define ADL_FIELDS_SIMION_H_

#include "ADL.h"
#include "ADL_Parser.h"
#include "ADL_Event.h"
#include "SIMION_ReadWrite.h"

static double ADL_Fields_SIMION_Center;
static double ADL_Fields_SIMION_GridSize;
static double ADL_Fields_SIMION_Height;
static int ADL_Fields_SIMION_InDetector;

struct SIMION_PA *GetADLEpot();
struct SIMION_PA *GetADLWpot();
struct SIMION_PA *GetADLStru();

void SetADLEpot(struct SIMION_PA *);
void SetADLWpot(struct SIMION_PA *);
void SetADLStru(struct SIMION_PA *);

double GetSimionGridSize();
double GetSimionCenter();
double GetSimionHeight();

void SetSimionGridSize(double);
void SetSimionCenter(double);
void SetSimionHeight(double);

//SIMION routines:
int ADL_Setup_FIELDS_SIMION( char *);
int ADL_Status_FIELDS_SIMION(void);
double  ADL_Weight_SIMION(int, double *);
int ADL_DetectorPos_SIMION(double *);
int ADL_InDetector_SIMION(double *);
int ADL_Efield_SIMION(double *, double *);
int ADL_Wfield_SIMION(int , double *, double *);
int ADL_Geo_SIMION(double *);

#endif /* ADL_FIELDS_SIMION_H_ */
