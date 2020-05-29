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

#ifndef ADL_TIME_H_
#define ADL_TIME_H_

#include "ADL.h"
#include "ADL_Parser.h"
#include "ADL_Event.h"
//different versions of this library:
#include "ADL_Time_FAST.h"

double GetTime(struct ADL_EVENT *evt);
int SetupTime(char*,char*);

//the obligatory setup file:
int ADL_Setup_TIME(char *, char *);
int ADL_Status_TIME(void);

////Specific versions of this library follow:
//
////FAST routines:
////Does nothing but inverting some segments:
//int ADL_Setup_TIME_FAST( char *);
//int ADL_Status_TIME_FAST(void);
//double ADL_TIME_FAST(struct ADL_EVENT *);


//POINTERS TO THE ACTIVE ROUTINES
//OF THE TIME ROUTINS                                 + initialization:
static int (*ADL_G_Setup_TIME)(char *);//            = ADL_Setup_TIME_FAST;
static int (*ADL_G_Status_TIME)();//                 = ADL_Status_TIME_FAST;
static double  (*ADL_G_TIME)(struct ADL_EVENT *);//      = ADL_TIME_FAST;

#endif /* ADL_TIME_H_ */
