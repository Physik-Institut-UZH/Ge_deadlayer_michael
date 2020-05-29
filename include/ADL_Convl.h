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

#ifndef ADL_CONVL_H_
#define ADL_CONVL_H_

#include "ADL.h"
//the different versions of this library:
#include "ADL_Convl_DUMMY.h"

//A global: an array of [0..nSeg] used in EACH of the CONVL versions
//Where we store 1/0 if the segment has an inverting preamp or not:
static int *ADL_G_INVERSE;

//POINTERS TO THE ACTIVE ROUTINES
//OF THE CONVL ROUTINS                                
static int (*ADL_G_Setup_CONVL)(char *);
static int (*ADL_G_Status_CONVL)();
static void (*ADL_G_Convolute)(struct ADL_TRACES *);

void ApplyConvolution(struct ADL_TRACES *);
int SetupConv(char*,char*);

//the obligatory setup file:
int ADL_Setup_CONVL(char *, char *);
int ADL_Status_CONVL(void);

#endif /* ADL_CONVL_H_ */
