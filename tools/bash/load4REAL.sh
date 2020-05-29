#! /bin/bash
## 1) Script to load all necessary path variables for adl4.
## 2) The adl4 installation where the script is executed is used
##    if the variable ADL4 wasn't previously set
## 3) Defines also the alias for paPlotter und pulsePlotter
## 4) If sourced it defines the variables in the current shell
##    if executed it opens a new shell with all variables defined
##    that can be ended by typing "exit" (prefered version)

#if test -z $ADL4 ;
#then 
#  export ADL4="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && cd ../../ && pwd )";
#fi

export ADL4="/home/atp/milorad/ADL4"
echo "ADL4 variables set. Version is: $ADL-4.2"

#export HOST=$(hostname)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ADL4/lib
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$ADL4/include
export CPLUS_INCLUDE_PATH=CPLUS_INCLUDE_PATH:$ADL4/include
export LIBRARY_PATH=$LIBRARY_PATH:$ADL4/lib

#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/Applications/adl-4.2/lib
#export C_INCLUDE_PATH=$C_INCLUDE_PATH:/Applications/adl-4.2/include
#export CPLUS_INCLUDE_PATH=CPLUS_INCLUDE_PATH:/Applications/adl-4.2/include
#export LIBRARY_PATH=$LIBRARY_PATH:/Applications/adl-4.2/lib

function paPlotter() { python $ADL4/tools/paPlotter/paPlotter.py "$@"; }
function paPlotterTrace() { python $ADL4/tools/paPlotterTrace/paPlotterTrace.py "$@"; }
function pulsePlotter() { python $ADL4/tools/pulsePlotter/pulsePlotter.py "$@"; }

export -f paPlotter
export -f paPlotterTrace
export -f pulsePlotter

#declare -x PS1="\`echo -ne >/dev/tty \"\\033]0;\$USER@\$HOST:\$PWD\\007\"\`$(dirname $ADL4)>> "

exec bash
