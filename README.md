*** UPDATES *************************************************************

ADL3 has been updated to ADL4 to fit the requirement of multi-detector
parallel computing for simulating Ge detectors array (as GERDA or LEGEND)
-> All "extern" variables have been converted to "static"

Pre-amplifier electronic response (simple RC circuit) is integrated by
mean of a unique time constant ADL_G_RCtau [mus]

The temperature dependance of e-/holes mobilities is integrated (ADL_G_Temp [K])
See M. Ali Omar and L. Reggiani (Solid-State Electronics Vol. 30, No. 12 (1987) 1351)
-> Code originating from SigGen (David Radford)

Charge cloud diffusion effect integration is under development...

*** INSTALLATION ********************************************************

make

To produce the respective libraries. The only requirement really is
a working C compiler, some standard libraries and python for plotting
the potentials. The variable CC indicates which compiler is used,
standard is c++ now, so that it can be compiled together with ROOT, 
GEANA, GEANT4 without the extern include.

*** EXAMPLE *************************************************************

Here a short example how to compile the software and simulate the
example. In the adl4 directory type:

make
cd examples/
make
./CreatePA ConfigFiles/ICOAX.txt          # Return the weighting and Electric potentials
./DepletionVoltage ConfigFiles/ICOAX.txt  # Return the depletion voltage
./SimulatePulse ConfigFiles/ICOAX.txt r z # Simulate a pulse a (r,z) location

source ../../tools/bash/load.sh

paPlotter -f ICOAX_Epot.pa -g gridsize -c
paPlotter -f ICOAX_Wpot.pa -g gridsize -c
paPlotter -f ICOAX_Stru.pa -g gridsize -c
pulsePlotter -f FILE.txt -c

The same also works for a bege and coaxial detector, by replacing 'ICOAX' with
'COAX' or 'Bege'.

For a more advanced example: https://github.com/Kermaidy/MaGeToADL.git

*** GEOMETRY SCAN *******************************************************

The detector geometry scan script package is used to find out the optimal
detector geometry for given Ge crystal properties (radius - height - impurities)

Usage:

cd scans

#Edit Launch_Scans.sh geometry parameter. Each parameter can be a list
of values that will be looped over by modifying the ConfigFile given
at the top ("file"). The file where detector configuration and depletion
voltage are stored is called "DepletionVoltageRecord.txt" - hard-coded
in the DepletionVoltage.c routine. The output are E-filed maps at various
bias voltage (DV + 500V/1000V/1500V) and the minimal E-field versus the
detector radius for the considered bias voltage + a detector geometry summary

sh Launch_All.sh 1 # The argument allow to remove previous simulation output if '1'
