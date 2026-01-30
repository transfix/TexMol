#ifndef RDETRES_STRUCT_H
#define RDETRES_STRUCT_H

#include <qvariant.h>

typedef struct
{
	int newRank;		// rank after GBRerank
	double newScore;	// score after GBRerank
	double delGpol;
	double areaProp;
	int rank;		// rank after F2Dock
	double score;		// score after F2Dock
	double elec;
	double hbond;
	double vdw;
	double mat[12];
	int conf;
	double rmsd;
} RerankDetailResults;

#endif
