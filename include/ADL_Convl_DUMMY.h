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

#ifndef ADL_CONVL_DUMMY_H_
#define ADL_CONVL_DUMMY_H_

#include "ADL.h"
#include "ADL_Event.h"
#include "ADL_Parser.h"

//DUMMY routines:
//Does nothing but inverting some segments:
int ADL_Setup_CONVL_DUMMY( char *);
int ADL_Status_CONVL_DUMMY(void);
void ADL_Convolute_DUMMY(struct ADL_TRACES *);

double GetRCtau();

#endif /* ADL_CONVL_DUMMY_H_ */
