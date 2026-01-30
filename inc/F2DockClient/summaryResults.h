/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of TexMol.

  TexMol is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  TexMol is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef SUMRES_STRUCT_H
#define SUMRES_STRUCT_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

#include <F2DockClient/detailResults.h>
#include <MolEnergyUtil/utils.h>

#ifdef WITH_INTERFACE_STATS
#include <InterfaceStats/ComputeInterfaceStats.h>
#endif

#include <UsefulMath/Matrix.h>

using namespace std;

class SummaryResults
{
	int hitsInRange[6];
	int hitsInRangeAfterRerank[6];
	int numGoodPeaks[2];
	int minRankedPeak[2];
	double minRankedPeakRMSD[2];
	double minRankedPeakScore[2];
	double minRMSD[2];
	int indexMinRMSD[2];
	double scoreMinRMSD[2];
	double time[2];
	int numSol;
	int rerankNumSol;

	string dockingOutputFileName;
	string rerankingOutputFileName;

	bool _isDockingOutput;
	bool _isRerankingOutput;

	bool _dockingOutputProcessed;
	bool _rerankingOutputProcessed;

	bool _rmsdPresent;

	DetailResults dummy;

	vector<DetailResults *> detres;
	vector<DetailResults *> rerankdetres;

#ifdef WITH_INTERFACE_STATS
	ComputeInterfaceStats *compStats;
#endif
	int numStats;

	bool readDockingOutputFromFile();
	bool readRerankingOutputFromFile();
	bool readOutputFromFile();

	bool mergeOutputs();
	bool flipOutputs();	
	bool mineOutputs();

	public:
	enum {HIT_RANGE_1_10, HIT_RANGE_1_100, HIT_RANGE_1_1000, HIT_RANGE_1_10000, HIT_RANGE_1_100000};

	SummaryResults();
	~SummaryResults();

	bool init();
	// set methods
	bool readDockingOutput(string outputFileName, bool rmsd);
	bool readRerankingOutput(string outputFileName, bool rmsd);
	bool readOutput(string outputFileName1, string outputFileName2, bool rmsd);
	
	// get methods
	int getHitInRange(int range) {return hitsInRange[range] ;}
	int* getHitsInRange(void) {return hitsInRange ;}

	int getHitInRangeAfterRerank(int range) {return hitsInRangeAfterRerank[range] ;}
	int* getHitsInRangeAfterRerank(void) {return hitsInRangeAfterRerank ;}

	int getNumGoodPeaks(void) {return numGoodPeaks[0];}
	int getNumGoodPeaksAfterRerank(void) {return numGoodPeaks[1];}

	int getMinRankedPeak(void) {return minRankedPeak[0];}
	int getMinRankedPeakAfterRerank(void) {return minRankedPeak[1];}

	double getMinRankedPeakRMSD(void) {return minRankedPeakRMSD[0];}
	double getMinRankedPeakRMSDAfterRerank(void) {return minRankedPeakRMSD[1];}

	double getMinRankedPeakScore(void) {return minRankedPeakScore[0];}
	double getMinRankedPeakScoreAfterRerank(void) {return minRankedPeakScore[1];}

	double getMinRMSD(void) {return minRMSD[0];}
	double getMinRMSDAfterRerank(void) {return minRMSD[1];}

	int getIndexMinRMSD(void) {return indexMinRMSD[0];}
	int getIndexMinRMSDAfterRerank(void) {return indexMinRMSD[1];}

	double getScoreMinRMSD(void) {return scoreMinRMSD[0];}
	double getScoreMinRMSDAfterRerank(void) {return scoreMinRMSD[1];}

	double getTime(void) {return time[0];}
	double getTimeAfterRerank(void) {return time[1];}

	int getNumSol(void) {return numSol;}
	int getRerankNumSol(void) {return rerankNumSol;}

	bool isDockingOutput(void) {return _isDockingOutput;}
	bool isRerankingOutput(void) {return _isRerankingOutput;}

	vector<DetailResults *>* getDetailResults(void) {return &detres;}
	DetailResults* getDetailResult(int i);
	int getSize(void) {return detres.size();}

	int getRank(int i) {if(i>detres.size()) return -1; return detres[i]->getRank();}
	int getNewRank(int i) {if(i>detres.size()) return -1; return detres[i]->getNewRank();}
	double getScore(int i) {if(i>detres.size()) return 0; return detres[i]->getScore();}
	double getNewScore(int i) {if(i>detres.size()) return 0; return detres[i]->getNewScore();}

	double getShape(int i) {if(i>detres.size()) return 0; return detres[i]->getShape();}
	double getSsr(int i) {if(i>detres.size()) return 0; return detres[i]->getSsr();}
	double getCcr(int i) {if(i>detres.size()) return 0; return detres[i]->getCcr();}
	double getScr(int i) {if(i>detres.size()) return 0; return detres[i]->getScr();}
	double getElec(int i) {if(i>detres.size()) return 0; return detres[i]->getElec();}
	double getHbond(int i) {if(i>detres.size()) return 0; return detres[i]->getHbond();}
	double getHydro(int i) {if(i>detres.size()) return 0; return detres[i]->getHydro();}
	double getScc(int i) {if(i>detres.size()) return 0; return detres[i]->getScc();}

	double getVdw(int i) {if(i>detres.size()) return 0; return detres[i]->getVdw();}
	int getClashes(int i) {if(i>detres.size()) return -1; return detres[i]->getClashes();}
	double getPgsolScore (int i) {if(i>detres.size()) return 0; return detres[i]->getPgsolScore();}
	double getPgsolhScore (int i) {if(i>detres.size()) return 0; return detres[i]->getPgsolhScore();}
	double getDeldispeScore (int i) {if(i>detres.size()) return 0; return detres[i]->getDeldispeScore();}

	double getDelGpol(int i) {if(i>detres.size()) return 0; return detres[i]->getDelGpol();}
	double getAreaProp(int i) {if(i>detres.size()) return 0; return detres[i]->getAreaProp();}

	int getConf(int i) {if(i>detres.size()) return -1; return detres[i]->getConf();}
	double getMat(int i, int j) {if(i>detres.size()) return 0; return detres[i]->getMat(j);}
	double getRmsd(int i) {if(i>detres.size()) return 0; return detres[i]->getRmsd();}

	int computeInterfaceStats(string pqrFileName1, string pqrFileName2, string quadFileName1, string quadFileName2, int num);

#ifdef WITH_INTERFACE_STATS
	InterfaceStats* getInterfaceStats(int i){if(i>detres.size()) return NULL; return detres[i]->getInterfaceStats();}
#endif

};

#endif
