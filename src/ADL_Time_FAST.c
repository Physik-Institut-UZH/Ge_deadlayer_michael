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

#include "ADL_Time.h"
#include "ADL_Event.h"

//setup***************************************************************************

int InitializeTimeFast(int&TIME_CHAN,int&TIME_WIDTH,int&TIME_GAP,int&TIME_BEGIN,int&TIME_END){
    TIME_CHAN  = 0;					//Defines from which channel time is derived
    TIME_WIDTH = 1;                   //Trigger Filter Length [in ticks]
    TIME_GAP	 = 0;                   //Trigger Filter Gap [in ticks]
    TIME_BEGIN = 0;                   //Beginposition in Trace for search trigger
    TIME_END   = 100;					//Endposition in Trace for search trigger
    
    return 0;
}

int ADL_Setup_TIME_FAST(char *filename_setupfile){

	int	i,len,err=1;
	struct	ADL_KEYWORD **Kwords;

    InitializeTimeFast(ADL_G_TIME_CHAN,ADL_G_TIME_WIDTH,ADL_G_TIME_GAP,ADL_G_TIME_BEGIN,ADL_G_TIME_END);
    
	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TIME_CHAN")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TIME_CHAN);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TIME_WIDTH")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TIME_WIDTH);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TIME_GAP")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TIME_GAP);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TIME_BEGIN")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TIME_BEGIN);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TIME_END")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TIME_END);
			}
		} //end parsing file
	if (GetADLDebug()) ADL_Status_TIME_FAST();
	return err;
	}


int ADL_Status_TIME_FAST(void){
	//print final values that are setup:
	printf("\nStatus TIME_FAST:\n");
	printf("-----------------\n");
	//only two parameters to setup here:
	printf("ADL_G_TIME_CHAN  = %d\n",ADL_G_TIME_CHAN);
	printf("ADL_G_TIME_WIDTH = %d\n",ADL_G_TIME_WIDTH);
	printf("ADL_G_TIME_GAP   = %d\n",ADL_G_TIME_GAP);
	printf("ADL_G_TIME_BEGIN = %d\n",ADL_G_TIME_BEGIN);
	printf("ADL_G_TIME_END   = %d\n",ADL_G_TIME_END);
	return 0;
}

//********************************************************************************

double ADL_TIME_FAST(struct ADL_EVENT *evt){
	//Returns the trigger time [sample postition] for the trace in arg1.
	//A short filter [ADL_G_TIME_WIDTH,ADL_G_TIME_GAP] is used to find a trigger time value.
	int PosMax,i,len;
	double Filtervalue,previous,FilterMax;
    
	PosMax = ADL_G_TIME_BEGIN;
	len = 2*ADL_G_TIME_WIDTH-ADL_G_TIME_GAP;
	if (ADL_G_TIME_BEGIN < len) ADL_G_TIME_BEGIN = len; //prevents reading outside string

	previous = FilterMax = 0;
	for (i=ADL_G_TIME_BEGIN;i<=ADL_G_TIME_END;i++)
		{
		Filtervalue = previous + evt->TD.Tr[ADL_G_TIME_CHAN][i]
			                   - evt->TD.Tr[ADL_G_TIME_CHAN][i-ADL_G_TIME_WIDTH]
							   - evt->TD.Tr[ADL_G_TIME_CHAN][i-ADL_G_TIME_WIDTH-ADL_G_TIME_GAP]
							   + evt->TD.Tr[ADL_G_TIME_CHAN][i-len];
		previous = Filtervalue;
		if (Filtervalue > FilterMax) {FilterMax = Filtervalue; PosMax = i;};
		}
    return PosMax;
	}
