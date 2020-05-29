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

#ifndef ADL_TRACES_H
#define ADL_TRACES_H

#include"ADL.h"
//Different version for this library:
#include "ADL_Traces_NUMRES.h"

void CalculateTraces(struct ADL_EVENT *);
void StatusTraces(struct ADL_EVENT *);
int SetupTraces(char*);

//POINTERS TO THE ACTIVE ROUTINES
//OF THE TRACES ROUTINES   
static int (*ADL_G_Setup_TRACES)(char *);
static int (*ADL_G_Status_TRACES)();
static int (*ADL_G_CalculateTraces)(struct ADL_EVENT *);

//Declaration of the library member functions:
static int ADL_Setup_TRACES(char *);
//int ADL_Status_TRACES(void);
static int ADL_Status_TRACES(struct ADL_EVENT *);

#endif /* ADL_TRACES_H */
