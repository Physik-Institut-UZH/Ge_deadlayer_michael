CC     =gcc
CXX    =c++
LIBS   =-I../include/ -L../lib/ -lADL-4-2
PROG   =CreatePA SimulatePulse

all: $(PROG)

%: %.c ; $(CXX) $< -o $@ $(LIBS)

clean: ; rm -f $(PROG) *.pdf *.txt
