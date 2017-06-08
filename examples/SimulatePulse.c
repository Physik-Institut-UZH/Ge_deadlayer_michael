#include "ADL.h"
#include <time.h>

int SimulateDetector(char buffer[100],double Px, double Pz, int i)
{
    struct ADL_EVENT *evt;

    int j,l;

    SetADLDebug(0);
    SetADLVersion(4.2);
    
    ADLSetup(buffer);
    SetupFields(buffer);
    //ADLStatus();
    //StatusFields();
    
    //get some variables that define the used arrays (such as height)
    double gridsize = GetSimionGridSize();       //predefined variable for the gridsize used
    double center   = GetSimionCenter();           //predefined variable of the center of a symmetric array (2D and 3D)
    double height   = GetSimionHeight();
    if(!center){ printf("Not a symmetric field, exit\n"); return 1; }
    
    //lets create a new event structure:
    evt = new_event();
    //Fill in the Hit Pattern (HP):
    evt->HP.Eint[0]=1.0;             //Energy of interaction 1 (we only simulate a single interaction here)
    evt->HP.T0= 1.000;               //Time the interaction occurs in the trace
    evt->HP.Pos[0][0]=Px;            //Position where this interaction occures
    evt->HP.Pos[0][1]=center;
    evt->HP.Pos[0][2]=Pz;
    
    //On basis of the HP, here the traces are generated
    //Traces are stored in the Trace Data (TD) part of the event:
    
//    printf("Status traces \n");
    //StatusTraces(evt);
//    printf("Calculate traces \n");
    CalculateTraces(evt);
//    printf("Print traces \n");
    
    //print traces
    
    FILE *output;
    char outfilename[100];
     
    sprintf(outfilename, "%.02f_%.02f_%.02f.txt",evt->HP.Pos[0][0],evt->HP.Pos[0][1],evt->HP.Pos[0][2]);
    output = fopen(outfilename,"w");
    fprintf(output, "# TeX TeY TeZ ThX ThY THZ P1 P2\n");
    for (j = 0; j < GetDIMT() + 1; j++) {
        fprintf(output, "%lf %lf %lf ", GetNUMRES_XYZe()[j][1], GetNUMRES_XYZe()[j][2], GetNUMRES_XYZe()[j][3]);
        fprintf(output, "%lf %lf %lf ", GetNUMRES_XYZh()[j][1], GetNUMRES_XYZh()[j][2], GetNUMRES_XYZh()[j][3]);
        for (l = 0; l < GetNSEG(); l++) fprintf(output, "%lf ", (evt->TD).Tr[l][j]);
        fprintf(output, "\n");
    }
    fclose(output);
    
    return 0;
}

int main (int argc, char **argv) {
    
    char setupfile[100];

    double Px, Pz;
    
	//initialize the required functions
	if(argc<2){printf("Not enough path to SETUP file indicated.\n"); return 1;};
    sprintf(setupfile, "%s", argv[1]);

    Px = 0.5;
    Pz = 6;
    
    int Niter = 1;
    
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    
    for(int i = 0;i<Niter;i++){
        if(SimulateDetector(setupfile,Px,Pz,i)==1) return 1;
            //printf("Setup file : %s \t %i event \n",setupfile,i);
    }
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("\n Simulation ended normally in %lf seconds \n", cpu_time_used);

	return 0;
}
