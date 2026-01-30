#ifndef DETRES_STRUCT_H
#define DETRES_STRUCT_H

#include <qvariant.h>

typedef struct
{
	int rank; 			// Rank of this docking result according to the F2Dock scoring function
	double score; 			// overall F2Dock score, the higher the better
	double shape; 			// shape complementarity score
	double ssr; 			// skin-skin overlap score
	double ccr;			// core-core overlap score
	double scr;			// skin-core overlap score
	double elec;
	double hbond;
	double vdw;
	int clashes;
	double mat[12];
	int conf;
	double rmsd;
} DetailResults;

#endif
