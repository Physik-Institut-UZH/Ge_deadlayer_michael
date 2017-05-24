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

#include"ADL_Traces_NUMRES.h"
#include <stdio.h>
#include <stdlib.h>

//Globals influencing the integration routines:
double  ADL_G_NUMRES_EPS    = 1.0e-6;    //(routine RKQS) error tolerance for integration
int ADL_G_NUMRES_MAXSTP     = 10000;     //(routine odeint) maximum integration steps between 2 samples
double  ADL_G_NUMRES_TINY   = 1.0e-6;    //(routine odeint) small position difference [cm]
double  ADL_G_NUMRES_DTMIN  = 0.0;       //(routine odeint) minimum stepsize to take [us]
double  ADL_G_NUMRES_DT0    = 0.001;     //(routine odeint) first stepsize to try [us]
double  ADL_G_NUMRES_DIFF   = 0.;        // initial charge cloud size [mm]

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

int InitializeTracesNumres(){
    
    ADL_G_NUMRES_XYZe  = NULL;      //needed for storing path of electrons
    ADL_G_NUMRES_XYZh  = NULL;      //needed for storing path of holes

    return 0;
}

double** GetNUMRES_XYZe(){return ADL_G_NUMRES_XYZe;}
double** GetNUMRES_XYZh(){return ADL_G_NUMRES_XYZh;}

/****************************************************************************/
/* allocate a double matrix with subscript range m[0..nrh][0..nch] */
static double **ADL_TRACES_matrix(int nrh,int nch)
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

//Here the setup file for this library version:
int ADL_Setup_TRACES_NUMRES(char *filename_setupfile){
  int	i,len,err=1;
  struct	ADL_KEYWORD **Kwords;
  
  InitializeTracesNumres();
  
  //stepsize should be of the order of DeltaT by default:
  ADL_G_NUMRES_DT0 = GetDeltaT();
  
  //get list of keywords from file:
  if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
    Kwords = ADL_parse_file (filename_setupfile);
    
    len = Kwords[1]->linenumber;
    err = Kwords[len+2]->linenumber;
    //give keywords parsed values:
    for (i=0;i<len;i++) {
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_EPS")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_NUMRES_EPS);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_MAXSTP")==0) 
	sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_NUMRES_MAXSTP);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_TINY")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_NUMRES_TINY);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_DTMIN")==0) 
	sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_NUMRES_DTMIN);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_DT0")==0)
    sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_NUMRES_DT0);
      if (strcmp(Kwords[2+i]->keyword,"ADL_G_NUMRES_DIFF")==0)
    sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_NUMRES_DIFF);
    }
    free(Kwords);
  }
  
  //free XYZe, XYZh if they existed already:
  if (ADL_G_NUMRES_XYZe != NULL){
    printf("DEBUG: ADL_Setup_TRACES_NUMRES");
    free(ADL_G_NUMRES_XYZe);
  }
  if (ADL_G_NUMRES_XYZh != NULL) free(ADL_G_NUMRES_XYZh);
  //creation of XYZe, XYZh:
  ADL_G_NUMRES_XYZe = ADL_TRACES_matrix(GetDIMT(),3);
  ADL_G_NUMRES_XYZh = ADL_TRACES_matrix(GetDIMT(),3);
  
  if (GetADLDebug()) ADL_Status_TRACES_NUMRES();
  return err;
}

int ADL_Status_TRACES_NUMRES(void){
  //print final values that are setup:
  printf("\nStatus TRACES NUMRES:\n");
  printf("---------------------\n");
  printf("ADL_G_NUMRES_EPS    = %lf\n",ADL_G_NUMRES_EPS);
  printf("ADL_G_NUMRES_MAXSTP = %d\n", ADL_G_NUMRES_MAXSTP);
  printf("ADL_G_NUMRES_TINY   = %lf\n",ADL_G_NUMRES_TINY);
  printf("ADL_G_NUMRES_DTMIN  = %lf\n",ADL_G_NUMRES_DTMIN);
  printf("ADL_G_NUMRES_DT0    = %lf\n",ADL_G_NUMRES_DT0);
  printf("ADL_G_NUMRES_DIFF   = %lf\n",ADL_G_NUMRES_DIFF);
  printf("Depends on: EVENT, FIELDS, DRIFT, CONVL\n");
  if ((ADL_G_NUMRES_XYZe == NULL) || (ADL_G_NUMRES_XYZh == NULL))
    printf("WARNING: ADL_G_NUMRES_XYZe,h requires setup!!!\n");
  return 0;
  
}


//****************************************************************************

void ADL_ElectronVelocity(double tt, double P[], double dPdt[])
	//used in Runge-Kutta integrator : here the electron velocities are calculated
	{
	if (IsInDetector(P)) {
        double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
        GetElectronVelocity(E,dPdt);
	    //dPdt = Ve, electron drift velocity in [cm/us] !!!
}
	else dPdt[1] = dPdt[2] = dPdt[3] = 0.0; 
		//does not make sense to track particles outside detector...
	}
//****************************************************************************

void ADL_HoleVelocity(double tt, double P[], double dPdt[])
	//Is the same as the routine above, except for the call in (*)
	{
	if (IsInDetector(P)) {
        double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
        GetHoleVelocity(E,dPdt);
	    //dPdt = Vh, hole drift velocity in [cm/us] !!!
    }
	else dPdt[1] = dPdt[2] = dPdt[3] = 0.0; 
	//does not make sense to track particles outside detector...
	}

//****************************************************************************

int ADL_CalculatePath_NUMRES(double **Y, double *Y0, int nt,            //the output vector (dim nt+1) and starting values Y0
							     double dt, double t0,                          //the initial stepsize and trigger time
							     void (*derivs)(double, double [], double []) ) //the derivative function (ElectronVelocity or HoleVelocity)
	{
        static int i,j;					//number of steps taken
	static double Yout[4],t1,t2 = 0.0;	//initial and final time of step
	static int nok,nbad;			//for stepsize efficiency monitoring

	Y[0][1] = Yout[1] = Y0[0];			//copy starting position:
	Y[0][2] = Yout[2] = Y0[1];
	Y[0][3] = Yout[3] = Y0[2];

	t2 = -t0;	//starting time : t = t0 (derivs(t) = 0 if t<0)
        

    for (i=1; i<= nt; i++) {
		t1 = t2;		
		t2 += GetDeltaT();

		if (t2>0) {// We have to calculate the trajectory from t1 to t2
			if (t1<0) t1=0; //Holes and electrons are created from t=0;
			//calculate what happens from t1 to t2:
			if (NUMRES_odeint(Yout,3,t1,t2,ADL_G_NUMRES_EPS,dt,ADL_G_NUMRES_DTMIN,&nok,&nbad,derivs,NUMRES_rkqs,ADL_G_NUMRES_MAXSTP,ADL_G_NUMRES_TINY)) return -1;
		}

        //copy solution to array
        for (j=1;j<=3;j++) Y[i][j] = Yout[j];
    }
return 0;
}

//****************************************************************************
// MAIN FUNCTION
//****************************************************************************

int ADL_CalculateTraces_NUMRES(struct ADL_EVENT *evnt)
	//this routine needs the T0, Eint and Pos part of evnt filled in
	//and needs some allocated memory Ye(electron path),Yh(hole path)
	//on exit, the fields Eseg and Tr of evnt are given back.
	{
	static int i,j,k,errormsg = 0;

	//initialize Traces to 0.0
	for (j=0; j<GetNSEG(); j++)
		for (k=0; k<=GetDIMT(); k++)
			evnt->TD.Tr[j][k] = 0.0;

        //For all interactions ...
        for (i=0; i<GetNINT(); i++){
            double Pint[4]={0,evnt->HP.Pos[i][0],evnt->HP.Pos[i][1],evnt->HP.Pos[i][2]};
 
            if(IsInDetector(Pint)){
                //Calculate all the traces from the composite interactions
                if (evnt->HP.Eint[i] > 0)
                {
                    //calculate the path of the electrons
                    if (ADL_CalculatePath_NUMRES(ADL_G_NUMRES_XYZe,&(evnt->HP.Pos[i][0]),GetDIMT(),ADL_G_NUMRES_DT0,evnt->HP.T0,ADL_ElectronVelocity)) return 1;
                    //calculate	the path of the holes
                    if (ADL_CalculatePath_NUMRES(ADL_G_NUMRES_XYZh,&(evnt->HP.Pos[i][0]),GetDIMT(),ADL_G_NUMRES_DT0,evnt->HP.T0,ADL_HoleVelocity)) return 1;
                    //calculate the induced charge in segment j:
                    for (j=0; j<GetNSEG(); j++)
                        for (k=0; k<=GetDIMT(); k++)
                            evnt->TD.Tr[j][k] += (evnt->HP.Eint[i])*(GetWeight(j,ADL_G_NUMRES_XYZh[k])-GetWeight(j,ADL_G_NUMRES_XYZe[k]));
                    //preamp sign has to be taken into account by inverting the ADL_G_TRACES_NUMRES_Weightingfields or responses
                }
            }
        }

    ApplyConvolution(&(evnt->TD));
	
        //Add non-ideal measurement effects: convolution with response functions
	//Time shifts and crosstalk can all be implemented in the Response ...

        evnt->TD.Trig = (int) GetTime(evnt);
    //After adding response, set the core trigger time.

        for (j=0; j<GetNSEG(); j++) evnt->TD.Eseg[j] = evnt->TD.Tr[j][GetDIMT()];
	//Remark: the "energy" calculated here is a very simplistic one
	//It is not yet influenced by the preamplifier response,
	//and only approximates what the energy filter would yield.
	//(which cannot be simulated as the simulated traces are usually too short for that

	return 0; //return without errors.
	}
