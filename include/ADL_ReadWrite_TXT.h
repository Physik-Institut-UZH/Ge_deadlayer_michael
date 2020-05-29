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

#ifndef ADL_READ_WRITE_TXT_H
#define ADL_READ_WRITE_TXT_H

#include "ADL.h"
#include "ADL_Event.h"

int ADL_ReadFileHeader_TXT(FILE *);
int ADL_ReadEvent_TXT(FILE *, struct ADL_EVENT *);
int ADL_ReadEventHP_TXT(FILE *, struct ADL_HITS *);
int ADL_ReadEventTD_TXT(FILE *, struct ADL_TRACES *);

int ADL_WriteFileHeader_TXT(FILE *);
int ADL_WriteEvent_TXT(FILE *, struct ADL_EVENT *);
int ADL_WriteEventHP_TXT(FILE *, struct ADL_HITS *);
int ADL_WriteEventTD_TXT(FILE *, struct ADL_TRACES *);

#endif /* ADL_READ_WRITE_TXT_H */




