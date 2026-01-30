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
#include <F2DockClient/resultControl.h>

ResultControl::ResultControl()
{
	init();
}


ResultControl::~ResultControl()
{

}


void ResultControl::init()
{
	sumres = new SummaryResults();
	resultsReady = false;
	numStats = 0;
}


void ResultControl::setBasicParams(BasicParams *bp)
{
	basPar = bp;

	dockedOutput = basPar->getDockingOutputAvailable();
	rerankedOutput = basPar->getRerankingOutputAvailable();

	if(dockedOutput) dockingOutputFile = basPar->getDockingOutputFileName();
	if(rerankedOutput) rerankingOutputFile = basPar->getRerankingOutputFileName();

}


bool ResultControl::readResults()
{
	if(dockedOutput && rerankedOutput)
	{
		cout<<"DR"<<endl;
		resultsReady = sumres->readOutput(dockingOutputFile, rerankingOutputFile, basPar->getRMSDAvailable());
	}
	else if(dockedOutput)
	{
		cout<<"D"<<endl;
		resultsReady = sumres->readDockingOutput(dockingOutputFile, basPar->getRMSDAvailable());
	}
	else if(rerankedOutput)
	{
		cout<<"R"<<endl;
		resultsReady = sumres->readRerankingOutput(rerankingOutputFile, basPar->getRMSDAvailable());
	}
	return resultsReady;
}


bool ResultControl::computeStats(int n)
{
	if(basPar->getReceptorPQRAvailable() && basPar->getLigandPQRAvailable() && basPar->getReceptorQuadAvailable() && basPar->getLigandQuadAvailable() && resultsReady)
	{
		cout<< "Trying to compute Stats: in rescont now"<<endl;
		numStats = sumres->computeInterfaceStats(basPar->getReceptorPQRName(), basPar->getLigandPQRName(), basPar->getReceptorQuadName(), basPar->getLigandQuadName(), n);
		return true;
	}
	else
	{
		return false;
	}
}

string ResultControl::getNewRank(int i)
{
	ostringstream out; 

	int newrank = sumres->getNewRank(i);

	if(newrank!=-1)
		out << newrank;
	else
		out<<"NA";

	return out.str();
}

string ResultControl::getRank(int i)
{
	ostringstream out; 

	int rank = sumres->getRank(i);

	if(rank!=-1)
		out << rank;
	else
		out<<"NA";


	return out.str();
}

string ResultControl::getRmsd(int i)
{
	ostringstream out; 

	double rmsd = sumres->getRmsd(i);

	if(rmsd!=-1)
		out << rmsd;
	else
		out<<"NA";


	return out.str();
}

string ResultControl::getNewScore(int i)
{
	ostringstream out; 

	double newscore = sumres->getNewScore(i);

	if(newscore!=-1)
		out << newscore;
	else
		out<<"NA";


	return out.str();
}


string ResultControl::getScore(int i)
{
	ostringstream out; 

	double score = sumres->getScore(i);

	if(score!=-1)
		out << score;
	else
		out<<"NA";


	return out.str();
}


string ResultControl::getSsr(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getSsr();
	return out.str();
}

string ResultControl::getCcr(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getCcr();
	return out.str();
}

string ResultControl::getScr(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getScr();
	return out.str();
}

string ResultControl::getElec(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getElec();
	return out.str();
}

string ResultControl::getHbond(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getHbond();
	return out.str();
}

string ResultControl::getHydro(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getHydro();
	return out.str();
}

string ResultControl::getScc(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getScc();
	return out.str();
}

string ResultControl::getVdw(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getVdw();
	return out.str();
}

string ResultControl::getClashes(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getClashes();
	return out.str();
}

string ResultControl::getPgsolScore(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getPgsolScore();
	return out.str();
}

string ResultControl::getPgsolhScore(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getPgsolhScore();
	return out.str();
}

string ResultControl::getDeldispeScore(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getDeldispeScore();
	return out.str();
}

string ResultControl::getDelGpol(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getDelGpol();
	return out.str();
}

string ResultControl::getAreaProp(int i)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getAreaProp();
	return out.str();
}

string ResultControl::getMat(int i, int j)
{
	ostringstream out;
	out << sumres->getDetailResult(i)->getMat(j);
	return out.str();
}

double * ResultControl::getMat(int i)
{
	return sumres->getDetailResult(i)->getMat();
}

string ResultControl::getInterfaceArea(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getInterfaceArea();
#endif
	return out.str();
}

string ResultControl::getInterfaceVolume(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getInterfaceVolume();
#endif
	return out.str();
}

string ResultControl::getGapIndex(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getGapIndex();
#endif
	return out.str();
}

string ResultControl::getInterfaceWidth(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getInterfaceWidth();
#endif
	return out.str();
}

string ResultControl::getPlanarityIndex(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getPlanarityIndex();
#endif
	return out.str();
}

string ResultControl::getCircularityIndex(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getCircularityIndex();
#endif
	return out.str();
}

string ResultControl::getResresContact(int index, int i, int j)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getResresContact(i,j);
#endif
	return out.str();
}

string ResultControl::getNumInterfaceAtoms(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getNumInterfaceAtoms();
#endif
	return out.str();
}

string ResultControl::getNumInterfaceResidues(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getNumInterfaceResidues();
#endif
	return out.str();
}

string ResultControl::getNumPolarResidue(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getNumPolarResidue();
#endif
	return out.str();
}

string ResultControl::getNumNonPolarResidue(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getNumNonPolarResidue();
#endif
	return out.str();
}

string ResultControl::getLocalDensityIndex(int index)
{
	ostringstream out;
#ifdef WITH_INTERFACE_STATS
	out << sumres->getDetailResult(index)->getInterfaceStats()->getLocalDensityIndex();
#endif
	return out.str();
}



string ResultControl::getTotalPeaks(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getNumGoodPeaks();
	else 
		out << sumres->getNumGoodPeaksAfterRerank();

	return out.str();
}

string ResultControl::getHitsInRange(bool b, int i)
{
	ostringstream out;

	if(!b)	
		out << sumres->getHitInRange(i);
	else 
		out << sumres->getHitInRangeAfterRerank(i);

	return out.str();
}

string ResultControl::getMinRankedPeakRMSD(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getMinRankedPeakRMSD();
	else 
		out << sumres->getMinRankedPeakRMSDAfterRerank();

	return out.str();
}

string ResultControl::getMinRankedPeakScore(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getMinRankedPeakScore();
	else 
		out << sumres->getMinRankedPeakScoreAfterRerank();

	return out.str();
}

string ResultControl::getIndexMinRMSD(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getIndexMinRMSD();
	else 
		out << sumres->getIndexMinRMSDAfterRerank();

	return out.str();
}

string ResultControl::getMinRMSD(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getMinRMSD();
	else 
		out << sumres->getMinRMSDAfterRerank();

	return out.str();
}

string ResultControl::getScoreMinRMSD(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getScoreMinRMSD();
	else 
		out << sumres->getScoreMinRMSDAfterRerank();

	return out.str();
}

string ResultControl::getTime(bool b)
{
	ostringstream out;

	if(!b)	
		out << sumres->getTime();
	else 
		out << sumres->getTimeAfterRerank();

	return out.str();
}


string ResultControl::getGpolWeight()
{
	ostringstream out;
	out << rerankPar->getGpolWeight();
	return out.str();
}

string ResultControl::getNonpolWeight()
{
	ostringstream out;
	out << rerankPar->getGnonpolWeight();
	return out.str();
}

string ResultControl::getF2dockWeight()
{
	ostringstream out;
	out << rerankPar->getF2dockWeight();
	return out.str();
}

string ResultControl::getEpsilonBR()
{
	ostringstream out;
	out << rerankPar->getEpsilonBR();
	return out.str();
}

string ResultControl::getEpsilonGpol()
{
	ostringstream out;
	out << rerankPar->getEpsilonGpol();
	return out.str();
}

bool ResultControl::getApplyClashFilter(void)
{
	return dockPar->getApplyClashFilter();
}

bool ResultControl::getApplyVdWFilter(void)
{
	return dockPar->getApplyVdWFilter();
}

bool ResultControl::getApplyPseudoGsolFilter(void)
{
	return dockPar->getApplyPseudoGsolFilter();
}

bool ResultControl::getApplyDispersionFilter(void)
{
	return dockPar->getApplyDispersionFilter();
}

bool ResultControl::getApplyBasicRerank(void)
{
	return dockPar->getApplyBasicRerank();
}

string ResultControl::getComplexType(void)
{
	return dockPar->getComplexType();
}

string ResultControl::getNumSol()
{
	ostringstream out;
	out << dockPar->getNumSol();
	return out.str();
}

string ResultControl::getGridSpacing()
{
	ostringstream out;
	out << dockPar->getGridSpacing();
	return out.str();
}

string ResultControl::getRotSeparation()
{
	ostringstream out;
	out << dockPar->getRotSeparation();
	return out.str();
}

string ResultControl::getPeaksPerRotation()
{
	ostringstream out;
	out << dockPar->getPeaksPerRotation();
	return out.str();
}

string ResultControl::getSSWeight()
{
	ostringstream out;
	out << dockPar->getSSWeight();
	return out.str();
}

string ResultControl::getSCWeight()
{
	ostringstream out;
	out << dockPar->getSCWeight();
	return out.str();
}

string ResultControl::getCCWeight()
{
	ostringstream out;
	out << dockPar->getCCWeight();
	return out.str();
}

string ResultControl::getElecWeight()
{
	ostringstream out;
	out << dockPar->getElecWeight();
	return out.str();
}

string ResultControl::getHBondWeight()
{
	ostringstream out;
	out << dockPar->getHBondWeight();
	return out.str();
}

string ResultControl::getHydrophobicityWeight()
{
	ostringstream out;
	out << dockPar->getHydrophobicityWeight();
	return out.str();
}

string ResultControl::getSimpleChargeWeight()
{
	ostringstream out;
	out << dockPar->getSimpleChargeWeight();
	return out.str();
}

string ResultControl::getReceptorPDB()
{
	if(basPar->getReceptorPDBAvailable())
		return basPar->getReceptorPDBName();
	else if(basPar->getReceptorPQRAvailable())
		return basPar->getReceptorPQRName();
	else 
		return basPar->getReceptorF2dName();
}

string ResultControl::getLigandPDB()
{
	if(basPar->getLigandPDBAvailable())
		return basPar->getLigandPDBName();
	else if(basPar->getLigandPQRAvailable())
		return basPar->getLigandPQRName();
	else 
		return basPar->getLigandF2dName();
}


bool ResultControl::transform_pdb(double* trans, string input, string output)
{
	FILE* ifp = fopen(input.c_str(), "rt");

	if (ifp == NULL)
	{
		fprintf(stderr, "\n\nError: Failed to open file ( %s )!\n\n", input.c_str());
		return false;
	}


	FILE* ofp = fopen(output.c_str(), "wt");

	if (ofp == NULL)
	{
		fprintf(stderr, "\n\nError: Failed to create transformed PDB file ( %s )!\n\n", output.c_str());
		return false;
	}

	fseek(ifp, 0, SEEK_SET);
	char line[ 500 ];
	int l = 0;

	while (fgets(line, 300, ifp) != NULL)
	{
		l++;
		if (strncmp(line, "ATOM", 4) && strncmp(line, "HETATM", 6))
		{
			fprintf(ofp, "%s", line);
			continue;
		}
		double x, y, z;
		if (sscanf(line + 30, "%lf %lf %lf", &x, &y, &z) != 3)
		{
			fprintf(stderr, "\n\nError: Failed to read coordinates from line %d of input PDB file!\n\n", l);
			return false;
		}
		double nx = trans[  0 ] * x + trans[  1 ] * y + trans[  2 ] * z + trans[  3 ],
			   ny = trans[  4 ] * x + trans[  5 ] * y + trans[  6 ] * z + trans[  7 ],
			   nz = trans[  8 ] * x + trans[  9 ] * y + trans[ 10 ] * z + trans[ 11 ];
		line[ 30 ] = 0;
		fprintf(ofp, "%s%8.3lf%8.3lf%8.3lf%s", line, nx, ny, nz, line + 54);
	}

	fclose(ofp);
	return true;
}


bool ResultControl::saveConformation(int i)
{
	ostringstream conf;
	conf << i;

	string input;
	string ligandNameFragment;
	string extension("");

	if(basPar->getLigandPDBAvailable())
	{
		input = basPar->getLigandPDBName();
		extension += ".pdb";
	}
	else if (basPar->getLigandPQRAvailable())
	{
		input = basPar->getLigandPQRName(); 
		extension += ".pqr";
	}
	else 
		return false;

	string output;
	output.assign(input, 0, input.length()-4);
	output += "_";
	output += conf.str();
	output += extension;

	return transform_pdb(sumres->getDetailResult(i)->getMat(), input, output);
}
