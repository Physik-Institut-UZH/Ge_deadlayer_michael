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

#include "ADL_Event.h"

//**********************************
//List of global defined parameters:

void SetNINT(int NINT){ADL_G_EVENT_NINT = NINT;}
int GetNINT(){return ADL_G_EVENT_NINT;}
int GetNSEG(){return ADL_G_EVENT_NSEG;}
int GetDIMT(){return ADL_G_EVENT_DIMT;}
double GetDeltaT(){return ADL_G_EVENT_DeltaT;}
int SetupEvent(char *libversion, char *setupfile){return ADL_Setup_EVENT(libversion,setupfile);}

/****************************************************************************/
/* The obligatory setup file                                                */
int ADL_Setup_EVENT(char *lib_version,char *filename_setupfile){
	int		i,len,err=1;
	struct	ADL_KEYWORD **Kwords;

    ADL_G_EVENT_NINT = 1;       //nInt:maximum number of interactions assumed
    ADL_G_EVENT_NSEG = 2;       //nSeg:number of segments in play (including core)
    ADL_G_EVENT_DIMT = 99;      //dimT:trace length [0...dimT]!!!
    ADL_G_EVENT_DeltaT = 0.001; //[us]:Sampling interval to store traces Tr[nSeg]
	// lib_version is ignored, because there is only one at this moment...

	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
            if (strcmp(Kwords[2+i]->keyword,"ADL_G_EVENT_NINT")==0)
                sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_EVENT_NINT);
            if (strcmp(Kwords[2+i]->keyword,"ADL_G_EVENT_NSEG")==0)
                sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_EVENT_NSEG);
            if (strcmp(Kwords[2+i]->keyword,"ADL_G_EVENT_DIMT")==0)
                sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_EVENT_DIMT);
            if (strcmp(Kwords[2+i]->keyword,"ADL_G_EVENT_DeltaT")==0)
                sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_EVENT_DeltaT);
			}
		} //end parsing file

	if (GetADLDebug()) ADL_Status_EVENT();
	return err;
	}

int ADL_Status_EVENT(void){
	//print values that are setup:
	printf("\nStatus EVENT (v0):\n");
	printf("------------------\n");
	//only one parameter to setup here:
    printf("ADL_G_EVENT_NINT   = %d\n",ADL_G_EVENT_NINT);
    printf("ADL_G_EVENT_NSEG   = %d\n",ADL_G_EVENT_NSEG);
    printf("ADL_G_EVENT_DIMT   = %d\n",ADL_G_EVENT_DIMT);
    printf("ADL_G_EVENT_DeltaT = %lf\n",ADL_G_EVENT_DeltaT);
	return 0;
}


/****************************************************************************/
/* allocate a double matrix with subscript range m[0..nrh][0..nch] */
static double **ADL_EVENT_matrix(int nrh,int nch)
{
	int i, nrow=nrh+1,ncol=nch+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) calloc((nrow+1),sizeof(double*));
	if (!m)    printf("\nADL_EVENT_Matrx: allocation failure 1 in matrix\n");
	m ++;

	/* allocate rows and set pointers to them */
	m[0]=(double *) calloc((nrow*ncol+1),sizeof(double));
	if (!m[0]) printf("\nADL_EVENT_Matrx: allocation failure 2 in matrix\n");
	m[0]++;

	for(i=1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}


/****************************************************************************/
/* Dynamic allocation of a ADL_HITS structure                               */
struct ADL_HITS *new_hits(void){
	struct ADL_HITS *returnvalue;
	returnvalue = (struct ADL_HITS *) calloc(1,sizeof(struct ADL_HITS));
	returnvalue->Eint = (double *) calloc(ADL_G_EVENT_NINT,sizeof(double));
	returnvalue->Pos  = ADL_EVENT_matrix(ADL_G_EVENT_NINT,3);
	return returnvalue;
	}

/****************************************************************************/
/* Dynamic allocation of a ADL_TRACES structure                             */
struct ADL_TRACES *new_traces(void){
	struct ADL_TRACES *returnvalue;
	returnvalue = (struct ADL_TRACES *) calloc(1,sizeof(struct ADL_TRACES));
	returnvalue->Eseg = (double *) calloc(GetNSEG(),sizeof(double));
	returnvalue->Tr  = ADL_EVENT_matrix(GetNSEG(),GetDIMT());
	return returnvalue;
	}

/****************************************************************************/
/* Dynamic allocation of a ADL_EVENT structure								*/
struct ADL_EVENT *new_event(void){
	struct ADL_EVENT *returnvalue;
	returnvalue = (struct ADL_EVENT *) calloc(1,sizeof(struct ADL_EVENT));
	//creating memory for HITS:
	returnvalue->HP.Eint = (double *) calloc(ADL_G_EVENT_NINT,sizeof(double));
	returnvalue->HP.Pos  = ADL_EVENT_matrix(ADL_G_EVENT_NINT,3);
	//creating memory for TRACES:
	returnvalue->TD.Eseg = (double *) calloc(GetNSEG(),sizeof(double));
	returnvalue->TD.Tr  = ADL_EVENT_matrix(GetNSEG(),GetDIMT());
	//This we return
	return returnvalue;
	}
