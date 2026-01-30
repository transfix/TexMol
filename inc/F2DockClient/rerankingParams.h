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
#ifndef RERANKING_PARAMS
#define RERANKING_PARAMS

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class RerankingParams
{
	private:

	double gpolWeight;
	double gnonpolWeight;
	double f2dockWeight;
	double epsilonBR;
	double epsilonGpol;
	int numSol;

	public:
	RerankingParams();
	~RerankingParams();

	bool init(void);
	bool load(string fileName);
	bool save(string fileName);
	bool prepareInputFile(string fileName);

	//set methods
	bool reset();
	bool setGpolWeight(double gw) { gpolWeight = gw; return true;}
	bool setGnonpolWeight(double gnw) { gnonpolWeight = gnw;  return true;}
	bool setF2dockWeight(double fw) { f2dockWeight = fw;  return true;}
	bool setEpsilonBR(double e1) { epsilonBR = e1;  return true;}
	bool setEpsilonGpol(double e2) { epsilonGpol = e2;  return true;}
	bool setNumSol(int ns) { numSol = ns;  return true;}

	//get methods
	double getGpolWeight(void) {return gpolWeight;}
	double getGnonpolWeight(void) {return gnonpolWeight;}
	double getF2dockWeight(void) {return f2dockWeight;}
	double getEpsilonBR(void) {return epsilonBR;}
	double getEpsilonGpol(void) {return epsilonGpol;}
	int getNumSol(void) {return numSol;}
};




#endif
