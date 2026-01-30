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
#ifndef RESULT_CONTROL
#define RESULT_CONTROL

#include <F2DockClient/basicParams.h>
#include <F2DockClient/dockingParams.h>
#include <F2DockClient/rerankingParams.h>
#include <F2DockClient/visualizationParams.h>
#include <F2DockClient/summaryResults.h>

#ifdef WITH_INTERFACE_STATS
#include <InterfaceStats/InterfaceStats.h>
#endif

class ResultControl
{
	private:
	BasicParams *basPar;
	DockingParams *dockPar;
	RerankingParams *rerankPar;
	VisualizationParams *visPar;

	SummaryResults *sumres;
	
	int selectedResult;

	string dockingOutputFile;
	string rerankingOutputFile;

	bool dockedOutput;
	bool rerankedOutput;
	
	bool resultsReady;
	int numStats;


	public:
	ResultControl();
	~ResultControl();
	
	void init();

	//set Methods
	void setVisualizationParams(VisualizationParams *vp) {visPar = vp;}
	void setDockingParams(DockingParams *dp) {dockPar = dp;}
	void setRerankingParams(RerankingParams *rp) {rerankPar = rp;}
	void setBasicParams(BasicParams *bp);
	bool readResults();
	bool computeStats(int n);
	int getNumStats(){return numStats;}

	bool setUofBVis(bool b) {visPar->setDisplayType(UOFB_SURF_VIS); return true;}
	bool setSurfVis(bool b) {visPar->setDisplayType(SMOOTH_SURF_VIS); return true;}
	bool setPotVis(bool b) {visPar->setDisplayType(POTENTIAL_VIS); return true;}

	bool setCoulVis(bool b) {visPar->setPotentialType(COULOMBIC_POT); return true;}
	bool setPBVis(bool b) {visPar->setPotentialType(GB_POT); return true;}
	bool setGBVis(bool b) {visPar->setPotentialType(PB_POT); return true;}

	bool setMultipleVis(bool b) {visPar->setMultipleLigand(b); return true;}
	bool setTransparentInterface(bool b) {visPar->setHighlightInterface(b); return true;}

	void setResultIndex(int i) {selectedResult = i;}

	
	//get Methods
	
	bool getMode(void) {return sumres->isRerankingOutput();}
	bool getRMSDMode(void) {return basPar->getRMSDAvailable();}
	int getSize(void) {return sumres->getSize();}

	string getTotalPeaks(bool b);
	string getHitsInRange(bool b, int i);
	string getMinRankedPeakRMSD(bool b);
	string getMinRankedPeakScore(bool b);
	string getIndexMinRMSD(bool b);
	string getMinRMSD(bool b);
	string getScoreMinRMSD(bool b);
	string getTime(bool b);


	string getNewRank(int i);
	string getRank(int i);
	string getRmsd(int i);
	string getNewScore(int i);
	string getScore(int i);


	string getSsr(int i);
	string getCcr(int i);
	string getScr(int i);

	string getElec(int i);
	string getHbond(int i);
	string getHydro(int i);
	string getScc(int i);

	string getVdw(int i);
	string getClashes(int i);
	string getPgsolScore(int i);
	string getPgsolhScore(int i);
	string getDeldispeScore(int i);

	string getDelGpol(int i);
	string getAreaProp(int i);

	double *getMat(int i);
	string getMat(int i, int j);
	bool isSurface(){if(visPar->getReceptorDisplayType() == SMOOTH_SURF_VIS) return true; return false;}
	bool isPotential(){if(visPar->getReceptorDisplayType() == POTENTIAL_VIS) return true; return false;}
	int getPotentialType(){return visPar->getReceptorPotentialType();}
	bool isMultipleLigand(){return visPar->isMultipleLigand();}

	string getReceptorPDB();
	string getLigandPDB();

	string getGpolWeight();
	string getNonpolWeight();
	string getF2dockWeight();
	string getEpsilonBR();
	string getEpsilonGpol();

	string getRotSeparation(void);
	string getComplexType(void);
	string getGridSpacing(void);
	string getNumSol(void);
	string getPeaksPerRotation(void);

	bool getApplyClashFilter(void);
	bool getApplyVdWFilter(void);
	bool getApplyPseudoGsolFilter(void);
	bool getApplyDispersionFilter(void);
	bool getApplyBasicRerank(void);

	string getSSWeight();
	string getSCWeight();
	string getCCWeight();
	string getElecWeight();
	string getHBondWeight();
	string getHydrophobicityWeight();
	string getSimpleChargeWeight();

	string getInterfaceArea(int index);
	string getInterfaceVolume(int index);
	string getGapIndex(int index);
	string getInterfaceWidth(int index);

	string getPlanarityIndex(int index);
	string getCircularityIndex(int index);

	string getResresContact(int index, int i, int j);

	string getNumInterfaceAtoms(int index);
	string getNumInterfaceResidues(int index);

	string getNumPolarResidue(int index);
	string getNumNonPolarResidue(int index);

	string getLocalDensityIndex(int index);

	bool transform_pdb(double* trans, string input, string output);
	bool saveConformation(int i);
};

#endif
