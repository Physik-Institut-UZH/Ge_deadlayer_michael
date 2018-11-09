#include "ADL.h"
#include <time.h>

int SimulateDetector(char buffer[100],double Px, double Pz, int Niter)
{
    struct ADL_EVENT *evt;

    int j,l;

    const int Ndet = 1;
    int Nint = 1;
    
    SetADLDebug(0);
    SetADLVersion(4.2);
    
    // ADLStatus();
    // StatusFields();
    
    struct SIMION_PA *ADL_Epot[Ndet];
    struct SIMION_PA *ADL_Wpot[Ndet];
    struct SIMION_PA *ADL_Stru[Ndet];
    
    double GridSize[Ndet];
    double Center[Ndet];
    double Height[Ndet];
    
    for(int i = 0;i<Ndet;i++){
        ADLSetup(buffer);
        SetupFields(buffer);

        ADL_Epot[i] = GetADLEpot();
        ADL_Wpot[i] = GetADLWpot();
        ADL_Stru[i] = GetADLStru();
    
        GridSize[i] = GetSimionGridSize();
        Center[i] = GetSimionCenter();
        Height[i] = GetSimionHeight();
        
        printf(" Detector %i size : %lf %lf %lf \n",i,GridSize[i],Center[i],Height[i]);
        if(!Center[i]){ printf("Not a symmetric field, exit\n"); return 1; }
    }
    
    
    for(int i = 0;i<Ndet;i++){
    
        SetADLEpot(ADL_Epot[i]);
        SetADLWpot(ADL_Wpot[i]);
        SetADLStru(ADL_Stru[i]);
        
        SetSimionGridSize(GridSize[i]);
        SetSimionCenter(Center[i]);
        SetSimionHeight(Height[i]);
        
        //lets create a new event structure:
        evt = new_event();
        //Fill in the Hit Pattern (HP):
        for(int j = 0;j<Nint;j++){
            evt->HP.Eint[j]=1.0/double(Nint);             //Energy of interaction 1 (we only simulate a single interaction here)
            evt->HP.T0= 1.000;               //Time the interaction occurs in the trace
            evt->HP.Pos[j][0]=Px-0.005*j;            //Position where this interaction occures
            evt->HP.Pos[j][1]=Center[i];
            evt->HP.Pos[j][2]=Pz-0.005*j;
        }
        //On basis of the HP, here the traces are generated
        //Traces are stored in the Trace Data (TD) part of the event:
    
        //    printf("Status traces \n");
        //StatusTraces(evt);
        //    printf("Calculate traces \n");
        CalculateTraces(evt);
        //    printf("Print traces \n");
    
        if(i == Niter-1){
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
        }
    }
    return 0;
}

int main (int argc, char **argv) {
    
    char setupfile[100];

    double Px, Pz;
    
	//initialize the required functions
	if(argc<2){printf("Not enough path to SETUP file indicated.\n"); return 1;};
    sprintf(setupfile, "%s", argv[1]);

    Px = atof(argv[2]);
    Pz = atof(argv[3]);
    
    int Niter = 1;
    
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    
    if(SimulateDetector(setupfile,Px,Pz,Niter)==1) return 1;
            //printf("Setup file : %s \t %i event \n",setupfile,i);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("\n Simulation ended normally in %lf seconds \n", cpu_time_used);

	return 0;
}
