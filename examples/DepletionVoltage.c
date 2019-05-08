#include "ADL.h"
#include <iostream>

double ADL_G_SIMION_GridSize;
double ADL_G_SIMION_Center;
double FindDepletionVoltage();
int TestDepletion(SIMION_HEADER *hh,int *geo,double *wpot,double *spot, double voltage);
char buffer[100];

int main (int argc, char **argv) {
  FILE *output;
  struct ADL_EVENT *evt;
  int j,l;
  double P[4];
  
  //initialize the required functions
  if(argc==1){printf("No path to SETUP file indicated.\n"); return 1;};
  sprintf(buffer, "%s", argv[1]);
  
  ADLSetup(buffer);
  SetupFields(buffer);
//  ADLStatus();  

//  SIMION_Status_GEOMETRY();
//  SIMION_Status_SOLVER();
  
  ADL_G_SIMION_GridSize = GetSimionGridSize();
  ADL_G_SIMION_Center = GetSimionCenter();

  if(argc > 2){
	char key[100];
	char *pars = key;
	int arg;

  	pars += sprintf(pars, "%s", argv[2]);
  	for(arg = 3;arg<argc;arg++) pars += sprintf(pars, "_%s", argv[arg]);

	output = fopen("DepletionVoltageRecord.txt","a");
	fprintf(output, "%s \t %4.0f \n", key, FindDepletionVoltage());
	printf("Depletion voltage: %4.0f V for config: %s \n",FindDepletionVoltage(),key);
	printf("Store depletion voltage into 'DepletionVoltageRecord.txt' file \n");
  }
  else printf("Depletion voltage: %4.0f V \n",FindDepletionVoltage());

  return 0;
}

/*******************************************
 *Function to calculate dep voltage of current geometry (Marco Salathe function)
 *******************************************/
double FindDepletionVoltage()
{
  SetVoltage(0);
  printf("input voltage: %f \n",GetVoltage());

  double voltage, maxvolt=20000;
  struct SIMION_PA *field_t = GetSIMIONnewPA();
  struct SIMION_PA *spcha_t = GetSIMIONnewPA();
  struct SIMION_PA *stru_t = GetSIMIONnewPA();
  struct SIMION_HEADER *hh = &(field_t->h);

  GetSIMIONWfield(field_t, 1);
  GetSIMIONEfield(spcha_t,0);
  GetSIMIONStructure(stru_t,1);
 
  SetADLWpot(field_t);
  SetADLEpot(spcha_t);
  SetADLStru(stru_t);

  SIMION_Solve(field_t);
  SIMION_Solve(spcha_t);
 
  SIMION_adapt_geometry(field_t->Geo, hh);

  for (voltage=0; voltage<maxvolt; voltage++)
    if(0==TestDepletion(hh,field_t->Geo,field_t->Pot[0],spcha_t->Pot[0],voltage)) break;
  return voltage;
}


/*******************************************
 *Function to test the depletion voltage (Marco Salathe function)
 *******************************************/


int TestDepletion(SIMION_HEADER *hh,int *geo,double *wpot,double *spot, double voltage)
{
    double Ex;
    double Ez;
    int i;
/*
    for (i=6;i < hh->nz-2;++i)
    {
      if (geo[SIMION_node(hh,1,0,i+1)]==0) //means:skip boundary points
        {
            //there is undepleted region at the contact
            if (voltage*wpot[SIMION_node(hh,1,0,i)]+spot[SIMION_node(hh,1,0,i)]<0) { return 1; }
            //there is a bubble
            if (voltage*(wpot[SIMION_node(hh,1,0,i+1)]-wpot[SIMION_node(hh,1,0,i-1)]) +
                (spot[SIMION_node(hh,1,0,i+1)]-spot[SIMION_node(hh,1,0,i-1)])<0) { return 2; }
        }
        else break;
    }
*/
    for (i=6;i < hh->nz-50;++i)
      {
       for (int j=1;j < hh->nx-10;++j)
	  {
	    //if (geo[SIMION_node(hh,j,0,i+1)]==0 && ADL_G_Stru->Pot[0][SIMION_node(ADL_G_SIMION_Header,nx,0,nz)] == BULK) //means:skip boundary points
            double P1[4]={0,j*ADL_G_SIMION_GridSize,ADL_G_SIMION_Center,i*ADL_G_SIMION_GridSize};
	    if(IsInDetector(P1))
	      {
//		printf("  %f (%f,%f) ",voltage,j*ADL_G_SIMION_GridSize,i*ADL_G_SIMION_GridSize);
		//there is undepleted region at the contact
            	if (voltage*wpot[SIMION_node(hh,j,0,i)]+spot[SIMION_node(hh,j,0,i)]<0) { return 1; }
            	//there is a bubble
                Ez = (voltage*(wpot[SIMION_node(hh,j-1,0,i-1)]-wpot[SIMION_node(hh,j-1,0,i)]) +
		      (spot[SIMION_node(hh,j-1,0,i-1)]-spot[SIMION_node(hh,j-1,0,i)]))/ADL_G_SIMION_GridSize;
                Ex = (voltage*(wpot[SIMION_node(hh,j,0,i-1)]-wpot[SIMION_node(hh,j-1,0,i-1)]) +
		      (spot[SIMION_node(hh,j,0,i-1)]-spot[SIMION_node(hh,j-1,0,i-1)]))/ADL_G_SIMION_GridSize;
//		printf("  %f %f %f \n",Ex,Ez,sqrt(Ex*Ex + Ez*Ez));
            	if (sqrt(Ex*Ex + Ez*Ez)<5 && sqrt(Ex*Ex + Ez*Ez)>0.0001) { return 2; }
	      }
	  }
      }
        
    return 0;
}
