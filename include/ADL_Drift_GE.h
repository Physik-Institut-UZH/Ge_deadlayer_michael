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

#ifndef ADL_DRIFT_GE_H_
#define ADL_DRIFT_GE_H_

#include "ADL.h"
#include "ADL_Parser.h"

//GE routines:
int ADL_Setup_DRIFT_GE( char *);
int ADL_Status_DRIFT_GE(void);
void ADL_CalculateElectronVelocity_GE(double *, double *);
void ADL_CalculateHoleVelocity_GE(double *, double *);

#endif /* ADL_DRIFT_GE_H_ */
