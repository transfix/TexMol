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
#ifndef DOCK_CONTROL
#define DOCK_CONTROL

#include <F2DockClient/dockingParams.h>
#include <F2DockClient/basicParams.h>


class DockControl
{
	private:
	DockingParams *dockPar;
	BasicParams *basPar;

	public:
	DockControl();
	~DockControl();
	
	void init();

	bool handleJob();

	// get methods
	bool getBound(void);
	int getComplexType(void);
	bool getPerformRerank(void);

	string getRotSeparation(void);
	string getGridSpacing(void);
	string getNumSol(void);
	string getNumRot(void);
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

	// set Methods
	bool reset(int complexType);

	void setDockingParams(DockingParams *rp) {dockPar = rp;}
	void setBasicParams(BasicParams *bp) {basPar = bp;}

	bool setJobTitle(string name);
	bool setReceptorPDBName(string fileName);
	bool setLigandPDBName(string fileName);

	bool setNumSol(int n);
	bool setNumRot(int n);
	bool setBound(bool b);
	bool setComplexType(int n);	
	bool setPerformRerank(bool b);
	bool setPeaksPerRotation(int n);
	bool setRotSeparation(int sep);
	bool setGridSpacing(double gs);

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

	bool setReceptorPQRName(string fileName);
	bool setLigandPQRName(string fileName);
	bool setReceptorF2dName(string fileName);
	bool setLigandF2dName(string fileName);
	bool setReceptorRAWNName(string fileName);
	bool setLigandRAWNName(string fileName);
	bool setReceptorQuadName(string fileName);
	bool setLigandQuadName(string fileName);
	bool setDockingInputName(string fileName);
	bool setRMSDName(string fileName);

	void setStoreReceptorPQR();
	void setStoreLigandPQR();
	void setStoreReceptorF2d();
	void setStoreLigandF2d();
	void setStoreReceptorRAWN();
	void setStoreLigandRAWN();
	void setStoreReceptorQuad();
	void setStoreLigandQuad();

	bool setParamsFromFile(string fileName);
};

#endif
