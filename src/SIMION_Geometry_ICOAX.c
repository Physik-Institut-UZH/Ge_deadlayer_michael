#include "SIMION_Geometry_ICOAX.h"


double GetICoaxGridSize(){return SIMION_GridSize_ICoax;}
double GetICoaxHeight(){return SIMION_GridSize_ICoax*double(ICOAX_G_Height);}
double GetICoaxDimension(){return SIMION_Dimension_ICoax;}
double GetICoaxVoltage(){return SIMION_Voltage_ICoax;}
double GetICoaxEpsScale(){return SIMION_EpsScale_ICoax;}
double GetICoaxEpsExtScale(){return SIMION_EpsExtScale_ICoax;}
char* GetICoaxDescription(){return SIMION_Description_ICoax;}

void InitializeICOAX(){
  
  //setup geometry
  
  ICOAX_G_GrooveDepth         = 30;
  ICOAX_G_GrooveWidth         = 30;
  ICOAX_G_PointContactRadius  = 70;  // NEW
  ICOAX_G_PointContactDepth   = 1;  // NEW
  ICOAX_G_PasLayThickness     = 10;
  ICOAX_G_Radius              = 350;
  ICOAX_G_Height              = 800;
  ICOAX_G_SurfaceContactDepth = 10;  // NEW
  ICOAX_G_GrooveInnerRadius   = 70;
  ICOAX_G_EdgeRadius          = 0;  // Allow outer tapering
  ICOAX_G_EdgeHeight          = 1;  // Allow outer tapering
  ICOAX_G_Spacing             = 0;
  ICOAX_G_ExtGroundWidth      = 0;
  
  ICOAX_G_WellRadius          = 16;  // NEW
  ICOAX_G_WellRadiusTop       = ICOAX_G_WellRadius;  // Allow well tapering
  ICOAX_G_WellDepth           = 40;  // NEW
  ICOAX_G_WellRounding        = 0;  // NEW
  
  ICOAX_G_ImpTop               = -1.0;
  ICOAX_G_ImpBot               = -1.0;
  int k; for(k=0;k<10;k++){ICOAX_G_Imp[0][k]=0.; ICOAX_G_Imp[1][k]=0.;}
  ICOAX_G_ImpIn                = 0.0;
  ICOAX_G_ImpOut               = 0.0;
  ICOAX_G_Center               = 0.0;
}

int SIMION_Setup_GEOMETRY_ICOAX(char *filename_setupfile) {
  
  InitializeICOAX();
  InitializeArray(SIMION_GridSize_ICoax,SIMION_Dimension_ICoax,SIMION_Voltage_ICoax,SIMION_EpsScale_ICoax,SIMION_EpsExtScale_ICoax,SIMION_Description_ICoax);
  int	i,len, err=1,k;
  double temp=0;
  struct	ADL_KEYWORD **Kwords;
  
  //get list of keywords from file:
  if (strlen(filename_setupfile)>1) {// if a filename is supplied, parse it
    Kwords = ADL_parse_file (filename_setupfile);
    
    //overwrite keywords with parsed values:
    //first scan for gridsize!
    len = Kwords[1]->linenumber;
    err = Kwords[len+2]->linenumber;
    for (i=0;i<len;i++) {
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_GridSize")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_GridSize_ICoax);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Dimension")==0)
        sscanf(Kwords[2+i]->svalue,"%i",&SIMION_Dimension_ICoax);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Voltage")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_Voltage_ICoax);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsScale")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsScale_ICoax);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_EpsExtScale")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&SIMION_EpsExtScale_ICoax);
      if (strcmp(Kwords[2+i]->keyword,"SIMION_G_Description")==0)
        sscanf(Kwords[2+i]->svalue,"%s",SIMION_Description_ICoax);
    }
    for (i=0;i<len;i++) {
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_Radius")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_Radius=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_Height")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_Height=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_PointContactRadius")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_PointContactRadius=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_PointContactDepth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_PointContactDepth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_SurfaceContactDepth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_SurfaceContactDepth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_WellDepth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_WellDepth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_WellRadius")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_WellRadius=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_WellRadiusTop")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_WellRadiusTop=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_WellRounding")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_WellRounding=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_GrooveInnerRadius")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_GrooveInnerRadius=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_GrooveDepth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_GrooveDepth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_GrooveWidth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_GrooveWidth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_PasLayThickness")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_PasLayThickness=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_EdgeRadius")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_EdgeRadius=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_EdgeHeight")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_EdgeHeight=SIMION_grid(temp,SIMION_GridSize_ICoax);
        if (ICOAX_G_EdgeHeight<1) ICOAX_G_EdgeHeight=1;
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_ExtGroundWidth")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_ExtGroundWidth=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_Spacing")==0){
        sscanf(Kwords[2+i]->svalue,"%lf" ,&temp);
        ICOAX_G_Spacing=SIMION_grid(temp,SIMION_GridSize_ICoax);
      }
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_ImpTop")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&ICOAX_G_ImpTop);
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_ImpBot")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&ICOAX_G_ImpBot);
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_ImpIn")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&ICOAX_G_ImpIn);
      if (strcmp(Kwords[2+i]->keyword,"ICOAX_G_ImpOut")==0)
        sscanf(Kwords[2+i]->svalue,"%lf",&ICOAX_G_ImpOut);

      for(k=0;k<10;k++){
        char key[50];
        sprintf(key, "ICOAX_G_Imp%d", k);
        if (strcmp(Kwords[2+i]->keyword,key)==0){
          sscanf(Kwords[2+i]->svalue,"%lf",&ICOAX_G_Imp[0][k]);
          sscanf(Kwords[2+i]->svalue2,"%lf",&ICOAX_G_Imp[1][k]);
          //printf("%s                = %lf - %lf\n",key,ICOAX_G_Imp[0][k],ICOAX_G_Imp[1][k]);
        }
      }
    }
  }
  
  if(SIMION_Dimension_ICoax==2) ICOAX_G_Center=0;
  else ICOAX_G_Center = ICOAX_G_Radius + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth-1.0;
  return err;
}

int SIMION_Status_GEOMETRY_ICOAX(void){
  //print final values that are setup:
  printf("\nStatus SIMION_Geometry_ICOAX:\n");
  printf("---------------------\n");
  printf("SIMION_G_GridSize          = %lf\n",SIMION_GridSize_ICoax);
  printf("SIMION_G_EpsScale          = %lf\n",SIMION_EpsScale_ICoax);
  printf("SIMION_G_EpsExtScale       = %lf\n",SIMION_EpsExtScale_ICoax);
  printf("SIMION_G_Description       = %s \n",SIMION_Description_ICoax);
  printf("SIMION_G_Dimension         = %i \n",SIMION_Dimension_ICoax);
  printf("SIMION_G_Voltage           = %lf\n",SIMION_Voltage_ICoax);
  printf("---------------------\n");
  printf("ICOAX_G_Radius              = %i\n",ICOAX_G_Radius);
  printf("ICOAX_G_Height              = %i\n",ICOAX_G_Height);
  printf("ICOAX_G_WellDepth           = %i\n",ICOAX_G_WellDepth);
  printf("ICOAX_G_WellRadius          = %i\n",ICOAX_G_WellRadius);
  printf("ICOAX_G_WellRadiusTop       = %i\n",ICOAX_G_WellRadiusTop);
  printf("ICOAX_G_WellRounding        = %i\n",ICOAX_G_WellRounding);
  printf("ICOAX_G_PointContactRadius  = %i\n",ICOAX_G_PointContactRadius);
  printf("ICOAX_G_PointContactDepth   = %i\n",ICOAX_G_PointContactDepth);
  printf("ICOAX_G_GrooveInnerRadius   = %i\n",ICOAX_G_GrooveInnerRadius);
  printf("ICOAX_G_GrooveDepth         = %i\n",ICOAX_G_GrooveDepth);
  printf("ICOAX_G_GrooveWidth         = %i\n",ICOAX_G_GrooveWidth);
  printf("ICOAX_G_PasLayThickness     = %i\n",ICOAX_G_PasLayThickness);
  printf("ICOAX_G_EdgeRadius          = %i\n",ICOAX_G_EdgeRadius);
  printf("ICOAX_G_EdgeHeight          = %i\n",ICOAX_G_EdgeHeight);
  printf("ICOAX_G_Spacing             = %i\n",ICOAX_G_Spacing);
  printf("ICOAX_G_ExtGroundWidth      = %i\n",ICOAX_G_ExtGroundWidth);
  printf("ICOAX_G_ImpTop              = %lf\n",ICOAX_G_ImpTop);
  printf("ICOAX_G_ImpBot              = %lf\n",ICOAX_G_ImpBot);
  printf("ICOAX_G_Imp0                = %lf - %lf\n",ICOAX_G_Imp[0][0],ICOAX_G_Imp[1][0]);
  printf("ICOAX_G_Imp1                = %lf - %lf\n",ICOAX_G_Imp[0][1],ICOAX_G_Imp[1][1]);
  printf("ICOAX_G_Imp2                = %lf - %lf\n",ICOAX_G_Imp[0][2],ICOAX_G_Imp[1][2]);
  printf("ICOAX_G_Imp3                = %lf - %lf\n",ICOAX_G_Imp[0][3],ICOAX_G_Imp[1][3]);
  printf("ICOAX_G_Imp4                = %lf - %lf\n",ICOAX_G_Imp[0][4],ICOAX_G_Imp[1][4]);
  printf("ICOAX_G_Imp5                = %lf - %lf\n",ICOAX_G_Imp[0][5],ICOAX_G_Imp[1][5]);
  printf("ICOAX_G_Imp6                = %lf - %lf\n",ICOAX_G_Imp[0][6],ICOAX_G_Imp[1][6]);
  printf("ICOAX_G_Imp7                = %lf - %lf\n",ICOAX_G_Imp[0][7],ICOAX_G_Imp[1][7]);
  printf("ICOAX_G_Imp8                = %lf - %lf\n",ICOAX_G_Imp[0][8],ICOAX_G_Imp[1][8]);
  printf("ICOAX_G_Imp9                = %lf - %lf\n",ICOAX_G_Imp[0][9],ICOAX_G_Imp[1][9]);
  printf("ICOAX_G_ImpIn               = %lf\n",ICOAX_G_ImpIn);
  printf("ICOAX_G_ImpOut              = %lf\n",ICOAX_G_ImpOut);
  return 0;
}

struct SIMION_PA *SIMION_newPA_ICOAX(void){
  //adding 0.5 for correct rounding
  
  int npot=3;
  
  int NX=1;
  int NY=1;
  int NZ=(ICOAX_G_Height+2.0*ICOAX_G_Spacing+2.0*ICOAX_G_ExtGroundWidth);
  if (SIMION_Dimension_ICoax==2){
    NX=(ICOAX_G_Radius+ICOAX_G_Spacing+ICOAX_G_ExtGroundWidth);
    NY=1;
  }
  else{
    NX=2.0*(ICOAX_G_Radius+ICOAX_G_Spacing+ICOAX_G_ExtGroundWidth)-1.0;
    NY=2.0*(ICOAX_G_Radius+ICOAX_G_Spacing+ICOAX_G_ExtGroundWidth)-1.0;
  }
  
  struct SIMION_PA * pa = new_SIMION_PA(SIMION_Description_ICoax, npot, SIMION_GridSize_ICoax, NX, NY, NZ);
  
  return pa;
}

int SIMION_CalcPoint_ICOAX(int nx, int ny, int nz, int i){ // get the radii
  
  if (i!=1 && i!=0){
    printf("\nWARNING in SIMION_CalcPoint_ICOAX: Wrong contact number %d!\n",i);
  }
  
  if(SIMION_Dimension_ICoax==2) ICOAX_G_Center=0;
  else ICOAX_G_Center = ICOAX_G_Radius + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth-1.0;
  
  double radii = sqrt((double)((nx - ICOAX_G_Center)*(nx - ICOAX_G_Center) + (ny - ICOAX_G_Center)*(ny - ICOAX_G_Center)));
  double rEdge=(double)ICOAX_G_EdgeRadius/(double)ICOAX_G_EdgeHeight*(double)(ICOAX_G_Height-nz) + (double)(ICOAX_G_Radius-ICOAX_G_EdgeRadius);
  
  if (ICOAX_G_EdgeHeight==0) printf ("\nERROR in SIMION_CalcPoint_ICOAX: ICOAX_G_EdgeHeight cannot be equals to zero!\n");
  
  double dlEdge = (double)ICOAX_G_EdgeRadius/(double)ICOAX_G_EdgeHeight*(double)(ICOAX_G_Height-nz) + (double)(ICOAX_G_Radius-ICOAX_G_EdgeRadius) - (double)ICOAX_G_SurfaceContactDepth/(double)ICOAX_G_EdgeHeight*sqrt((double)(ICOAX_G_EdgeHeight*ICOAX_G_EdgeHeight+ICOAX_G_EdgeRadius*ICOAX_G_EdgeRadius));
  
  double wellEdge;
  if(ICOAX_G_WellRounding == 0) wellEdge = (double)(ICOAX_G_WellRadiusTop - ICOAX_G_WellRadius)/(double)ICOAX_G_WellDepth*(double)(ICOAX_G_WellDepth - ICOAX_G_Height + nz);
  else wellEdge = (double)(ICOAX_G_WellRadiusTop - ICOAX_G_WellRadius)/(double)(ICOAX_G_WellDepth - ICOAX_G_WellRadius)*(double)(ICOAX_G_WellDepth - ICOAX_G_WellRadius - ICOAX_G_Height + nz);
  
  //point is inside detector volume
  if(radii < rEdge && radii < ICOAX_G_Radius && nz < ICOAX_G_Height + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth && nz >= ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth){
    
    //point is in the groove (or passivation layer)
    if(radii <= ICOAX_G_GrooveInnerRadius + ICOAX_G_GrooveWidth && radii >= ICOAX_G_GrooveInnerRadius && nz <= ICOAX_G_GrooveDepth+ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth){
      
      //point is in the groove
      if (radii < ICOAX_G_GrooveInnerRadius + ICOAX_G_GrooveWidth - ICOAX_G_PasLayThickness && radii > ICOAX_G_GrooveInnerRadius + ICOAX_G_PasLayThickness && nz < ICOAX_G_GrooveDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth - ICOAX_G_PasLayThickness) return GROOVE;
      //point is in the passivation layer
      else return P_LAY;
    }
    // passivation surface between the groove and the p-contact
    else if(radii < ICOAX_G_GrooveInnerRadius && radii >= ICOAX_G_PointContactRadius && nz <= ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth + ICOAX_G_PasLayThickness) return P_LAY;
    //inner contact (point contact)
    else if(radii <= ICOAX_G_PointContactRadius && nz <= ICOAX_G_PointContactDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth){
      if(ICOAX_G_PointContactRadius == ICOAX_G_GrooveInnerRadius){
            if (i==0) return V_CONT;
            else return Z_CONT;
      }
      // dimple-contact rounding
      else if(radii <= ICOAX_G_PointContactRadius - ICOAX_G_PointContactDepth + sqrt(pow(ICOAX_G_PointContactDepth,2)-pow(nz,2))){
        if (i==0) return V_CONT;
        else return Z_CONT;
      }
      else return BULK;
    }
    //Point is in the well
    else if(radii <= ICOAX_G_WellRadius + wellEdge + ICOAX_G_SurfaceContactDepth && nz > ICOAX_G_Height - ICOAX_G_WellDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth - ICOAX_G_SurfaceContactDepth){
      if(ICOAX_G_WellRounding == 0){
        //Point is in the well
        if(radii < ICOAX_G_WellRadius + wellEdge                            && nz > ICOAX_G_Height - ICOAX_G_WellDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth) return OUTSIDE;
        //Point is in the well outer contact
        else {
          if (i==1) return V_CONT;
          else return Z_CONT;
        }
      }
      else{
        //Point is in the well
        if(radii < ICOAX_G_WellRadius + wellEdge                                    && nz > ICOAX_G_Height - ICOAX_G_WellDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth + ICOAX_G_WellRadius) return OUTSIDE;
        else if(radii <= ICOAX_G_WellRadius + wellEdge + ICOAX_G_SurfaceContactDepth && nz > ICOAX_G_Height - ICOAX_G_WellDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth + ICOAX_G_WellRadius){
          if (i==1) return V_CONT;
          else return Z_CONT;
        }
        else if(radii < ICOAX_G_WellRadius+ wellEdge  + ICOAX_G_SurfaceContactDepth && sqrt((double)((nx - ICOAX_G_Center)*(nx - ICOAX_G_Center) + (ny - ICOAX_G_Center)*(ny - ICOAX_G_Center) +
                                                                                                     (nz - (ICOAX_G_Height-ICOAX_G_WellDepth+ ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth +ICOAX_G_WellRadius))*(nz - (ICOAX_G_Height-ICOAX_G_WellDepth+ ICOAX_G_Spacing + 	ICOAX_G_ExtGroundWidth +ICOAX_G_WellRadius)))) < ICOAX_G_WellRadius) return OUTSIDE;
        else if(radii < ICOAX_G_WellRadius + wellEdge + ICOAX_G_SurfaceContactDepth && sqrt((double)((nx - ICOAX_G_Center)*(nx - ICOAX_G_Center) + (ny - ICOAX_G_Center)*(ny - ICOAX_G_Center) +
                                                                                                     (nz - (ICOAX_G_Height-ICOAX_G_WellDepth+ ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth +ICOAX_G_WellRadius))*(nz - (ICOAX_G_Height-ICOAX_G_WellDepth+ ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth +ICOAX_G_WellRadius)))) < ICOAX_G_WellRadius + ICOAX_G_SurfaceContactDepth){
          //Point is in the well outer contact
          if (i==1) return V_CONT;
          else return Z_CONT;
        }
        else return BULK;
      }
    }
    //       }
    //outer contact (surface contact)
    else if(nz >= ICOAX_G_Height - ICOAX_G_SurfaceContactDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth){
      if (i==1) return V_CONT;
      else return Z_CONT;
    }
    else if (radii >= dlEdge || radii >= ICOAX_G_Radius - ICOAX_G_SurfaceContactDepth){
      if (i==1) return V_CONT;
      else return Z_CONT;
    }
    else if(radii >= ICOAX_G_GrooveInnerRadius + ICOAX_G_GrooveWidth && nz < ICOAX_G_SurfaceContactDepth + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth){
      if (i==1) return V_CONT;
      else return Z_CONT;
    }
    else return BULK;
  }
  else if(radii >= ICOAX_G_Radius + ICOAX_G_Spacing || nz < ICOAX_G_ExtGroundWidth || nz >= 2*ICOAX_G_Spacing + ICOAX_G_Height + ICOAX_G_ExtGroundWidth) return EXT_GROUND;
  else return OUTSIDE;
}


double SIMION_CalcCharge_ICOAX(int nx, int ny, int nz, int i){
  
  if(SIMION_Dimension_ICoax==2) ICOAX_G_Center=0;
  else ICOAX_G_Center = ICOAX_G_Radius + ICOAX_G_Spacing + ICOAX_G_ExtGroundWidth-1.0;
  
  if (SIMION_CalcPoint_ICOAX(nx, ny, nz, i)==P_LAY) return 100;
  
  if(ICOAX_G_Imp[0][0] < 0.){
    int k=0;
    while(nz > ICOAX_G_Imp[1][k+1]/SIMION_GridSize_ICoax) k++;
    
    return ICOAX_G_Imp[0][k]
    + (ICOAX_G_Imp[0][k+1]-ICOAX_G_Imp[0][k])
    * (nz-ICOAX_G_ExtGroundWidth-ICOAX_G_Spacing-ICOAX_G_Imp[1][k]/SIMION_GridSize_ICoax)
    / ((ICOAX_G_Imp[1][k+1]-ICOAX_G_Imp[1][k])/SIMION_GridSize_ICoax)
    + (ICOAX_G_ImpOut-ICOAX_G_ImpIn)*(nx-double(ICOAX_G_Radius)/2.)/ICOAX_G_Radius;
  }
  
  return ICOAX_G_ImpBot+(ICOAX_G_ImpTop-ICOAX_G_ImpBot)*(nz-ICOAX_G_ExtGroundWidth-ICOAX_G_Spacing)/ICOAX_G_Height + (ICOAX_G_ImpOut-ICOAX_G_ImpIn)*(nx-double(ICOAX_G_Radius)/2.)/ICOAX_G_Radius;
}
