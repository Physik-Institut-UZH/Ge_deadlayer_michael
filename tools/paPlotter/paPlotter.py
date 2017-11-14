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
fsize=20

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
parser.add_option("-d", "--dimension", action="store_true", dest="dim",
                  help="Consider 2D or 3D plots", default=False)
parser.add_option("-a", "--axis", dest="ax",
                  help="Choose the crystal axis", metavar="NUMBER", default="100")
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
print("Dimension: ", options.dim)
print("Axis: ", options.ax)

angle = 0

#change crystal axis to angle
if options.ax == '100':
	angle = 0
elif options.ax == '110':
	angle = 45

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

spcha_flag = 0
wpot_flag = 0
noimp_flag = 0
stru_flag = 0

if len(sys.argv) > 3:
        #	if len(sys.argv[i].split(".")) == 2:
        #	spcha_flag = 1
        #	pa_NoImpPot = PA.PA(file = sys.argv[3])
        #	break
	if sys.argv[2].split("_")[-1] == "NoImp.pa":
		noimp_flag = 1
	elif sys.argv[2].split("_")[-1] == "Wpot.pa":
		wpot_flag = 1
		print("Wpot considered",wpot_flag)
	elif sys.argv[2].split("_")[-1] == "Stru.pa":
		stru_flag = 1
	else:
		for i in range(0,len(sys.argv[2].split("_"))):
			if sys.argv[2].split("_")[i] == "Wpot":
				wpot_flag = 1
				print("Wpot considered",wpot_flag)
				break
			elif sys.argv[2].split("_")[i] == "Stru":
				stru_flag = 1
				print("Stru considered",stru_flag)
				break

print("spcha_flag : ",spcha_flag)
print("wpot_flag : ",wpot_flag)
print("noimp_flag : ",noimp_flag)
print("stru_flag : ",stru_flag)
print("angle : ",angle)

#set .pa dimensions
nz=pa_pot.nz()
nx=pa_pot.nx()
ny=pa_pot.ny()

if spcha_flag == 1:
	if nx>pa_Wpot.nx(): nx=pa_NoImpPot.nx()
	if ny>pa_Wpot.ny(): ny=pa_NoImpPot.ny()
	if nz>pa_Wpot.nz(): nz=pa_NoImpPot.nz()

print("Pot size : ", nx, ny, nz)

normax=0;
potmax=0;

#print i,j, 4000*pa_Wpot.potential(j, i, int(nz*0.5)), pa_pot.potential(j, i, int(nz*0.5)),4000*pa_Wpot.potential(j, i, int(nz*0.5)) + pa_pot.potential(j, i, int(nz*0.5))


#create an empty array to save the .pa data into
#extract the information from the .pa
if options.hor:
	pot = np.zeros((ny,nx))
	nor = np.zeros((ny,nx))
	i = 0
	while i < ny:
		j = 0
		while j < nx:
			if wpot_flag == 1:
				pot[i,j] = 4000*pa_Wpot.potential(j, i, int(nz*0.5)) + pa_pot.potential(j, i, int(nz*0.5))
			else:
				pot[i,j] = pa_pot.potential(j, i, int(nz*0.5))
			if j>1 and i>1:
				nor[i-1,j-1] = math.sqrt(math.pow(pot[i-1,j-1]-pot[i,j-1],2)+math.pow(pot[i-1,j]-pot[i-1,j-1],2))
				if nor[i-1,j-1]>normax: normax=nor[i-1,j-1]
			j = j + 1
		i = i + 1
else:
	pot = np.zeros((nz,nx))
	nor = np.zeros((nz,nx))

	a = math.tan(math.radians(angle))
	b = (ny-a*nx)/2.
    
	i = 0
	while i < nz:
		j = 0
		while j < nx:
			if spcha_flag == 1:
				pot[i,j] = pa_pot.potential(j, int(ny*0.5), i) - pa_NoImpPot.potential(j, int(ny*0.5), i)
            #pot[i,j] = pa_pot.potential(int(nx*0.5),j, i) - pa_NoImpPot.potential(int(nx*0.5),j, i)
			else:
				pot[i,j] = pa_pot.potential(j, int(a*j+b), i)
            #pot[i,j] = pa_pot.potential(int(nx*0.5),j, i)
            #pot[i,nx-1-j] = pa_pot.potential(j, int(ny*0.5), i)
			if pot[i,j]>potmax:	potmax=pot[i,j]
			if j>1 and i>1:
				nor[i-1,j-1] = math.sqrt(math.pow(pot[i-1,j-1]-pot[i,j-1],2)+math.pow(pot[i-1,j]-pot[i-1,j-1],2))*math.cos(math.radians(angle))/float(options.gridsize)/1e3
				if nor[i-1,j-1]>normax:	normax=nor[i-1,j-1]
			j = j + 1
		i = i + 1



normax=10000

toto = 0

#for i in range(0,len(pot)):
#	print(i*float(options.gridsize), pot[i,0])

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

if noimp_flag == 0 and spcha_flag == 0 and wpot_flag == 0:
    custom_map2 = LinearSegmentedColormap('custom_map2', cdictGrad)

potmax=potmax/10000;

#plot the .pa
fig = plt.figure(1, figsize=(10,10))
#plt.title(('Potential'),fontsize=fsize)
plt.xlabel('length (mm)',fontsize=fsize,weight='bold')
plt.ylabel('height (mm)',fontsize=fsize,weight='bold')
#plt.plot([87,91],[6,6],'k', linewidth=30, color='w')
if spcha_flag == 1:
	plt.imshow(pot, vmin=-2000, vmax=0, cmap=custom_map1, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(pot, vmin=1e-3, vmax=1, cmap=custom_map1, origin='lower', interpolation="Nearest", norm=LogNorm())
elif stru_flag == 1:
	plt.imshow(pot, cmap='jet', origin='lower', interpolation="Nearest")
else:
	plt.imshow(pot, vmin=0, vmax=4000, cmap=custom_map1, origin='lower', interpolation="Nearest")
cbar = plt.colorbar(pad=.01)
cbar.set_label('Weighting potential', rotation=270, fontsize=25, labelpad=20)
cbar.ax.tick_params(labelsize=20)
#plt.xticks(plt.xticks()[0], [str(-int(gridsize*(nx-t))) for t in plt.xticks()[0]],weight='bold',fontsize=fsize)
#plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]],weight='bold',fontsize=fsize)
plt.xticks(plt.xticks()[0], [str(gridsize*t) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([0,pot[1,:].size, 0, pot[:,1].size])
plt.savefig(ofile + '.pdf', bbox_inches='tight', transparent=True)

normax=normax/10000;

plt.figure(2, figsize=(10,10))
plt.title(('Norm of field'),fontsize=fsize)
plt.xlabel('length [mm]',fontsize=fsize)
plt.ylabel('height [mm]',fontsize=fsize)
#plt.plot([87,91],[6,6],'k', linewidth=30, color='w')
#plt.imshow(nor, cmap='jet', origin='lower', interpolation="Nearest", norm=LogNorm(vmin=normax))
if spcha_flag == 1:
	plt.imshow(nor, cmap=custom_map2, vmax=0.3e4, origin='lower', interpolation="Nearest")
elif wpot_flag == 1:
	plt.imshow(nor, cmap=custom_map2, vmax=3, origin='lower', interpolation="Nearest")
elif stru_flag == 1:
	plt.imshow(nor, cmap='jet', origin='lower', interpolation="Nearest")
elif noimp_flag == 0:
	plt.imshow(nor, cmap=custom_map2, vmax=1.0e1, origin='lower', interpolation="Nearest")
else:
	plt.imshow(nor, cmap=custom_map2, vmax=1.2e4, origin='lower', interpolation="Nearest")

cbar = plt.colorbar(pad=.01)
cbar.set_label('Electric field (kV/cm)', rotation=270, fontsize=25, labelpad=20)
cbar.ax.tick_params(labelsize=20)

n_ext = nx/2.*(1./math.cos(math.radians(angle))-1)*math.cos(math.radians(angle))

plt.xticks(plt.xticks()[0], [str(gridsize*int((t-n_ext)/math.cos(math.radians(angle)))) for t in plt.xticks()[0]])
plt.yticks(plt.yticks()[0], [str(gridsize*t) for t in plt.yticks()[0]])
plt.axis([n_ext,nor[1,:].size-n_ext, 0, nor[:,1].size])


#save the plot
plt.savefig(ofile + '_grad.pdf', bbox_inches='tight', transparent=True)


#show the plot on the screen
if options.con:
	plt.show()
