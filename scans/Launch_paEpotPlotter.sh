#!/bin/bash

list=(ICOAX_Epot*.pa)

#rm *_Emin.txt

for file in ${list[@]}
do
	python ../tools/paEpotPlotter/paEpotPlotter.py -f ${file} -g $1
done
