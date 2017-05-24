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

#include "ADL_Trapping_FIELD.h"
//******************************************************
//THIS IS THE ONE TO BE USED WITH NEUTRON DAMAGE      **
//******************************************************
//These trapping rates assuming constant mean free paths.
//Under such assumption, the electron trapping rate becomes depending on the drift velocity.

#ifndef ADL_NORM_
#define ADL_NORM_

//extra definitions used here
#define scalar(a1,a2,a3,b1,b2,b3)	(a1*b1+a2*b2+a3*b3)
#define norm(a1,a2,a3)				(sqrt(scalar(a1,a2,a3,a1,a2,a3)))

#endif // ADL_NORM_

double ADL_ElectronTrappingRate_FIELD(double P[])
{
	double *Ve;
	if (! IsInDetector(P)) return 0.0;
    double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
							//Calculate Field strength (needed for drift velocity)
	GetElectronVelocity(E,Ve);		//dPdt[1..3] = Ve, electron drift velocity in [cm/us] !!!
	E[0] = norm(E[1],E[2],E[3])/1000.0;			//scale to something of the order of 1 
	Ve[0] = norm(Ve[1],Ve[2],Ve[3]);
	return Ve[0]/(0.001+E[0]);					//something small added to E to prevent deviding by 0
}

double ADL_HoleTrappingRate_FIELD(double P[])
{
	double *Vh;
	if (! IsInDetector(P)) return 0.0;
    double E[4] = {GetEField(P)[0],GetEField(P)[1],GetEField(P)[2],GetEField(P)[3]};
							//Calculate Field strength (needed for drift velocity)
	GetHoleVelocity(E,Vh);			//dPdt[1..3] = Vh, hole drift velocity in [cm/us] !!!
	E[0] = norm(E[1],E[2],E[3])/1000.0;			//scale to something of the order of 1 
	Vh[0] = norm(Vh[1],Vh[2],Vh[3]);
	return Vh[0]/(0.001+E[0]);					//something small added to E to prevent deviding by 0
}
