/*
 * AGATA Detector simulation Library (ADL)
 *
 * Bart Bruyneel - bartb@ikp.uni-koeln.de
 * Benedikt Birkenbach - bene@ikp.uni-koeln.de
 *
 * IKP Cologne
 * http://www.ikp.uni-koeln.de/agata
 *
 */

#include "ADL_ReadWrite_TXT.h"

/* ROUTINE TO READ THE HEADER AND TO COMPARE THE DEFINES */
int ADL_ReadFileHeader_TXT(FILE * datafile) {
	/* THE MAXIMUM OF INTERACTIONS, THE NUMBER OF SEGMENTS, \
     THE TRACE LENGTH AND THE SAMPLING INTERVAL ARE \
     DEFINED IN Simulation_Main.c */

	int RE_nInt = 0;
	int RE_nSeg = 0;
	int RE_dimT = 0;
	double RE_DeltaT = 0.0;
	double tmp = 0.0;
	char main_header[100];

	/* CHECK IF THE FILE POINTER EXISTS */
	if (datafile == NULL) {
		if (GetADLDebug()) printf("\nERROR: File pointer for reading the header is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		return (-1);
	}

	/* THE MAIN HEADER CONTAINING A SHORT TEXT AND GLOBAL DEFINES AS nInt, nSeg, dimT, DeltaT */

	/* THE FIRST LINE CONTAINS ONLY TEXT */
	fgets(main_header, 100, datafile);

	/* THEN COMES THE LINE DESCRIBING THE NEXT VALUES */
	fgets(main_header, 100, datafile);

	/* THE SECOND ONE THE DEFINES */
	fscanf(datafile, "%d %d %d %lf\n", &RE_nInt, &RE_nSeg, &RE_dimT, &RE_DeltaT);

	/* IF THE DEFINITONS IN THE FILE AND THE PROGRAMM DIFFER, WE HAVE A PROBLEM */
	tmp = GetDeltaT() / RE_DeltaT; /* COMPARING FLOATS IS A MESS */
	if ((RE_nInt != GetNINT()) || (RE_nSeg != GetNSEG()) || (RE_dimT != GetDIMT()) || (tmp < 0.99999) || (tmp > 1.00001)) {
		if (GetADLDebug()) printf("\nERROR: The definitions in the input file and the preprocessor defines differ.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		if (GetADLDebug()) printf("ERROR: nInt: %d\tnSeg: %d\tdimT: %d\tDeltaT: %lf\n",RE_nInt, RE_nSeg, RE_dimT, RE_DeltaT);
		return (-1);
	}
	return (0);
}

/* ROUTINE TO READ THE NEXT INPUT */
int ADL_ReadEventHP_TXT(FILE * datafile, struct ADL_HITS * event) {

	int i;
	char input_header[10];
	char description[100];

	/* CHECK IF THE FILE POINTER EXISTS */
	if (datafile == NULL) {
		if (GetADLDebug()) printf("ERROR: File pointer is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		return (-1);
	}
	/* GO THROUGH THE FILE UNTIL THE INPUT-HEADER IS FOUND */
	while (fgets(input_header, 10, datafile) != NULL) {
		if (strcmp(input_header, "# HITS\n") == 0) {

			/* READ THE T0 TIME */
			fgets(description, 100, datafile);
			fscanf(datafile, "%lf\n", &event->T0);

			/* READ THE ENERGY DEPOSITON OF nInt INTERACTIONS */
			fgets(description, 100, datafile);
			for (i = 0; i < GetNINT(); i++)
				fscanf(datafile, "%lf\n", &event->Eint[i]);

			/* READ ALL THE INTERACTION POSITIONS */
			fgets(description, 100, datafile);
			for (i = 0; i < GetNINT(); i++)
				fscanf(datafile, "%lf %lf %lf\n",&event->Pos[i][0],&event->Pos[i][1],&event->Pos[i][2]);

			/* LEAVE THE ROUTINE */
			return (0);
		}
	}

	/* IF NO INPUT, THEN RETURN A MINUS 1 */
	if (GetADLDebug()) printf("\nERROR: Cannot read HITS from file.\n");
	if (GetADLDebug()) printf("ERROR: Message from ReadWriteHP.\n");
	return (-1);
}

/* READ THE TRACE DATA */
int ADL_ReadEventTD_TXT(FILE * datafile, struct ADL_TRACES * event) {

	int i, j;
	char output_header[10];
	char description[100];
	/* CHECK IF THE FILE POINTER EXISTS */
	if (datafile == NULL) {
		if (GetADLDebug()) printf("\nERROR: File pointer is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWriteTD.\n");
		return (-1);
	}

	/* GO THROUGH THE FILE UNTIL THE OUTPUT-HEADER IS FOUND */
	while (fgets(output_header, 10, datafile) != NULL) {
		if (strcmp(output_header, "# TRACES\n") == 0) {

			/* READ THE TRIGGER TIME */
			fgets(description, 100, datafile);
			fscanf(datafile, "%d ", &event->Trig);

			/* READ THE ENERGIE OF ALL SEGMENTS */
			fgets(description, 100, datafile);
			for (i = 0; i < GetNSEG(); i++)
				fscanf(datafile, "%lf ", &event->Eseg[i]);

			/* READ THE TRACES */
			fgets(description, 100, datafile);
			for (j = 0; j < GetDIMT() + 1; j++) {
				for (i = 0; i < GetNSEG(); i++)
					fscanf(datafile, "%lf ", &event->Tr[i][j]);
			}
			return (0);
		}
	}

	/* IF NO OUTPUT IN FILE, THEN RETURN A MINUS ONE */
	if (GetADLDebug()) printf("\nERROR: Cannot read TRACES from file.\n");
	if (GetADLDebug()) printf("ERROR: Message from ReadWriteTD.\n");
	return (-1);

}

/* READ ONE EVENT INCLUDING COMPARISSON OF THE HEADER */
int ADL_ReadEvent_TXT(FILE * datafile, struct ADL_EVENT * event) {

	/* READ THE HITPATTERN */
	if (ADL_ReadEventHP_TXT(datafile, &(event->HP)) == -1)
		return (-1);
	/* READ THE TRACE DATA */
	if (ADL_ReadEventTD_TXT(datafile, &(event->TD)) == -1)
		return (-1);
	return (0);
}

/* WRITE THE HEADER INCLUDING THE DEFINES */
int ADL_WriteFileHeader_TXT(FILE * output) {
	int WR_nInt = GetNINT();
	int WR_nSeg = GetNSEG();
	int WR_dimT = GetDIMT();
	double WR_DeltaT = GetDeltaT();

	/* CHECK IF THE FILE POINTER EXISTS */
	if (output == NULL) {
		if (GetADLDebug()) printf("\nERROR: File pointer for writing is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		return (-1);
	}

	/* PRINT THE HEADER TEXT AND THE DEFINES */
	fprintf(output, "# AGATA DATASET\n");
	fprintf(output, "# nInt nSeg dimT DeltaT\n");
	fprintf(output, "%d %d %d %lf\n", WR_nInt, WR_nSeg, WR_dimT, WR_DeltaT);
	return (0);
}

/* WRITE THE INPUT */
int ADL_WriteEventHP_TXT(FILE * output, struct ADL_HITS * event) {

	int i;

	/* CHECK IF THE FILE POINTER EXISTS */
	if (output == NULL) {
		if (GetADLDebug()) printf("\nERROR: File pointer for writing is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		return (-1);
	}

	/* PRINT THE INPUT HEADER */
	fprintf(output, "\n# HITS\n");

	/* PRINT THE TRIGGER TIME */
	fprintf(output, "# TIME T0 [us]\n");
	fprintf(output, "%lf\n", event->T0);

	/* PRINT THE ENERGY DEPOSITION OF nInT INTERACTIONS */
	fprintf(output, "# ENERGIES OF THE nInt INTERACTIONS\n");
	for (i = 0; i < GetNINT(); i++)
		fprintf(output, "%lf\n", event->Eint[i]);

	/* PRINT THE POSITION OF nInT INTERACTIONS */
	fprintf(output, "# POSITIONS OF THE nInt INTERACTIONS\n");
	for (i = 0; i < GetNINT(); i++)
		fprintf(output, "%lf %lf %lf\n", event->Pos[i][0], event->Pos[i][1],
				event->Pos[i][2]);
	return (0);
}

/* WRITE THE OUTPUT */
int ADL_WriteEventTD_TXT(FILE * output, struct ADL_TRACES * event) {
	int i, j;
	/* CHECK IF THE FILE POINTER EXISTS */
	if (output == NULL) {
		if (GetADLDebug()) printf("\nERROR: File pointer for writing is wrong.\n");
		if (GetADLDebug()) printf("ERROR: Message from ReadWrite.\n");
		return (-1);
	}

	/* PRINT THE OUTPUT HEADER */
	fprintf(output, "\n# TRACES\n");

	/* PRINT THE TRIGGER TIME */
	fprintf(output, "# TRIGGER TIME\n");
	fprintf(output, "%d ", event->Trig);
	fprintf(output, "\n");

	/* PRINT THE ENERGIE OF ALL SEGMENTS */
	fprintf(output, "# ENERGY OF THE nSeg SEGMENTS\n");
	for (i = 0; i < GetNSEG(); i++) fprintf(output, "%lf ", event->Eseg[i]);
	fprintf(output, "\n");

	/* PRINT ALL TRACES */
	fprintf(output, "# TRACES OF THE nSeg SEGMENTS\n");
	for (j = 0; j < GetDIMT() + 1; j++) {
		for (i = 0; i < GetNSEG(); i++) fprintf(output, "%lf ", event->Tr[i][j]);
		fprintf(output, "\n");
	}
	return (0);
}

/* WRITE ONE EVENT */
int ADL_WriteEvent_TXT(FILE * output, struct ADL_EVENT * event) {

	/* WRITE THE HITPATTERN */
	if (ADL_WriteEventHP_TXT(output, &(event->HP)) == -1)
		return (-1);
	/* WRITE THE TRACE DATA */
	if (ADL_WriteEventTD_TXT(output, &(event->TD)) == -1)
		return (-1);

	return (0);
}

