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
#ifndef DOCKING_PARAMS
#define DOCKING_PARAMS

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;



class DockingParams
{
	private:
	bool isBound;
	int complexType;

	int rotSeparation;
	double gridSpacing;
	int numSol;
	int numRot;
	int peaksPerRotation;

	double ssWeight;
	double scWeight;
	double ccWeight;
	double elecWeight;
	double hBondWeight;
	double hydrophobicityWeight;
	double simpleChargeWeight;

/*	bool singleLayerSkin;
	double pseudoAtomRadius;
	bool curvatureWeightedReceptorSkin;
	bool curvatureWeightedLigandSkin;
	double curvatureWeightingRadius;*/

	bool applyClashFilter;
	bool applyVdWFilter;
	bool applyPseudoGsolFilter;
	bool applyDispersionFilter;
	bool applyBasicRerank;

	void setDefaultsBasedOnComplexType();
	void setDefaults();

	public:
	enum {ANTIBODY, ENZYME, NEITHER, UNKNOWN};

	DockingParams();
	~DockingParams();

	bool init();

	bool load(string fileName);
	bool save(string fileName);
	bool prepareInputFile(string fileName);


	//set methods
	bool reset();
	bool reset(int complexType);

	bool setBound(bool b);
	bool setComplexType(int c);

	bool setRotSeparation(int sep);
	bool setGridSpacing(double gs);
	bool setNumSol(int n);
	bool setNumRot(int n);
	bool setPeaksPerRotation(int n);

	bool setSSWeight(double ss);
	bool setSCWeight(double sc);
	bool setCCWeight(double cc);
	bool setElecWeight(double elec);
	bool setHBondWeight(double hw);
	bool setHydrophobicityWeight(double hw);
	bool setSimpleChargeWeight(double sw);

	void setApplyClashFilter(bool cf);
	void setApplyVdWFilter(bool vf);
	void setApplyPseudoGsolFilter(bool pf);
	void setApplyDispersionFilter(bool df);
	void setApplyBasicRerank(bool br);

	//get methods

	bool getBound(void) {return isBound;}
	int getComplexTypeInt(void) {return complexType;}
	string getComplexType(void);

	int getRotSeparation(void) {return rotSeparation;}
	double getGridSpacing(void) {return gridSpacing;}
	int getNumSol(void) {return numSol;}
	int getNumRot(void) {return numRot;}
	int getPeaksPerRotation(void) {return peaksPerRotation;}

	double getSSWeight(void) {return ssWeight;}
	double getSCWeight(void) {return scWeight;}
	double getCCWeight(void) {return ccWeight;}
	double getElecWeight(void) {return elecWeight;}
	double getHBondWeight(void) {return hBondWeight;}
	double getHydrophobicityWeight(void) {return hydrophobicityWeight;}
	double getSimpleChargeWeight(void) {return simpleChargeWeight;}

	bool getApplyClashFilter(void) {return applyClashFilter;}
	bool getApplyVdWFilter(void) {return applyVdWFilter;}
	bool getApplyPseudoGsolFilter(void) {return applyPseudoGsolFilter;}
	bool getApplyDispersionFilter(void) {return applyDispersionFilter;}
	bool getApplyBasicRerank(void) {return applyBasicRerank;}
};



#endif
