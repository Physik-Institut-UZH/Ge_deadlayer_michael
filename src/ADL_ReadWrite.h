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

#ifndef ADL_READ_WRITE_H
#define ADL_READ_WRITE_H

#include "ADL.h"
//The different versions of this library:
#include "ADL_ReadWrite_TXT.h"

//POINTERS TO THE ACTIVE ROUTINES
//OF THE READ/WRITE ROUTINES

static int (*ADL_G_ReadFileHeader)(FILE *);
static int (*ADL_G_ReadEvent)(FILE *, struct ADL_EVENT *);
static int (*ADL_G_ReadEventHP)(FILE *, struct ADL_HITS *);
static int (*ADL_G_ReadEventTD)(FILE *, struct ADL_TRACES *);

static int (*ADL_G_WriteFileHeader)(FILE *);
static int (*ADL_G_WriteEvent)(FILE *, struct ADL_EVENT *);
static int (*ADL_G_WriteEventHP)(FILE *, struct ADL_HITS *);
static int (*ADL_G_WriteEventTD)(FILE *, struct ADL_TRACES *);

int SetupReadWrite(char*,char*);

int ADL_Setup_READWRITE(char *, char *);
int ADL_Status_READWRITE(void);

#endif /* ADL_READWRITE_ */




