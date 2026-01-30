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
#ifndef RERANK_CONTROL
#define RERANK_CONTROL

#include <F2DockClient/rerankingParams.h>
#include <F2DockClient/basicParams.h>


class RerankControl
{
	private:
	RerankingParams *rerankPar;
	BasicParams *basPar;

	public:
	RerankControl();
	~RerankControl();
	
	void init();

	bool handleJob();

	// get methods
	string getNumsol();
	string getGpolWeight();
	string getNonpolWeight();
	string getF2dockWeight();
	string getEpsilonBR();
	string getEpsilonGpol();

	// set Methods
	void setRerankingParams(RerankingParams *rp) {rerankPar = rp;}
	void setBasicParams(BasicParams *bp) {basPar = bp;}

	bool reset();

	bool setJobTitle(string name);
	bool setReceptorPDBName(string fileName);
	bool setLigandPDBName(string fileName);
	bool setDockingOutputName(string fileName);
	bool setNumsol(int n);

	bool setGpolWeight(double w);
	bool setNonpolWeight(double w);
	bool setF2dockWeight(double w);
	bool setEpsilonBR(double e);
	bool setEpsilonGpol(double e);

	bool setReceptorPQRName(string fileName);
	bool setLigandPQRName(string fileName);
	bool setReceptorRAWNName(string fileName);
	bool setLigandRAWNName(string fileName);
	bool setReceptorQuadName(string fileName);
	bool setLigandQuadName(string fileName);

	void setStoreReceptorPQR();
	void setStoreLigandPQR();
	void setStoreReceptorRAWN();
	void setStoreLigandRAWN();
	void setStoreReceptorQuad();
	void setStoreLigandQuad();

	bool setParamsFromFile(string fileName);
};

#endif
