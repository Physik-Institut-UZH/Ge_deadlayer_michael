#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
from optparse import OptionParser
import sys
import os

#read in command line parameters
parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
				help="The file that is analysed (.pa)", metavar="FILE")
parser.add_option("-t", "--timestep", dest="timestep",
				help="The timestep of the pulse (in ns, default 1)", metavar="NUMBER", default="1")
parser.add_option("-s", "--saveas", dest="ofile",
				help="A filename to save the pdf to", metavar="FILE")
parser.add_option("-c", "--control", action="store_true", dest="con",
				help="Show plots", default=False)
(options, args) = parser.parse_args()

fsize=14

#print parsed parameters
print "Filename:  ", options.filename
print "Output:    ", options.ofile
print "Timestep:  ", options.timestep
print "Control:   ", options.con

#check if filename is given
if not options.filename:   # if filename is not given
	parser.error("No file defined to analyse: Please use at least option -f.")
	sys.exit()

#extract the output name
if not options.ofile:
	pathname, filename = os.path.split(options.filename)
	filename = filename.split(".")
	filename = filename[0]
	ofile=filename
else:
	ofile=options.ofile

#try opening the file
try:
	pulse  = np.genfromtxt(options.filename, skip_header=1, autostrip=True)
except IOError as e:
	print "Couldn't find the file", options.filename
	sys.exit()

timestep=float(options.timestep)

pulse=pulse[:,6]
time=np.arange(0, len(pulse)*timestep, timestep)

plt.figure(1)
plt.plot(time, pulse, label="pulse")
plt.xlabel("time [ns]", fontsize=fsize)
plt.ylabel("charge [au]", fontsize=fsize)
plt.grid()
plt.legend(loc=2, prop={'size':fsize})
plt.savefig(ofile + '_cha.pdf', bbox_inches='tight', transparent=True)

pulse_dif=-pulse[0:-2:1]+pulse[1:-1:1]
time=time[0:-2:1]

plt.figure(2)
plt.plot(time, pulse_dif, label="pulse")
plt.xlabel("time [ns]", fontsize=fsize)
plt.ylabel("current [au]", fontsize=fsize)
plt.grid()
plt.legend(loc=2, prop={'size':fsize})
plt.savefig(ofile + '_cur.pdf', bbox_inches='tight', transparent=True)

if options.con:
	plt.show()
