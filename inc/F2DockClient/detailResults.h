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
#ifndef DETRES_STRUCT_H
#define DETRES_STRUCT_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

#ifdef WITH_INTERFACE_STATS
#include <InterfaceStats/InterfaceStats.h>
#endif

#include <UsefulMath/Matrix.h>

using namespace std;

class DetailResults
{
	bool _isDockingOutput;

	int rank; 			// Rank of this docking result according to the F2Dock scoring function
	int newRank;			// rank after GBRerank
	double score; 			// overall F2Dock score, the higher the better
	double newScore;		// score after GBRerank

	double shape;
	double ssr; 			
	double scr; 		
	double ccr; 			
	double elec; 		
	double hbond;
	double hydro;
	double scc;

	double vdwScore;
	int clashes;
	double pgsolScore;
	double pgsolhScore;
	double deldispeScore;

	double delGpol;
	double areaProp;

	double mat[12];
	int conf;
	double rmsd;
#ifdef WITH_INTERFACE_STATS
	InterfaceStats* intStat;
#endif

	public:
	DetailResults(bool isDockingOutput);
	DetailResults();
	~DetailResults();

	bool init();

	//set methods
	void setRank(int r) {rank = r;} 			
	void setNewRank(int nr) {newRank = nr;}		
	void setScore(double s) {score = s;} 		
	void setNewScore(double ns) {newScore = ns;}	

	void setShape(double s) {shape = s;}
	void setSsr(double s) {ssr = s;} 		
	void setCcr(double s) {ccr = s;}		
	void setScr(double s) {scr = s;}		
	void setElec(double s) {elec = s;}
	void setHbond(double s) {hbond = s;}
	void setHydro(double h) {hydro = h;}
	void setScc (double s) {scc = s;}


	void setVdw(double s) {vdwScore = s;}
	void setClashes(int c) {clashes = c;}
	void setPgsolScore(double p) {pgsolScore = p;}
	void setPgsolhScore(double p) {pgsolhScore = p;}
	void setDeldispeScore(double p) {deldispeScore = p;}


	void setDelGpol(double g) {delGpol = g;}
	void setAreaProp(double a) {areaProp = a;}

	void setMat(double *m) {for(int i=0;i<12;i++) mat[i] = m[i];}
	void setConf(int c) {conf = c;}
	void setRmsd(double r) {rmsd = r;}

#ifdef WITH_INTERFACE_STATS
	void setInterfaceStats(InterfaceStats *is){intStat = is;}
#endif

	//get methods
	bool isDockingOutput(void) {return _isDockingOutput;}

	int getRank(void) {return rank;}
	int getNewRank(void) {return newRank;}
	double getScore(void) {return score;}
	double getNewScore(void) {return newScore;}

	double getShape(void) {return shape;}
	double getSsr(void) {return ssr;}
	double getCcr(void) {return ccr;}
	double getScr(void) {return scr;}
	double getElec(void) {return elec;}
	double getHbond(void) {return hbond;}
	double getHydro(void) {return hydro;}
	double getScc(void) {return scc;}

	double getVdw(void) {return vdwScore;}
	int getClashes(void) {return clashes;}
	double getPgsolScore (void) {return pgsolScore;}
	double getPgsolhScore (void) {return pgsolhScore;}
	double getDeldispeScore (void) {return deldispeScore;}

	double getDelGpol(void) {return delGpol;}
	double getAreaProp(void) {return areaProp;}

	int getConf(void) {return conf;}
	double getMat(int i) {return mat[i];}
	CCVOpenGLMath::Matrix getTransformationMatrix();
	double* getMat(void) {return mat;}
	double getRmsd(void) {return rmsd;}

#ifdef WITH_INTERFACE_STATS
	InterfaceStats* getInterfaceStats(void){return intStat;}
#endif
};
#endif
