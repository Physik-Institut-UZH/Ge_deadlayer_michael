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

density = 5.54e-3
save    = 1
fsize   = 18

def GetMass(params):
  
  cylVol  =  math.pi * pow(float(params[0]),2)*float(params[1])
  wellVol =  math.pi * pow(float(params[6]),2)*float(params[7])
  grVol   =  math.pi * (pow(float(params[2])+float(params[3]),2)+pow(float(params[2]),2))*float(params[3])

  vol     = cylVol-wellVol-grVol
  mass    = vol*density
  
  return mass

def GetMin(E,r):
  min = 1e6
  for i in range(0,len(E)):
    if((r[i] < 0.4 or (r[i] > 0.7 and r[i] < 2.5)) and E[i]<min): min = E[i]
  return min

depVolfile="DepletionVoltageRecord.txt"

try:
  fo = open(depVolfile, "r")
  configs = fo.readlines()
  fo.close()
#  configs = np.genfromtxt(depVolfile, autostrip=True)
except IOError as e:
  print("Couldn't find the file", depVolfile)
  sys.exit()

print("%s detector configuration found:\r" % len(configs))
for config in configs:
    if(len(config.split()) != 2): continue
    print(("  - %s -> DV = %s V \r") % (config.split()[0],config.split()[1]))
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

    print("  %i input files found \r" % len(SSlist))
    for iter,(label,file) in enumerate(zip(Labels,SSlist)):
        print("    - ",label,file)

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
    props = dict(boxstyle='round', facecolor='wheat', alpha=1)
    config_items = ['Radius','Height','Groove radius','Groove depth','Groove width','Well radius','Well top radius','Well depth','p+ radius','Mass']
    config_values = config.split()[0].split('_')

    config_values.append(GetMass(config_values))
    config_text = 'Detector geometry: \n'

    if(len(config_items) != len(config_values)):
        print(("ERROR - Number of configuration items (%s) do not match values (%s)"),(len(config_items),len(config_values)))
        sys.exit()


#    print("Set plot legend")
    plt.xlim([0, 4.0])
    plt.ylim([0, 1000])
    ax.legend(fontsize=12,loc="upper right")

    for iter, (config_item,config_value) in enumerate(zip(config_items,config_values)):
      if(config_item == 'Mass'): config_text += '\n - ' + config_item + ': ' + '%4.0f' % config_value + ' g'
      else: config_text += ' - ' + config_item + ': ' + str(config_value) + ' mm \n'
    plt.text(0.1,440,config_text, bbox=props)

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
