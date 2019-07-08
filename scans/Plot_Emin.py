#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import math as math
import matplotlib.colors as colors
from matplotlib.colors import LogNorm
import scipy.interpolate
import glob

density = 5.55e-3
#density = 5.323e-3
save    = 1
fsize   = 18

def GetMass(params):
  
  cylVol  =  math.pi * pow(float(params[0]),2)*float(params[1])
  wellVol =  math.pi * pow(float(params[5]),2)*float(params[7])
  wellVol =  math.pi * (pow(float(params[6]),2)-pow(float(params[5]),2))*float(params[7])/2. + wellVol
  grVol   =  math.pi * (pow(float(params[2])+float(params[3]),2)-pow(float(params[2]),2))*float(params[3])
  tpVol   =  math.pi * (pow(float(params[0]),2)-pow(float(params[0])-float(params[12]),2))*float(params[13])/2.

  cylDlVol  = math.pi * pow(float(params[0])-float(params[11]),2)*(float(params[1])-2*float(params[11]))
  wellDlVol = math.pi * pow(float(params[5])+float(params[11]),2)*(float(params[7])+float(params[11]))
  botDlVol  = math.pi * pow(float(params[2])+float(params[3]),2)*float(params[11])

  vol     = cylVol-wellVol-grVol-tpVol
  mass    = vol*density

  actvol  = vol - (cylVol-cylDlVol+botDlVol) - (wellDlVol-wellVol)
  actmass = actvol*density

  print(" ")
  print("  Volumes and masses information:")
  print("    - Cylinder vol. %6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (cylVol,cylVol*density,100*cylVol/cylVol))
  print("    - Well vol.     %6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (wellVol,wellVol*density,100*wellVol/cylVol))
  print("    - Groove vol.   0%6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (grVol,grVol*density,100*grVol/cylVol))
  print("    - Taper vol.    %6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (tpVol,tpVol*density,100*tpVol/cylVol))
  print("    - Total vol.    %6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (vol,mass,100*vol/cylVol))
  print(" ")
  print(" Active part:")
  print("    - Vol.          %6.2f cm3 \t - M = %4.0f g \t - frac. %3.2f " % (actvol,actmass,100*actvol/vol))
  print(" ")
  
  return mass

def GetMin(E,r):
  min = 1e6
  for i in range(0,len(E)):
    if((r[i] < 0.4 or (r[i] > 0.7 and r[i] < 2.5)) and E[i]<min): min = E[i]
  return min


print(" ")
print("Start summarizing minimal E-field scan")
print(" ")

# Retrieve the scale used to store dimensions in integer format in the file name
if(len(sys.argv)>1):
	scale=float(sys.argv[1])
else:
	scale=10

depVolfile="DepletionVoltageRecord.txt"
print(" - Scaling: %f" % scale)
print(" - Input  : %s" % depVolfile)

try:
  fo = open(depVolfile, "r")
  configs = fo.readlines()
  fo.close()
#  configs = np.genfromtxt(depVolfile, autostrip=True)
except IOError as e:
  print("Couldn't find the file: %s" % depVolfile)
  sys.exit()

print("%s detector configuration found:\r" % len(configs))
for config in configs:
    if(len(config.split()) != 2): continue
    print("  - %s -> DV = %s V \r" % (config.split()[0],config.split()[1]))
    DV = float(config.split()[1])
    
    Labels = []
    Labels.append(config.split()[1] + " + 500 V")
    Labels.append(config.split()[1] + " + 1000 V")
    Labels.append(config.split()[1] + " + 1500 V")
    
    SSlist = [f for f in glob.glob("./*"+ config.split()[0] + "*.txt")]
    if(len(SSlist) == 0): sys.exit()
    else: SSlist.sort()
    
    ## PLOT EMIN DISTRIBUTION
    plt.figure(1)
    ax = plt.subplot(111)

    print(" ")
    print("  %i input files found \r" % len(SSlist))
    for iter,(label,file) in enumerate(zip(Labels,SSlist)):
        print("    - %s --> %s " %(label,file))

        R = []
        Z = []
        Emin = []
        emin = []

        filename = file
        #try opening the file
        try:
          Data = np.genfromtxt(filename, skip_header=1, autostrip=True)
        except IOError as e:
          print("Couldn't find the file", filename)
          sys.exit()

        r = []
        z = []
        e = []

        r = np.array(Data[0:len(Data)-2,0]).astype(np.float)
        z = np.array(Data[0:len(Data)-2,1]).astype(np.float)
        e = np.array(Data[0:len(Data)-2,2]).astype(np.float)

        plt.plot(r,e,'-',linewidth=2, label=label + ' - Emin: ' + '%3.0f' % GetMin(e,r) + ' V/cm')

          #   print("Set det. config.")
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
    config_items = ['Radius','Height','Groove radius','Groove depth','Groove width','Well radius','Well top radius','Well depth','p+ radius','Imp. bot','Imp. top','DL','Taper radius','Taper height','Mass']
    config_values = config.split()[0].split('_')
    for entry in range(0,len(config_values)):
	config_values[entry] = float(config_values[entry]) / scale

    config_values.append(GetMass(config_values))
    config_text = 'Detector geometry: \n'

    if(len(config_items) != len(config_values)):
        print(("ERROR - Number of configuration items (%s) do not match values (%s)"),(len(config_items),len(config_values)))
        sys.exit()


#    print("Set plot legend")
    plt.xlim([0, 4.0])
    plt.ylim([0, 1000])
    ax.legend(fontsize=12,loc="upper right")

    taper_set = 0

    for iter, (config_item,config_value) in enumerate(zip(config_items,config_values)):
      if(config_item == 'Mass'): config_text += '\n - ' + config_item + ': ' + '%4.0f' % config_value + ' g'
      elif(config_item == 'Imp. bot' or config_item == 'Imp. top'): 
	config_value = float(config_value)
	config_text += ' - ' + config_item + ': -' + str(config_value) + ' e9 /cc \n'
      elif(config_item == 'DL'): 
	config_value = float(config_value)
	config_text += ' - ' + config_item + ': ' + str(config_value) + ' mm \n'
      elif((config_item == 'Taper radius' or config_item == 'Taper height') and (float(config_value) == 0 and taper_set==0)):
	config_text += 'Outer taper: None\n'
	taper_set=1
      else: config_text += ' - ' + config_item + ': ' + str(config_value) + ' mm \n'
    plt.text(0.15,230,config_text, bbox=props)

#print("Set plot axis")
    plt.xticks(fontsize=15)
    plt.yticks(fontsize=15)
    plt.xlabel("R [cm]", fontsize=18)
    plt.ylabel("Minimal E-field [V/cm]", fontsize=18)
#    print("Set plot grid")
    plt.grid()

    plotname = 'ICOAX_' + config.split()[0] + '_R_Emin.pdf'
    if(save == 1): plt.savefig(plotname, bbox_inches='tight', transparent=True)
#print("Save plot")

    plt.show()
#print("Plot shown")
