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

#ifndef ADL_TIME_FAST_H_
#define ADL_TIME_FAST_H_

#include "ADL.h"

int InitializeTimeFast(int&,int&,int&,int&,int&);

//Global defines for this library version:
static int ADL_G_TIME_CHAN;        //Defines from which channel time is derived
static int ADL_G_TIME_WIDTH;       //Trigger Filter Length [in ticks]
static int ADL_G_TIME_GAP;         //Trigger Filter Gap [in ticks]
static int ADL_G_TIME_BEGIN;       //Beginposition in Trace for search trigger
static int ADL_G_TIME_END;         //Endposition in Trace for search trigger

//FAST routines:
//Does nothing but inverting some segments:
int ADL_Setup_TIME_FAST( char *);
int ADL_Status_TIME_FAST(void);
double ADL_TIME_FAST(struct ADL_EVENT *);

#endif /* ADL_TIME_FAST_H_ */
