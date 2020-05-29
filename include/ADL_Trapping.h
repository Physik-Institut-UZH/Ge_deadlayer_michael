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

#ifndef ADL_TRAPPING_H
#define ADL_TRAPPING_H

#include "ADL.h"
//the different versions of this library:
#include "ADL_Trapping_FIELD.h" //field dependent (hole trapping model)

int SetupTrapping(char*,char*);

static double **ADL_G_TRAPPING_XYZe;      //needed for storing path of electrons
static double **ADL_G_TRAPPING_XYZh;      //needed for storing path of holes

//POINTERS TO THE ACTIVE TRAPPING RATES:           
static double (*ADL_G_ElectronTrappingRate)(double []);
static double (*ADL_G_HoleTrappingRate)(double []);

//Routines in version TRACES_NUMRES:
int ADL_Setup_TRAPPING(char *, char *);
int ADL_Status_TRAPPING(void);
int ADL_CalculateTrappingSensitivity(int /*segnr*/, double * /*position*/, double * /*Se*/, double * /*Sh*/);

#endif /* ADL_TRAPPING_H */
