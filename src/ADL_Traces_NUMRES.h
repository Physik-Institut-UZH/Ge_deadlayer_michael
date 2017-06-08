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

#ifndef ADL_TRACES_NUMRES_H
#define ADL_TRACES_NUMRES_H

#include "ADL.h"
#include "ADL_Event.h"
#include "ADL_Parser.h"
#include "ADL_Convl.h"
#include "ADL_Time.h"
#include "ADL_Fields.h"
#include "ADL_Drift.h"
#include "ADL_NumRes.h"
#include "ADL_Fields.h"

int InitializeTracesNumres();

double GetCloudRMS();
double** GetNUMRES_XYZe();
double** GetNUMRES_XYZh();

static double **ADL_G_NUMRES_XYZe;      //needed for storing path of electrons
static double **ADL_G_NUMRES_XYZh;      //needed for storing path of holes

//Routines in version TRACES_NUMRES:
int ADL_Setup_TRACES_NUMRES(char *);
int ADL_Status_TRACES_NUMRES(void);
int ADL_CalculateTraces_NUMRES(struct ADL_EVENT *);

#endif /* ADL_TRACES_NUMRES_H */
