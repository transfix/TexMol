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
#include <F2DockClient/rerankingParams.h>

using namespace std;

RerankingParams::RerankingParams()
{
	init();
}


RerankingParams::~RerankingParams()
{

}


bool RerankingParams::init(void)
{
	gpolWeight = -1;
	gnonpolWeight = 0.005;
	f2dockWeight = 0.5;
	epsilonBR = 0.7;
	epsilonGpol = 0;
	numSol = 2000;

	return true;
}

bool RerankingParams::reset(void)
{
	return init();
}


bool RerankingParams::load(string fileName)
{
	string rerankFileName(fileName);
	rerankFileName += ".rerank";

	ifstream FP(rerankFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.rerank)"<<endl;
		return false;
	}
	else
	{
		FP >> gpolWeight;
		FP >> gnonpolWeight;
		FP >> f2dockWeight;
		FP >> epsilonBR;
		FP >> epsilonGpol;
		FP >> numSol;

		FP.close();
	}
	return true;
}


bool RerankingParams::save(string fileName)
{
	string rerankFileName(fileName);
	rerankFileName += ".rerank";

	ofstream FP(rerankFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.rerank)"<<endl;
		return false;
	}
	else
	{
		FP << gpolWeight << endl;
		FP << gnonpolWeight << endl;
		FP << f2dockWeight << endl;
		FP << epsilonBR << endl;
		FP << epsilonGpol << endl;
		FP << numSol << endl;

		FP.close();
	}
	return true;
}


bool RerankingParams::prepareInputFile(string fileName)
{
	FILE *gbInpFP = fopen(fileName.c_str(), "w");

	if(!gbInpFP)
	{
		cerr << "Error creating reranking input file"<<endl;
		return false;
	}
	else
	{
		fprintf(gbInpFP, "numSol %d\n", numSol);
//		fprintf(gbInpFP, "spectrum 1:100-1:200\n");
		fprintf(gbInpFP, "GpolWeight %lf\n", gpolWeight);
		fprintf(gbInpFP, "GnonpolWeight %lf\n", gnonpolWeight);
		fprintf(gbInpFP, "F2DockScoreWeight %lf\n", f2dockWeight);
//		fprintf(gbInpFP, "distanceCutoff 1.5\n");
//		fprintf(gbInpFP, "numThreadsBR 4\n");
//		fprintf(gbInpFP, "numThreadsGpol 4\n");
		fprintf(gbInpFP, "epsilonBR %lf\n", epsilonBR);
		fprintf(gbInpFP, "epsilonGpol %lf\n", epsilonGpol);
//		fprintf(gbInpFP, "useApproxMath false\n");

		fclose(gbInpFP);
	}
	return true;
}
