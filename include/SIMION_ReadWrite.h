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

#ifndef SIMION_READWRITE_H
#define SIMION_READWRITE_H

#include "ADL.h"

int SIMION_Setup_READWRITE(char *, char *);
int SIMION_Status_READWRITE(void);
struct SIMION_PA * SIMION_READ(char *,double );
int SIMION_WRITE(struct SIMION_PA *);						//writes all pa's in the structure
int SIMION_WRITE_PA(struct SIMION_PA *, int , char *);		//writes a single pa from the structure with supplied filename. 


/* internal functions for editing PAs */
void SIMION_set_array_boundary(int *, struct SIMION_HEADER *);
double * SIMION_read_in_pot_array(char *,struct SIMION_HEADER *);

#endif /* SIMION_READWRITE_H */




