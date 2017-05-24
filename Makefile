CC=c++ -g

OBJ = src/ADL.o src/ADL_Convl.o src/ADL_Convl_DUMMY.o src/ADL_Drift.o src/ADL_Drift_GE.o src/ADL_Event.o src/ADL_Fields.o src/ADL_Fields_SIMION.o src/ADL_NumRes.o src/ADL_Parser.o src/ADL_ReadWrite.o src/ADL_ReadWrite_TXT.o src/ADL_Time.o src/ADL_Time_FAST.o src/ADL_Traces.o src/ADL_Traces_NUMRES.o src/SIMION_Array.o src/SIMION_ReadWrite.o src/ADL_Trapping.o src/ADL_Trapping_FIELD.o src/SIMION_Geometry.o src/SIMION_Geometry_BEGE.o src/SIMION_Geometry_ICOAX.o src/SIMION_Geometry_PLANAR.o src/SIMION_Geometry_COAX.o src/SIMION_Solver.o src/SIMION_Solver_POISSON.o src/SIMION_Solver_LAPLACE.o src/SIMION_Solver_INHOMOG.o

HDR = $(OBJ:.o=.h)

all: install examples

install: header libADL-4-2.a

objects: $(OBJ)

header:
	mkdir -p include && cp $(HDR) include

libADL-4-2.a: $(OBJ)
	mkdir -p lib && ar r lib/libADL-4-2.a $?

clean:
	rm -rf lib src/*.o include 2> /dev/null

distclean: clean cleanexamples

examples:
	cd examples/&& $(MAKE)

cleanexamples:
	cd examples/&& $(MAKE) clean

.PHONY: examples cleanexamples
