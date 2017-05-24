#!/usr/bin/env python

# Python script modified by Yoann Kermaidic in 2017 (yoann.kermaidic@mpi-hd.mpg.de)

import numpy as np
import matplotlib.pyplot as plt
import PA
from optparse import OptionParser
import sys
import os
import math as math
from matplotlib.colors import LogNorm
from matplotlib.colors import LinearSegmentedColormap

# the fonsize of the plot
fsize=14

#read in command line parameters
parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
				help="The file that is analysed (.pa)", metavar="FILE")
parser.add_option("-g", "--gridsize", dest="gridsize",
				help="The gridsize of the pa (in cm, default 0.01)", metavar="NUMBER", default="0.01")
parser.add_option("-o", "--horizontal", action="store_true", dest="hor",
				help="The cut is in horizontal direction (default vertical)", default=False)
parser.add_option("-s", "--saveas", dest="ofile",
				help="A filename to save the pdf to", metavar="FILE")
parser.add_option("-c", "--control", action="store_true", dest="con",
				help="Plot some 2d control plots", default=False)
(options, args) = parser.parse_args()

#check if filename is given
if not options.filename:   # if filename is not given
	parser.error("No file defined to analyse: Please use at least option -f.")
	sys.exit()

#print parsed parameters
print("Filename:  ", options.filename)
print("Gridsize:  ", options.gridsize)
print("Horizontal:", options.hor)
print("Control: ", options.con)


#change gridsize to mm
gridsize=float(options.gridsize)*10.0

#try opening the file
try:
	pa_pot = PA.PA(file = (options.filename))
except IOError as e:
	print("Couldn't find the file", options.filename)
	sys.exit()

#extract the output name
if not options.ofile:
	pathname, filename = os.path.split(options.filename)
	filename = filename.split(".")
	filename = filename[0]
	ofile=filename
else:
	ofile=options.ofile

wpot_flag = 0
stru_flag = 0

if len(sys.argv) >= 3:
	if sys.argv[2].split("_")[1] == "Wpot" or sys.argv[2].split("_")[1] == "Wpot.pa":
		wpot_flag = 1
	elif sys.argv[2].split("_")[1] == "Stru" or sys.argv[2].split("_")[1] == "Stru.pa":
		stru_flag = 1

print("wpot_flag : ",wpot_flag)
print("stru_flag : ",stru_flag)

#set .pa dimensions
nz=pa_pot.nz()
nx=pa_pot.nx()
ny=pa_pot.ny()

print("Pot size : ", nx, ny, nz)

normax=0;
potmax=0;

#create an empty array to save the .pa data into
#extract the information from the .pa
if options.hor:
	pot = np.zeros((ny,nx))
	nor = np.zeros((ny,nx))
	i = 0
	while i < ny:
		j = 0
		while j < nx:
			pot[i,j] = pa_pot.potential(j, i, int(nz*0.5))
			if j>1 and i>1:
				nor[i-1,j-1] = math.sqrt(math.pow(pot[i-1,j-1]-pot[i,j-1],2)+math.pow(pot[i-1,j]-pot[i-1,j-1],2))
				if nor[i-1,j-1]>normax: normax=nor[i-1,j-1]
			j = j + 1
		i = i + 1
else:
	pot = np.zeros((nz,nx))
	nor = np.zeros((nz,nx))
	i = 0
	while i < nz:
		j = 0
		while j < nx:
			pot[i,j] = pa_pot.potential(j, int(ny*0.5), i)
			if pot[i,j]>potmax:	potmax=pot[i,j]
			if j>1 and i>1:
				nor[i-1,j-1] = math.sqrt(math.pow(pot[i-1,j-1]-pot[i,j-1],2)+math.pow(pot[i-1,j]-pot[i-1,j-1],2))/float(options.gridsize)
				if nor[i-1,j-1]>normax:	normax=nor[i-1,j-1]
			j = j + 1
		i = i + 1

normax=10000

toto = 0

if options.con and toto == 1:
	plt.figure(10)
	plt.plot(pot[:,0])
	plt.title(('Vertical cut through the center of the potential'),fontsize=fsize)
	plt.xlabel('Height [mm]',fontsize=fsize)
	plt.ylabel('Potential [V]',fontsize=fsize)
	plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
	#save the plot
	plt.savefig(ofile + '_Ver_cut.pdf', bbox_inches='tight', transparent=True)
	plt.figure(11)
	plt.plot(pot[40,:])
	plt.title(('Horizontal cut at quarter the height of the potential'),fontsize=fsize)
	plt.xlabel('Length [mm]',fontsize=fsize)
	plt.ylabel('Potential [V]',fontsize=fsize)
	plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
	#save the plot
	plt.savefig(ofile + '_Hor_cut.pdf', bbox_inches='tight', transparent=True)

cdictInv = {'blue':   ((0.0, 0.0, 0.0),
                    (0.1, 0.8, 0.8),
                    (0.2, 0.0, 0.0),
                    (0.3, 1.0, 1.0),
                    (1.0, 1.0, 1.0)),
    
        'green':  ((0.0, 0.0, 0.0),
               (0.1, 0.8, 0.8),
               (0.2, 1.0, 1.0),
               (0.3, 1.0, 1.0),
               (1.0, 0.0, 0.0)),
        
        'red':  ((0.0, 1.0, 1.0),
                  (0.1, 0.8, 0.8),
                  (0.2, 0.0, 0.0),
                  (1.0, 0.0, 0.0))
        }

cdictW = {'red':   ((0.0, 0.0, 0.0),
                    (0.1, 0.8, 0.8),
                    (0.2, 0.0, 0.0),
                    (0.3, 1.0, 1.0),
                    (0.875, 1.0, 1.0),
                    (1.0, 0.5, 0.5)),
    
          'green':  ((0.0, 0.0, 0.0),
                     (0.1, 0.8, 0.8),
                     (0.2, 1.0, 1.0),
                     (0.3, 1.0, 1.0),
                     (1.0, 0.0, 0.0)),
              
           'blue':  ((0.0, 1.0, 1.0),
                     (0.1, 0.8, 0.8),
                     (0.2, 0.0, 0.0),
                    (1.0, 0.0, 0.0))
          }

cdictE = {'red':   ((0.0, 0.0, 0.0),
                    (0.2, 0.8, 0.8),
                    (0.33, 0.0, 0.0),
                    (0.5, 1.0, 1.0),
                    (0.875, 1.0, 1.0),
                    (1.0, 0.5, 0.5)),
    
          'green':  ((0.0, 0.0, 0.0),
                     (0.2, 0.8, 0.8),
                     (0.33, 1.0, 1.0),
                     (0.5, 1.0, 1.0),
                     (1.0, 0.0, 0.0)),
        
          'blue':  ((0.0, 1.0, 1.0),
                    (0.2, 0.8, 0.8),
                    (0.33, 0.0, 0.0),
                    (1.0, 0.0, 0.0))
        }
cdictGrad = {'red':   ((0.0, 0.0, 0.0),
                       (0.05, 0.8, 0.8),
                       (0.1, 0.0, 0.0),
                       (0.125, 1.0, 1.0),
                       (0.875, 1.0, 1.0),
                       (1.0, 0.5, 0.5)),
    
            'green':  ((0.0, 0.0, 0.0),
                       (0.05, 0.8, 0.8),
                       (0.1, 1.0, 1.0),
                       (0.125, 1.0, 1.0),
                       (0.25, 0.25, 0.25),
                       (1.0, 0.0, 0.0)),
        
            'blue':  ((0.0, 1.0, 1.0),
                      (0.05, 0.8, 0.8),
                      (0.1, 0.0, 0.0),
                      (0.25, 0.0, 0.0),
                      (1.0, 0.0, 0.0))
    }

custom_map1 = LinearSegmentedColormap('custom_map1', cdictE)
custom_map2 = LinearSegmentedColormap('custom_map2', cdictW)

if wpot_flag == 1:
	custom_map1 = LinearSegmentedColormap('custom_map1', cdictW)
elif wpot_flag == 0:
	custom_map2 = LinearSegmentedColormap('custom_map2', cdictGrad)

potmax=potmax/10000;

#plot the .pa
fig = plt.figure(1, figsize=(10,10))
plt.title(('Potential'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if wpot_flag == 1:
	plt.imshow(pot, vmin=0, vmax=1, cmap=custom_map1, origin='lower', interpolation="Nearest")
elif stru_flag == 1:
	plt.imshow(pot, cmap='jet', origin='lower', interpolation="Nearest")
else:
	plt.imshow(pot, vmin=0, vmax=4000, cmap=custom_map1, origin='lower', interpolation="Nearest")
plt.colorbar()

plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,pot[1,:].size, 0, pot[:,1].size])
plt.savefig(ofile + '.pdf', bbox_inches='tight', transparent=True)

normax=normax/10000;

plt.figure(2, figsize=(10,10))
plt.title(('Norm of field'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if wpot_flag == 1:
	plt.imshow(nor, cmap=custom_map2, vmax=3, origin='lower', interpolation="Nearest")
elif stru_flag == 1:
	plt.imshow(nor, cmap='jet', origin='lower', interpolation="Nearest")
else:
	plt.imshow(nor, cmap=custom_map2, vmax=1.2e4, origin='lower', interpolation="Nearest")

plt.colorbar()
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,nor[1,:].size, 0, nor[:,1].size])

#save the plot
plt.savefig(ofile + '_grad.pdf', bbox_inches='tight', transparent=True)


#show the plot on the screen
if options.con:
	plt.show()
