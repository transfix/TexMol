#ifndef SUMRES_STRUCT_H
#define SUMRES_STRUCT_H

#include <qvariant.h>

typedef struct
{
	int hitsInRange[6];
	int numGoodPeaks;
	int minRankedPeak;
	double minRankedPeakRMSD;
	double minRankedPeakScore;
	double minRMSD;
	int indexMinRMSD;
	double scoreMinRMSD;
	double time;
	int numSol;
} SummaryResults;

#endif
