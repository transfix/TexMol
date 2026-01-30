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
#ifndef JOBPAR
#define JOBPAR

#include <F2DockClient/basicParams.h>
#include <F2DockClient/dockingParams.h>
#include <F2DockClient/rerankingParams.h>
#include <F2DockClient/f2dGenParams.h>
#include <F2DockClient/quadGenParams.h>
#include <F2DockClient/connectionParams.h>
#include <F2DockClient/visualizationParams.h>
#include <F2DockClient/summaryResults.h>

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class JobParams
{
	BasicParams *basicPar;
	DockingParams *dockPar;
	RerankingParams *rerankPar;
	QuadGenParams *quadPar;
	F2dGenParams *f2dPar;
	ConnectionParams *connPar;
	VisualizationParams *visPar;
	SummaryResults *sumRes;

	public:
	JobParams();
	~JobParams();

	bool load(string jobFileName);
	bool save(string jobFileName);
	bool prepareInputFiles();

	bool init();
	
	void setBasicParams(BasicParams *bp) {basicPar = bp;}
	void setDockingParams(DockingParams *dp) {dockPar = dp;}
	void setRerankingParams(RerankingParams *rp) {rerankPar = rp;}
	void setQuadParams(QuadGenParams *qp) {quadPar = qp;} 
	void setF2dGenParams(F2dGenParams *fp) {f2dPar = fp;}
	void setConnectionParams(ConnectionParams *cp) {connPar = cp;}
	void setVisualizationParams(VisualizationParams *vp) {visPar = vp;}
	void setSummaryReults(SummaryResults *sr) {sumRes = sr;}

	BasicParams * getBasicParams(void) {return basicPar;}
	DockingParams * getDockingParams(void) {return dockPar;}
	RerankingParams * getRerankingParams(void) {return rerankPar;}
	QuadGenParams * getQuadGenParams(void) {return quadPar;}
	F2dGenParams * getF2dGenParams(void) {return f2dPar;}
	ConnectionParams * getConnectionParams(void) {return connPar;}
	VisualizationParams * getVisualizationParams(void) {return visPar;}
	SummaryResults *getSummaryResults(void) {return sumRes;}
};




#endif

