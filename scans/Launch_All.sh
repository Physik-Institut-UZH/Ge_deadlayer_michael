#!/bin/bash

if [ $1 == 1 ]
then
	rm ./DepletionVoltageRecord.txt
	rm ./*.pa
	rm ./*.pdf
	rm ./*_Emin.txt
fi

# Create Stru.pa file
sh Launch_Scans.sh 0

# Create DepletionVoltageRecord.txt file to check for detector depletion
sh Launch_Scans.sh DV

# Create Stru.pa / Wpot.pa / Epot.pa files at D.V. + 500V and +1000V
sh Launch_Scans.sh 1
sh Launch_Scans.sh 2
sh Launch_Scans.sh 3

# Create _Pot.pdf / _E.pdf / _Ex.pdf / _Ez.pdf
sh Launch_paEpotPlotter.sh

# Plot the minimal E-field distribution for the various applied voltages
python Plot_Emin.py

# Move all the created files to more appropriate repository
#sh Launch_CleanUp.sh
