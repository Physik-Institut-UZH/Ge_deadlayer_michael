#!/usr/bin/env python
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
savePlot=1

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
print("Epot Filename:  ", options.filename)

if options.con:
	savePlot=1

#change gridsize to mm
gridsize=float(options.gridsize)

#try opening the file
try:
	pa_Epot = PA.PA(file = (options.filename))
	print("E_pot filename : ",options.filename)
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

spcha_flag = 0
wpot_flag = 0
noimp_flag = 0

if len(sys.argv) > 3:
	if len(sys.argv[3].split(".")) == 2:
		spcha_flag = 1
		pa_NoImpPot = PA.PA(file = sys.argv[3])
		print("Wpot considered",wpot_flag)
	elif sys.argv[2].split("_")[1] == "Wpot" or sys.argv[2].split("_")[1] == "Wpot.pa":
		wpot_flag = 1

print("spcha_flag : ",spcha_flag)
print("wpot_flag : ",wpot_flag)
print("noimp_flag : ",noimp_flag)

#set .pa dimensions
nz=pa_Epot.nz()
nx=pa_Epot.nx()
ny=pa_Epot.ny()

if spcha_flag == 1:
	if nx>pa_Wpot.nx(): nx=pa_NoImpPot.nx()
	if ny>pa_Wpot.ny(): ny=pa_NoImpPot.ny()
	if nz>pa_Wpot.nz(): nz=pa_NoImpPot.nz()

print("Pot size : ", nx, ny, nz)


normax=0;

#create an empty array to save the .pa data into
#extract the information from the .pa

Epot = np.zeros((nz,nx))
Wpot = np.zeros((nz,nx))
nor = np.zeros((nz,nx))
norx = np.zeros((nz,nx))
norz = np.zeros((nz,nx))
radius = np.zeros((nz,nx))
i = 0
while i < nz:
	j = 0
	while j < nx:
		radius[i][j] = math.sqrt(i*i+j*j)*gridsize
		Epot[i,j] = pa_Epot.potential(j,int(ny*0.5),i)
		if j>1 and i>1:
			egrad = math.pow(Epot[i-1,j-1]-Epot[i,j-1],2)+math.pow(Epot[i-1,j]-Epot[i-1,j-1],2)
			nor[i-1,j-1] = math.sqrt(egrad)/gridsize
			norx[i-1,j-1] = abs(Epot[i-1,j]-Epot[i-1,j-1])/gridsize
			norz[i-1,j-1] = abs(Epot[i-1,j-1]-Epot[i,j-1])/gridsize
			if nor[i-1,j-1]>normax:	normax=nor[i-1,j-1]
		j = j + 1
	i = i + 1

Emin = 1e6
Xmin = -1
Zmin = -1
threshold = 1.5

with open(ofile+"_Emin.txt", "w") as myfile:
	myfile.write("r(cm) \t z(cm) \t E(V/cm) \n")

	for j in range(2,nx-2):
		Emin = 1e6
		for i in range(2,nz-50):
			if(nor[i][j]>1 and Emin>nor[i][j]):
				Emin = nor[i][j]
				Xmin = j*gridsize
				Zmin = i*gridsize
		towrite = '{} \t {} \t {} \n'.format(Xmin,Zmin,Emin)
		myfile.write(towrite)
                #		print >> myfile, '%.3f \t %.3f \t %.2f' % (Xmin,Zmin,Emin)

	myfile.close()

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

if noimp_flag == 0 and spcha_flag == 0 and wpot_flag == 0:
	custom_map2 = LinearSegmentedColormap('custom_map2', cdictGrad)


#plot the .pa
plt.figure(1, figsize=(10,10))
plt.title(('Potential'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if spcha_flag == 1:
	plt.imshow(Epot, vmin=-2000, vmax=0, cmap=custom_map1, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(Epot, vmin=0, vmax=1, cmap=custom_map1, origin='lower', interpolation="Nearest")
else:
	plt.imshow(Epot, vmin=0, vmax=3000, cmap=custom_map1, origin='lower', interpolation="Nearest")
plt.colorbar()
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,Epot[1,:].size, 0, Epot[:,1].size])
if(savePlot): plt.savefig(ofile + '_Pot.pdf', bbox_inches='tight', transparent=True)

normax=normax/10000;

plt.figure(2, figsize=(10,10))
plt.title(('Norm of field'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if spcha_flag == 1:
	plt.imshow(nor, cmap=custom_map2, vmax=0.3e4, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(nor, cmap=custom_map2, vmax=3, origin='lower', interpolation="Nearest")
elif noimp_flag == 0:
	plt.imshow(nor, cmap=custom_map2, vmax=1.0e4, origin='lower', interpolation="Nearest")
else:
	plt.imshow(nor, cmap=custom_map2, vmax=1.2e4, origin='lower', interpolation="Nearest")
plt.colorbar()
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,nor[1,:].size, 0, nor[:,1].size])

#save the plot
if(savePlot): plt.savefig(ofile + '_E.pdf', bbox_inches='tight', transparent=True)

plt.figure(3, figsize=(10,10))
plt.title(('Field x direction'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if spcha_flag == 1:
	plt.imshow(norx, cmap=custom_map2, vmax=0.3e4, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(norx, cmap=custom_map2, vmax=3, origin='lower', interpolation="Nearest")
elif noimp_flag == 0:
	plt.imshow(norx, cmap=custom_map2, vmax=1.0e4, origin='lower', interpolation="Nearest")
else:
	plt.imshow(nor, cmap=custom_map2, vmax=1.2e4, origin='lower', interpolation="Nearest")
plt.colorbar()
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,norx[1,:].size, 0, norx[:,1].size])

#save the plot
if(savePlot): plt.savefig(ofile + '_Ex.pdf', bbox_inches='tight', transparent=True)

plt.figure(4, figsize=(10,10))
plt.title(('Field z direction'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
if spcha_flag == 1:
	plt.imshow(norz, cmap=custom_map2, vmax=0.3e4, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(norz, cmap=custom_map2, vmax=3, origin='lower', interpolation="Nearest")
elif noimp_flag == 0:
	plt.imshow(norz, cmap=custom_map2, vmax=1.0e4, origin='lower', interpolation="Nearest")
else:
	plt.imshow(norz, cmap=custom_map2, vmax=1.2e4, origin='lower', interpolation="Nearest")
plt.colorbar()
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,norz[1,:].size, 0, norz[:,1].size])

#save the plot
if(savePlot): plt.savefig(ofile + '_Ez.pdf', bbox_inches='tight', transparent=True)

#show the plot on the screen
#if options.con:
#	plt.show()
