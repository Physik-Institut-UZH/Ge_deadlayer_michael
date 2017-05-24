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

#include"ADL_Trapping.h"

//Globals influencing the integration routines:
double  ADL_G_TRAPPING_EPS    = 1.0e-6;    //(routine RKQS) error tolerance for integration
int ADL_G_TRAPPING_MAXSTP = 10000;     //(routine odeint) maximum integration steps between 2 samples
double  ADL_G_TRAPPING_TINY   = 1.0e-6;    //(routine odeint) small position difference [cm]
double  ADL_G_TRAPPING_DTMIN  = 0.0;       //(routine odeint) minimum stepsize to take [us]
double  ADL_G_TRAPPING_DT0    = 0.001;     //(routine odeint) first stepsize to try [us]

static int ADL_TRAPPING_segnr = 0;     //Used for selecting the segment one calculates the sensitivity for... 
									       // see ADL_ElectronTrapping, ADL_holeTrapping and ADL_CalculateTrappingSensitivity

int SetupTrapping(char *libversion, char *setupfile){return ADL_Setup_TRAPPING(libversion,setupfile);}

//++++++++++++++++++++++++++++++++++++++++++
// LIBRARY SETUP FUNCTION				
//++++++++++++++++++++++++++++++++++++++++++

/****************************************************************************/
/* allocate a double matrix with subscript range m[0..nrh][0..nch] */
static double **ADL_TRAPPING_matrix(int nrh,int nch)
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
int ADL_Setup_TRAPPING(char *libversion, char *filename_setupfile){
	int	i,len,err=1;
	struct	ADL_KEYWORD **Kwords;
    
    ADL_G_ElectronTrappingRate      = ADL_ElectronTrappingRate_FIELD;
    ADL_G_HoleTrappingRate          = ADL_HoleTrappingRate_FIELD;
    ADL_G_TRAPPING_XYZe             = NULL;      //needed for storing path of electrons
    ADL_G_TRAPPING_XYZh             = NULL;      //needed for storing path of holes
    
	//stepsize should be of the order of DeltaT by default:
	ADL_G_TRAPPING_DT0 = GetDeltaT();

	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);

		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		//give keywords parsed values:
		for (i=0;i<len;i++) {
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TRAPPING_EPS")==0)
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_TRAPPING_EPS);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TRAPPING_MAXSTP")==0) 
				sscanf(Kwords[2+i]->svalue,"%d",&ADL_G_TRAPPING_MAXSTP);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TRAPPING_TINY")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_TRAPPING_TINY);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TRAPPING_DTMIN")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_TRAPPING_DTMIN);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_TRAPPING_DT0")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_TRAPPING_DT0);
				}
		free(Kwords);
	}

	//free XYZe, XYZh if they existed already:
	if (ADL_G_TRAPPING_XYZe != NULL) free(ADL_G_TRAPPING_XYZe);
	if (ADL_G_TRAPPING_XYZh != NULL) free(ADL_G_TRAPPING_XYZh);
	//creation of XYZe, XYZh:
	ADL_G_TRAPPING_XYZe = ADL_TRAPPING_matrix(GetDIMT(),5);
	ADL_G_TRAPPING_XYZh = ADL_TRAPPING_matrix(GetDIMT(),5);

	if (GetADLDebug()) ADL_Status_TRAPPING();
	return err;
}

	int ADL_Status_TRAPPING(void){
		//print final values that are setup:
		printf("\nStatus TRAPPING:\n");
		printf("----------------\n");
		printf("ADL_G_ElectronTrappingRate  -> ADL_ElectronTrappingRate_FIELD\n");
        printf("ADL_G_HoleTrappingRate      -> ADL_HoleTrappingRate_FIELD\n");
		printf("ADL_G_TRAPPING_EPS          = %lf\n",ADL_G_TRAPPING_EPS);
		printf("ADL_G_TRAPPING_MAXSTP       = %d\n", ADL_G_TRAPPING_MAXSTP);
		printf("ADL_G_TRAPPING_TINY         = %lf\n",ADL_G_TRAPPING_TINY);
		printf("ADL_G_TRAPPING_DTMIN        = %lf\n",ADL_G_TRAPPING_DTMIN);
		printf("ADL_G_TRAPPING_DT0          = %lf\n",ADL_G_TRAPPING_DT0);
		printf("Depends on: EVENT, FIELDS\n");
		if ((ADL_G_TRAPPING_XYZe == NULL) || (ADL_G_TRAPPING_XYZh == NULL))
			printf("WARNING: ADL_G_TRAPPING_XYZe,h requires setup!!!\n");
		return 0;

	}


//****************************************************************************

static void ADL_ElectronTrapping(double tt, double P[], double dPdt[])
	//used in Runge-Kutta integrator : for calculation of electron trapping sensitivity
	{
      
	if (IsInDetector(P))
		{
         double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
										//Calculate Field strength (needed for drift velocity)
		GetElectronVelocity(E,dPdt);				//dPdt[1..3] = Ve, electron drift velocity in [cm/us] !!!
		dPdt[4] = ADL_G_ElectronTrappingRate(P);				//dPdt[4] = TrappingRate --> P[4] is Integral_0^t(Trappingrate dt)
		dPdt[5] = GetWeight(ADL_TRAPPING_segnr,P)*dPdt[4];   //dPdt[5] = dsensitivity/dt version 2
		}
	else dPdt[1] = dPdt[2] = dPdt[3] = dPdt[4] = dPdt[5] = 0.0; 
	//does not make sense to track particles outside detector...
    }

//****************************************************************************

static void ADL_HoleTrapping(double tt, double P[], double dPdt[])
	//used in Runge-Kutta integrator : for calculation of hole trapping sensitivity
	{
	if (IsInDetector(P))
		{
        double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
										//Calculate Field strength (needed for drift velocity)
		GetHoleVelocity(E,dPdt);					//dPdt[1..3] = Ve, electron drift velocity in [cm/us] !!!
		dPdt[4] = ADL_G_HoleTrappingRate(P);					//dPdt[4] = TrappingRate --> P[4] is Integral_0^t(Trappingrate dt)
		dPdt[5] = GetWeight(ADL_TRAPPING_segnr,P)*dPdt[4];	//dPdt[5] = dsensitivity/dt version 2
		}
	else dPdt[1] = dPdt[2] = dPdt[3] = dPdt[4] = dPdt[5] = 0.0; 
	//does not make sense to track particles outside detector...

	}

//****************************************************************************

static int ADL_CalculateTrapping(double **Y, double *Y0, int nt,   //the output vector (dim nt+1) and starting values Y0
			  double dt, double t0,                                        //the stepsize and trigger time
			  void (*derivs)(double, double [], double []))                //the derivative function (ElectronTrapping or HoleTrapping)
	{
	static int i,j;                    //number of steps taken
	static double Yout[7],t1,t2 = 0.0;     //initial and final time of step
	static int nok,nbad;               //for stepsize efficiency monitoring

	Y[0][1] = Yout[1] = Y0[0];             //copy starting position:
	Y[0][2] = Yout[2] = Y0[1];
	Y[0][3] = Yout[3] = Y0[2];
	Y[0][4] = Yout[4] = 0.0;
	Y[0][5] = Yout[5] = 0.0;

	t2 = -t0;						//starting time : t = t0 (derivs(t) = 0 if t<0)
	for (i=1; i<= nt; i++) {
		t1 = t2;		
		t2 += GetDeltaT();

		if (t2>0) {// We have to calculate the trajectory from t1 to t2
			if (t1<0) t1=0; //Holes and electrons are created from t=0;
			//calculate what happens from t1 to t2:
			if (NUMRES_odeint(Yout,5,t1,t2,ADL_G_TRAPPING_EPS,dt,ADL_G_TRAPPING_DTMIN,&nok,&nbad,derivs,NUMRES_rkqs,ADL_G_TRAPPING_MAXSTP,ADL_G_TRAPPING_TINY)) return -1;
		}

		//copy solution to array
		for (j=1;j<=5;j++) Y[i][j] = Yout[j];
		}
	return 0;
	}

//****************************************************************************
// MAIN FUNCTION
//****************************************************************************

int ADL_CalculateTrappingSensitivity(int segnr, double *Position, double *sensity_e, double *sensity_h)
	//calculates  TRAPPING "sensitivities":
	{

	//Passing on the segment number:
	ADL_TRAPPING_segnr = segnr;

	//calculate sensitivity along the path of the electrons
	if (ADL_CalculateTrapping(ADL_G_TRAPPING_XYZe,Position,GetDIMT(),GetDeltaT(),0.0,ADL_ElectronTrapping)) return 1;
	sensity_e[0] = ADL_G_TRAPPING_XYZe[GetDIMT()][4]; // I_e [-] total amount of electrons trapped
	sensity_e[1] = // n_e [cm] electron trapping sensitivity
		-fabs(ADL_G_TRAPPING_XYZe[GetDIMT()][5] - ADL_G_TRAPPING_XYZe[GetDIMT()][4]*GetWeight(segnr,ADL_G_TRAPPING_XYZe[GetDIMT()]));

	//calculate sensitivity along the path of the holes
	if (ADL_CalculateTrapping(ADL_G_TRAPPING_XYZh,Position,GetDIMT(),GetDeltaT(),0.0,ADL_HoleTrapping)) return 1;
	sensity_h[0] = ADL_G_TRAPPING_XYZh[GetDIMT()][4]; // I_h [-] total amount of holes trapped
	sensity_h[1] = // n_h [cm] hole trapping sensitivity
		-fabs(ADL_G_TRAPPING_XYZh[GetDIMT()][5] - ADL_G_TRAPPING_XYZh[GetDIMT()][4]*GetWeight(segnr,ADL_G_TRAPPING_XYZh[GetDIMT()]));

	//Some output:
	if (GetADLDebug()) {
		printf("\nTrapping Sensitivity Position %le %le %le Seg %d",Position[0],Position[1],Position[2],segnr);
		printf(" e sensitivities: %le %le ",sensity_e[0],sensity_e[1]);
		printf(" h sensitivities: %le %le ",sensity_h[0],sensity_h[1]);
	}

	return 0; //return without errors.
	}
