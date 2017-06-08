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

//**********************************
#ifndef ADL_EVENT_H_
#define ADL_EVENT_H_

#include "ADL.h"
#include "ADL_Parser.h"

//**********************************
//List of global defined parameters:
static int    ADL_G_EVENT_NINT;       //nInt:maximum number of interactions assumed
static int    ADL_G_EVENT_NSEG;       //nSeg:number of segments in play (including core)
static int    ADL_G_EVENT_DIMT;      //dimT:trace length [0...dimT]!!!
static double     ADL_G_EVENT_DeltaT; //[us]:Sampling interval to store traces Tr[nSeg][dimT+1]

int SetupEvent(char*,char*);

void SetNINT(int);
int GetNINT();
int GetNSEG();
int GetDIMT();
double GetDeltaT();


#ifndef ADL_HITS_
#define ADL_HITS_
struct ADL_HITS
    {
    double T0;          //time of interaction (in us)
    double *Eint;       //energy fractions
    double **Pos;       //interaction positions
    };
#endif /* ADL_HITS_ */

#ifndef ADL_TRACES_
#define ADL_TRACES_
struct ADL_TRACES
        {
    int Trig;       //Core trigger time (sample index)
    double *Eseg;       //energy in segments (0=core)
    double **Tr;        //nSeg traces (dimT ticks)
    };
#endif /* ADL_TRACES_ */

#ifndef ADL_EVENT_
#define ADL_EVENT_
struct ADL_EVENT
    {
    struct ADL_HITS    HP;  //Parameters on Hits
    struct ADL_TRACES  TD;  //Parameters on Traces
    };
#endif /* ADL_EVENT_ */


int ADL_Setup_EVENT(char*, char*);
int ADL_Status_EVENT(void);
struct ADL_HITS   *new_hits(void);
struct ADL_TRACES *new_traces(void);
struct ADL_EVENT  *new_event(void);

#endif /* ADL_EVENT_H_ */
