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

#include "ADL_Drift_GE.h"

//***************************************
//***  LATTICE ORIENTATION PARAMETERS ***
//***************************************

//In general described by three Euler angles:
double ADL_G_LatticePhi   = 0;        //[rad] 
double ADL_G_LatticeTheta = 0;        //[rad]
double ADL_G_LatticePsi   = 0;        //[rad]
//We suppose that these angles describe the orientation of the crystal lattice,
//defined by the coordinate system with axes along three equivalent <100> directions.
//Tranformations from lab system to this lattice system can be done using matrix:
double ADL_G_LatticeRotation[3][3]={{0,0,0},{0,0,0},{0,0,0}};	
//Is initialized in ADL_Setup_DRIFT_GE					

//***************************************
//***   ELECTRON MOBILITY PARAMETERS  ***
//***************************************

//Mass tensor components and Gamma matrices
double ADL_G_Gamma[4][3][3]={
	{{0,0,0},{0,0,0},{0,0,0}},   
	{{0,0,0},{0,0,0},{0,0,0}},
	{{0,0,0},{0,0,0},{0,0,0}},
	{{0,0,0},{0,0,0},{0,0,0}}};          //Gamma matrices

//Definitions for mobility parametrisation in 100 direction
double ADL_G_E0e100  = 507.7;          //[V/cm]
double ADL_G_Be100   = 0.80422;        //[]
double ADL_G_Mu0e100 = 0.0371654;      //[cm2/Vus]
double ADL_G_MuNe100 = -0.0001447;     //[cm2/Vus]
#define Ve100(E)	( ADL_G_Mu0e100*(E)/(pow( (1+pow( ((E)/ADL_G_E0e100),ADL_G_Be100 )),(1.0/ADL_G_Be100) )) - ADL_G_MuNe100*(E) )

//Scattering Cross Section Parametrisation (alternative to defining V111):
double ADL_G_LnNu0 = 0.459;		
double ADL_G_LnNu1 = 0.0294;
double ADL_G_LnNu2 = 0.000054;
double ADL_G_E0    = 1200.0;
#define LnNu(E)		(ADL_G_LnNu0 + ADL_G_LnNu1*log((E)/ADL_G_E0) + ADL_G_LnNu2*log((E)/ADL_G_E0)*log((E)/ADL_G_E0))	//assumin log-linear dependence
#define nu(E)		(pow((E),LnNu(E)))			//isotropic scattering amplitude (not normalized.)


//***************************************
//***     HOLE MOBILITY PARAMETERS    ***
//***************************************

//Defining the V100 hole velocity
double ADL_G_E0h100 = 181.9;      //[V/cm]
double ADL_G_Bh100  = 0.73526;    //[]
double ADL_G_Muh100 = 0.062934;   //[cm2/Vus]
#define Vh100(E)	( ADL_G_Muh100*(E)/(pow((1+pow(((E)/ADL_G_E0h100),ADL_G_Bh100)),(1/ADL_G_Bh100)) ) )

//Defining the V111 hole velocity
double ADL_G_E0h111 = 143.9;      //[V/cm]
double ADL_G_Bh111  = 0.7488;     //[]
double ADL_G_Muh111 = 0.062383;   //[cm2/Vus]
#define Vh111(E)	( ADL_G_Muh111*(E)/(pow((1+pow(((E)/ADL_G_E0h111),ADL_G_Bh111)),(1/ADL_G_Bh111)) ) )

//Parametrisation from the model (cfr. MathCad file)
#define beta(k)		(0.01713 - 0.08091*(k) - 0.04821*(k)*(k))
#define alpha(k)	(-0.04027 + 0.19093*(k) + 0.05641*(k)*(k))
#define KaNul(V)	(11.8143-35.1249*(V)+39.59119*(V)*(V)-16.12157*(V)*(V)*(V))


//Need no explanation:	
double  ADL_G_SmallField = 1e-6;	//[V/cm]
double ADL_G_Temp = 77; // [K]

#ifndef PI_
#define PI			3.14159265358979
#endif  

#ifndef ADL_NORM_
#define ADL_NORM_

//extra definitions used here
#define scalar(a1,a2,a3,b1,b2,b3)	(a1*b1+a2*b2+a3*b3)
#define norm(a1,a2,a3)				(sqrt(scalar(a1,a2,a3,a1,a2,a3)))

#endif // ADL_NORM_


//Some Matrix algebra *********************************************************************


static void ADL_MultiplyMatrices(double Mat1[3][3], double Mat2[3][3], double Result[3][3])
	{
	int i,j,k;
	for (i=0;i<3;i++) for (j=0;j<3;j++)
			{
			Result[i][j] = 0.0;
			for (k=0;k<3;k++) Result[i][j] += Mat1[i][k]*Mat2[k][j];
			}
	}

static void ADL_Transpose(double Mat[3][3])
	{
	int i,j;
	double Hulp;
	for (i=0;i<3;i++)
		for (j=0;j<i;j++) //switch elements
			{Hulp = Mat[i][j]; Mat[i][j] = Mat[j][i]; Mat[j][i] = Hulp;}
	}

static void ADL_Matrix_x_Vector(double Matrix[3][3], double Vector[3], double Result[3])
	{
	int k,j;
	for (k=0;k<3;k++)
		{
		Result[k] = 0.0;
		for (j=0;j<3;j++) Result[k] += Matrix[k][j]*Vector[j];
		}
	}

static void ADL_RotationMatrix(double phi, double theta, double psi, double A[3][3])
	//For an understanding, see  "Euler Angles" on Mathworld in internet
	{
	A[0][0] = cos(psi)*cos(phi)-cos(theta)*sin(phi)*sin(psi);
	A[0][1] = cos(psi)*sin(phi)+cos(theta)*cos(phi)*sin(psi);
	A[0][2] = sin(psi)*sin(theta);

	A[1][0] = -sin(psi)*cos(phi)-cos(theta)*sin(phi)*cos(psi);
	A[1][1] = -sin(psi)*sin(phi)+cos(theta)*cos(phi)*cos(psi);
	A[1][2] = cos(psi)*sin(theta);

	A[2][0] = sin(theta)*sin(phi);
	A[2][1] = -sin(theta)*cos(phi);
	A[2][2] = cos(theta);
	}

//***************************************
//***  SetupDriftVelocities Routines  ***
//***************************************
//setup***************************************************************************

int ADL_Setup_DRIFT_GE(char *filename_setupfile){

	int	i,len,err=1;
	struct	ADL_KEYWORD **Kwords;
	double Mt   = 0.0819;    //the transverse
	double Ml   = 1.64;      //and longitudinal masses
	double Beta = 0.61548;   //[rad] : angle between <111> and <110> directions	
	double Rot[3][3],Hulp[3][3];
	double ADL_G_Gamma0[3][3]= { 
	{1.0/Mt ,    0.0      ,    0.0      },
	{0.0          , 1.0/Ml,    0.0      },
	{0.0          ,    0.0      , 1.0/Mt} };

	//get list of keywords from file:
	if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
		Kwords = ADL_parse_file (filename_setupfile);
		
		//overwrite keywords with parsed values:
		len = Kwords[1]->linenumber;
		err = Kwords[len+2]->linenumber;
		for (i=0;i<len;i++) {
			//Lattice Orientation Params
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LatticePhi")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LatticePhi);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LatticeTheta")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LatticeTheta);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LatticePsi")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LatticePsi);
			//Electron Mobility Params
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Mu0e100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Mu0e100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_E0e100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_E0e100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Be100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Be100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_MuNe100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_MuNe100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LnNu0")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LnNu0);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LnNu1")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LnNu1);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_LnNu2")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_LnNu2);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_E0")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_E0);
			//Hole Mobility Params
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_E0h100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_E0h100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Bh100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Bh100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Muh100")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Muh100);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_E0h111")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_E0h111);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Bh111")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Bh111);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Muh111")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Muh111);
			//Other Parameters
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_SmallField")==0) 
				sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_SmallField);
			if (strcmp(Kwords[2+i]->keyword,"ADL_G_Temp")==0)
			  sscanf(Kwords[2+i]->svalue,"%lf",&ADL_G_Temp);
			}
		} //end parsing file

	ADL_RotationMatrix(ADL_G_LatticePhi,ADL_G_LatticeTheta,ADL_G_LatticePsi,ADL_G_LatticeRotation);
	ADL_Transpose(ADL_G_LatticeRotation); // LatticeRotation x Vector(LAB system) = Vector(Lattice system)

	//Setup Gamma Matrices:
	//Gamma[i] = (TotalRotation(i))^T x Gamma0 x TotalRotation(i))
	//with TotalRotation(i) = lattice_to_valley_i x lab_to_lattice_system
	for (i=0;i<4;i++)
		{
		ADL_RotationMatrix((PI/4 + PI*i/2.0),Beta,0.0,Hulp);
		ADL_MultiplyMatrices(Hulp,ADL_G_LatticeRotation,Rot);
		ADL_MultiplyMatrices(ADL_G_Gamma0,Rot,Hulp);
		ADL_Transpose(Rot);
		ADL_MultiplyMatrices(Rot,Hulp,ADL_G_Gamma[i]);
		}

    if (GetADLDebug()) ADL_Status_DRIFT_GE();
	return err;
	}


int ADL_Status_DRIFT_GE(void){
	printf("\nStatus DRIFT GE:\n");
	printf("----------------\n");

	printf("Lattice orientation Euler angles:\n");
	printf("ADL_G_LatticePhi   = %le\n",ADL_G_LatticePhi);
	printf("ADL_G_LatticeTheta = %le\n",ADL_G_LatticeTheta);
	printf("ADL_G_LatticePsi   = %le\n",ADL_G_LatticePsi);
	printf("Or orientations in Lab system:");
	printf("\n<100>: (%le %le %le) ",ADL_G_LatticeRotation[0][0],ADL_G_LatticeRotation[0][1],ADL_G_LatticeRotation[0][2]);
	printf("\n<010>: (%le %le %le) ",ADL_G_LatticeRotation[1][0],ADL_G_LatticeRotation[1][1],ADL_G_LatticeRotation[1][2]);
	printf("\n<001>: (%le %le %le) ",ADL_G_LatticeRotation[2][0],ADL_G_LatticeRotation[2][1],ADL_G_LatticeRotation[2][2]);

	printf("\n: Electron Mobility Parameters:");
	printf("\nADL_G_E0e100  = %le",ADL_G_E0e100);
	printf("\nADL_G_Be100   = %le",ADL_G_Be100);
	printf("\nADL_G_Mu0e100 = %le",ADL_G_Mu0e100);
	printf("\nADL_G_MuNe100 = %le",ADL_G_MuNe100);
	printf("\nADL_G_LnNu0  = %le",ADL_G_LnNu0);
	printf("\nADL_G_LnNu1  = %le",ADL_G_LnNu1);
	printf("\nADL_G_LnNu2  = %le",ADL_G_LnNu2);
	printf("\nADL_G_E0     = %le",ADL_G_E0);

	printf("\n: Hole Mobility Parameters:");
	printf("\nADL_G_E0h100 = %le",ADL_G_E0h100);
	printf("\nADL_G_Bh100  = %le",ADL_G_Bh100);
	printf("\nADL_G_Muh100 = %le",ADL_G_Muh100);
	printf("\nADL_G_E0h111 = %le",ADL_G_E0h111);
	printf("\nADL_G_Bh111  = %le",ADL_G_Bh111);
	printf("\nADL_G_Muh111 = %le",ADL_G_Muh111);

	printf("\n: Other Parameters:");
	printf("\n ADL_G_SmallField = %le",ADL_G_SmallField);
	printf("\n ADL_G_Temp = %le",ADL_G_Temp);
	printf("\n");
	return 0;
}



//**************************************************************************

//********************************************
//***  CalculateElectronVelocity Routines  ***
//********************************************


void ADL_CalculateElectronVelocity_GE(double *Efield, double *Result)
	{
        // this routine uses the Gamma matrices Gamma[i], i=0,1,2,3
	// these global defined matrices are set up in SetupDriftVelocities.
	#define E100GammaE100	2.888470213		//constant Gamma0 in thesis. 

	double Gamma_E[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
	double E_Gamma_E, eps[4], Ni[4], g[4];
	double normE, Sum_Ni = 0.0;
	int i,j,k;

	normE = norm(Efield[1],Efield[2],Efield[3]);

	// Tentative to incorporate temperature dependance (herited from SigGen 2017: D. Radford)
	double T0 = 77;  //K
	double T1 = 200; //K
	double exp_e = -1.68;
	double B_e = 1.3e7; // cm/s
	double mu_e = 5.66e7; //cm2/Vs
	
	double mu_0_1 = mu_e * pow(T0, exp_e);
	double v_s_1 = B_e * sqrt(tanh(0.5 * T1 / T0));
	double E_c_1 = v_s_1 / mu_0_1;
	double mu_0_2 = mu_e * pow(ADL_G_Temp, exp_e);
	double v_s_2 = B_e * sqrt(tanh(0.5 * T1 / ADL_G_Temp));
	double E_c_2 = v_s_2 / mu_0_2;
        
	double f = (v_s_2 * (normE/E_c_2) / sqrt(1.0 + (normE/E_c_2) * (normE/E_c_2))) /
	  (v_s_1 * (normE/E_c_1) / sqrt(1.0 + (normE/E_c_1) * (normE/E_c_1)));
    //

	if (normE > ADL_G_SmallField)
		{
		for (i=0;i<4;i++)
			{
			//calculate Efield in mass transformed frame: Gamma_E[i] = Gamma[i] x Efield:
			for (j=0;j<3;j++) for (k=0;k<3;k++) Gamma_E[i][j] += ADL_G_Gamma[i][j][k]*Efield[k+1];
			E_Gamma_E = scalar(Efield[1],Efield[2],Efield[3],Gamma_E[i][0],Gamma_E[i][1],Gamma_E[i][2]);
			eps[i] = pow(E_Gamma_E,0.5);
			//calculate drift velocity in mass transformed frame:
			g[i] =  Ve100(eps[i]/E100GammaE100)/(eps[i]*E100GammaE100);
			//calculate relative population of i-th valley:
			Ni[i] = 1.0/nu(eps[i]);
			Sum_Ni += Ni[i];
			}

	//Normalize population Ni_N in valley i = 0...3:
		for (i=0;i<4;i++) Ni[i] = Ni[i]/Sum_Ni;

		for (i=0;i<3;i++) //mobility becomes sum of contributions of 4 valeys:
			{
			Result[i+1] = 0.0;
			for (j=0;j<4;j++) Result[i+1] += -Ni[j]*g[j]*Gamma_E[j][i]*f; //Correct for temp. (Y. Kermaidic 2017)
			}

		}

	else  for (i=1;i<4;i++) Result[i] = 0.0;
    }

//**************************************************************************

//****************************************
//***  CalculateHoleVelocity Routines  ***
//****************************************

void ADL_HoleVelocityInLatticeSystem(double Efield[3], double Result[3])
	{
	int i;
	double normE, Amp, k0 = 0.0;
	double Vr,Vteta,Vphi,phi,teta,sin2teta,sin2phi2;									

	normE = norm(Efield[0],Efield[1],Efield[2]);

	// Tentative to incorporate temperature dependance (herited from SigGen 2017: D. Radford)
	double T0 = 77;  //K
	double T1 = 200; //K
	double exp_h = -2.398;
	double B_h = 1.2e7; // cm/s
	double mu_h = 1.63e9; //cm2/Vs
        
	double mu_0_1 = mu_h * pow(T0, exp_h);
	double v_s_1 = B_h * sqrt(tanh(0.5 * T1 / T0));
	double E_c_1 = v_s_1 / mu_0_1;
	double mu_0_2 = mu_h * pow(ADL_G_Temp, exp_h);
	double v_s_2 = B_h * sqrt(tanh(0.5 * T1 / ADL_G_Temp));
	double E_c_2 = v_s_2 / mu_0_2;
	
	double f = (v_s_2 * (normE/E_c_2) / sqrt(1.0 + (normE/E_c_2) * (normE/E_c_2))) /
	  (v_s_1 * (normE/E_c_1) / sqrt(1.0 + (normE/E_c_1) * (normE/E_c_1)));
    //
    
	Amp = Vh100(normE)*f; // Correct for temp. (Y. Kermaidic 2017)
	if (Amp > 0) k0 = KaNul(Vh111(normE)/Amp);

	if (normE > ADL_G_SmallField)
		{
		//Calculating the angles:
		teta = acos(Efield[2]/normE); 
		if (fabs(Efield[0]) > ADL_G_SmallField)  phi = atan(Efield[1]/Efield[0]);
		else phi = 0.0;
		if (Efield[0] <= 0.0) phi += PI;   //adjustment in II and III quadrant of atan function

		sin2teta = sin(teta)*sin(teta);
		sin2phi2 = sin(2*phi)*sin(2*phi);
		//in spherical coordinates:
		Vr		= Amp*(1-alpha(k0)*(sin2teta*sin2teta*sin2phi2+sin(2*teta)*sin(2*teta)));
		Vphi	= Amp*beta(k0)*(sin2teta*sin(teta)*sin(4*phi));
		Vteta	= Amp*beta(k0)*(2*(sin2teta*sin(teta)*cos(teta)*sin2phi2)+sin(4*teta));

		//transform to cartesian coordinates:
		Result[0]= Vr*cos(phi)*sin(teta) + Vteta*cos(phi)*cos(teta) - Vphi*sin(phi);
		Result[1]= Vr*sin(phi)*sin(teta) + Vteta*sin(phi)*cos(teta) + Vphi*cos(phi);
		Result[2]= Vr*cos(teta) - Vteta*sin(teta);
		}

	else  for (i=0;i<3;i++) Result[i] = 0.0;
	}


void ADL_CalculateHoleVelocity_GE(double *Efield, double *Result)
	{
	int i,j; 
	double Vlattice[3],Elattice[3] = {0,0,0};

	//Transform Efield to lattice system:
	//is a matrix multiplication with LatticeRotation:	
	for (i=0;i<3;i++) for (j=0;j<3;j++) Elattice[i] += ADL_G_LatticeRotation[i][j]*Efield[j+1];

	//Get the velocity in the lattice system:
	ADL_HoleVelocityInLatticeSystem(Elattice,Vlattice);

	//Transform Vlattice back to lab frame:
	//is a matrix multiplication with transposed of LatticeRotation:
	for (i=1;i<4;i++) Result[i] = 0.0;
	for (i=0;i<3;i++) for (j=0;j<3;j++) Result[i+1] += ADL_G_LatticeRotation[j][i]*Vlattice[j];
	}

