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
#include <F2DockClient/jobParams.h>

using namespace std;

JobParams::JobParams()
{
	init();
}


JobParams::~JobParams()
{

}


bool JobParams::init()
{
	basicPar = new BasicParams();
	dockPar = new DockingParams();
	rerankPar = new RerankingParams();
	quadPar = new QuadGenParams();
	f2dPar = new F2dGenParams();
	connPar = new ConnectionParams();
	visPar = new VisualizationParams();
	return true;
}


bool JobParams::load(string fileName)
{
	string jobFileName;

	ifstream FP(fileName.c_str());
	if(!FP) return false;
	else
	{
		FP >> jobFileName;
		FP.close();

		basicPar->setJobTitle(jobFileName);
	}
	
	if( basicPar->load(jobFileName) && dockPar->load(jobFileName) && rerankPar->load(jobFileName) && f2dPar->load(jobFileName) && quadPar->load(jobFileName) && connPar->load(jobFileName) && visPar->load(jobFileName) ) return true;

	return false;
}

bool JobParams::save(string jobTitle)
{
	string jobFileName(jobTitle);
	jobFileName += ".job";

	ofstream FP(jobFileName.c_str());
	if(!FP) return false;
	else
	{
		FP << jobTitle;
		FP.close();
	}

	if( basicPar->save(jobTitle) && dockPar->save(jobTitle) && rerankPar->save(jobTitle) && f2dPar->save(jobTitle) && quadPar->save(jobTitle) && connPar->save(jobTitle) && visPar->save(jobTitle) ) return true;

	return false;
}


bool JobParams::prepareInputFiles()
{
	if(!basicPar->getDockingInputAvailable())
	{
		if(!dockPar->prepareInputFile(basicPar->getDockingInputFileName()))
			return false;
		else
			basicPar->setDockingInputAvailable(true);
	}

	if(!basicPar->getRerankingInputAvailable())
	{
		if(!rerankPar->prepareInputFile(basicPar->getRerankingInputFileName()))
			return false;
		else
			basicPar->setRerankingInputAvailable(true);
	}

	if(!basicPar->getF2dGenInputAvailable())
	{
		if(!f2dPar->prepareInputFile(basicPar->getF2dGenInputFileName()))
			return false;
		else
			basicPar->setF2dGenInputAvailable(true);
	}

	if(!basicPar->getQuadGenInputAvailable())
	{
		if(!quadPar->prepareInputFile(basicPar->getQuadGenInputFileName()))
			return false;
		else
			basicPar->setQuadGenInputAvailable(true);
	}

	return true;
}
