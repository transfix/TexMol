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
#include <F2DockClient/dockingParams.h>

using namespace std;



DockingParams::DockingParams()
{
	init();
}


DockingParams::~DockingParams()
{

}


void DockingParams::setDefaultsBasedOnComplexType()
{
	if(complexType == ANTIBODY)
	{
		peaksPerRotation = 3;

		ssWeight = 0.73;
		scWeight = -0.31;
		ccWeight = 31;
		elecWeight = 0.72;
		hydrophobicityWeight = 8.5;
		simpleChargeWeight = 0.1;
	}

	else if(complexType == ENZYME)
	{
		peaksPerRotation = 2;

		ssWeight = 0.78;
		scWeight = -0.08;
		ccWeight = 5;
		elecWeight = 0.15;
		hydrophobicityWeight = 9.0;
		simpleChargeWeight = 5.5;
	}

	else if(complexType == NEITHER)
	{
		peaksPerRotation = 5;

		ssWeight = 0.57;
		scWeight = -0.23;
		ccWeight = 5;
		elecWeight = 0.72;
		hydrophobicityWeight = 8.5;
		simpleChargeWeight = 2.0;
	}

	else if(complexType == UNKNOWN)
	{
		peaksPerRotation = 1;
		ssWeight = 0.7;
		scWeight = -0.2;
		ccWeight = 17;
		elecWeight = 0.86;
		hydrophobicityWeight = 8.5;
		simpleChargeWeight = 2.0;
	}
}


void DockingParams::setDefaults()
{
	rotSeparation = 15;
	gridSpacing = 1.2;
	numSol = 20000;
	numRot = 54000;
	peaksPerRotation = 1;

	applyClashFilter = true;
	applyVdWFilter = true;
	applyPseudoGsolFilter = true;
	applyDispersionFilter = false;
	applyBasicRerank = false;

	ssWeight = 0.7;
	scWeight = -0.2;
	ccWeight = 17;
	elecWeight = 0.86;
	hBondWeight = 0;
	hydrophobicityWeight = 8.5;
	simpleChargeWeight = 2.0;
}


bool DockingParams::init()
{
	isBound = false;
	complexType = UNKNOWN;

	setDefaults();
	return true;
}


// set methods
bool DockingParams::reset()
{
	setDefaults();
	return true;
}

bool DockingParams::reset(int c)
{
	if(!setComplexType(c)) return false;
	setDefaults();
	setDefaultsBasedOnComplexType();
	return true;		
}

bool DockingParams::setBound(bool b)
{
	isBound = b;
	return true;
}

bool DockingParams::setComplexType(int c)
{
	if(c>UNKNOWN)
	{
		return false;
	}	
	else
	{
		complexType = c;
		return true;
	}
}

bool DockingParams::setRotSeparation(int sep)
{
	if(sep == 4 || sep == 6 || sep == 8 || sep == 10 || sep == 12 || sep == 15 || sep == 20)
	{
		rotSeparation = sep;
		return true;
	}
	return false;
}

bool DockingParams::setGridSpacing(double gs)
{
	if(gs<0) return false;
	
	gridSpacing = gs;
	return true;
}

bool DockingParams::setNumSol(int n)
{
	if(n > numRot*peaksPerRotation) return false;
	
	numSol = n;
	return true;
}

bool DockingParams::setNumRot(int n)
{
	numRot = n;
	return true;
}


bool DockingParams::setPeaksPerRotation(int n)
{
	if(n<0 || n>10) return false;
	
	peaksPerRotation = n;
	return true;
}

bool DockingParams::setSSWeight(double ss)
{
	ssWeight = ss;
	return true;
}

bool DockingParams::setSCWeight(double sc)
{
	scWeight = sc;
	return true;
}

bool DockingParams::setCCWeight(double cc)
{
	ccWeight = cc;
	return true;
}

bool DockingParams::setElecWeight(double elec)
{
	elecWeight = elec;
	return true;
}

bool DockingParams::setHBondWeight(double hw)
{
	hBondWeight = hw;
	return true;
}

bool DockingParams::setHydrophobicityWeight(double hw)
{
	hydrophobicityWeight = hw;
	return true;
}

bool DockingParams::setSimpleChargeWeight(double sw)
{
	simpleChargeWeight = sw;
	return true;
}

void DockingParams::setApplyClashFilter(bool cf)
{
	applyClashFilter = cf;
}

void DockingParams::setApplyVdWFilter(bool vf)
{
	applyVdWFilter = vf;
}

void DockingParams::setApplyPseudoGsolFilter(bool pf)
{
	applyPseudoGsolFilter = pf;
}

void DockingParams::setApplyDispersionFilter(bool df)
{
	applyDispersionFilter = df;
}

void DockingParams::setApplyBasicRerank(bool br)
{
	applyBasicRerank = br;
}

string DockingParams::getComplexType(void)
{
	string complex;	

	if(complexType == ANTIBODY) complex.assign("Antibody-Antigen"); 
	else if (complexType == ENZYME) complex.assign("Enzyme-Inhibitor/Substrate"); 
	else if (complexType == NEITHER) complex.assign("Neither Enzyme/Antibody"); 
	else if (complexType == UNKNOWN) complex.assign("Unknown"); 
	
	return complex;
}


bool DockingParams::load(string fileName)
{
	string dockFileName(fileName);
	dockFileName += ".dock";

	ifstream FP(dockFileName.c_str());	

	int temp;

	if(!FP)
	{
		cerr << "Error loading job (.dock)"<<endl;
		return false;
	}
	else
	{
		FP >> isBound;
		FP >> complexType;

		FP >> rotSeparation;
		FP >> gridSpacing;
		FP >> numSol;
		FP >> peaksPerRotation;

		FP >> ssWeight;
		FP >> scWeight;
		FP >> ccWeight;
		FP >> elecWeight;
		FP >> hBondWeight;
		FP >> hydrophobicityWeight;
		FP >> simpleChargeWeight;

		FP >> temp;
		if(temp) applyClashFilter = true;
		else applyClashFilter = false;

		FP >> temp;
		if(temp) applyVdWFilter = true;
		else applyVdWFilter = false;

		FP >> temp;
		if(temp) applyPseudoGsolFilter = true;
		else applyPseudoGsolFilter = false;

		FP >> temp;
		if(temp) applyDispersionFilter = true;
		else applyDispersionFilter = false;

		FP >> temp;
		if(temp) applyBasicRerank = true;
		else applyBasicRerank = false;

		FP.close();
	}
	return true;
}


bool DockingParams::save(string fileName)
{
	string dockFileName(fileName);
	dockFileName += ".dock";

	ofstream FP(dockFileName.c_str());	

	if(!FP)
	{
		cerr << "Error saving job (.dock)"<<endl;
		return false;
	}
	else
	{
		FP << isBound << endl;
		FP << complexType << endl;

		FP << rotSeparation << endl;
		FP << gridSpacing << endl;
		FP << numSol << endl;
		FP << peaksPerRotation << endl;

		FP << ssWeight << endl;
		FP << scWeight << endl;
		FP << ccWeight << endl;
		FP << elecWeight << endl;
		FP << hBondWeight << endl;
		FP << hydrophobicityWeight << endl;
		FP << simpleChargeWeight << endl;

		FP << applyClashFilter << endl;
		FP << applyVdWFilter << endl;
		FP << applyPseudoGsolFilter << endl;
		FP << applyDispersionFilter << endl;
		FP << applyBasicRerank << endl;

		FP.close();
	}
	return true;
}


bool DockingParams::prepareInputFile(string fileName)
{
	FILE *inpFP = fopen(fileName.c_str(), "w");

	char cType[4] = {'A', 'E', 'G', 'U'};

	if(!inpFP)
	{
		cerr << "Error creating docking input file"<<endl;
		return false;
	}
	else
	{
		fprintf(inpFP, "complexType %c\n", cType[complexType]);

		fprintf(inpFP, "rotFile deg%02d.mtx\n", rotSeparation);
		fprintf(inpFP, "numRot %d\n", numRot);
		fprintf(inpFP, "effGridFile fftw-rank.txt\n");
		fprintf(inpFP, "gridSpacing %lf\n", gridSpacing);
		fprintf(inpFP, "numSolutions %d\n", numSol);
		fprintf(inpFP, "peaksPerRotation %d\n", peaksPerRotation);

		fprintf(inpFP, "skinSkinWeight %lf\n", ssWeight);
		fprintf(inpFP, "coreCoreWeight %lf\n", ccWeight);
		fprintf(inpFP, "skinCoreWeight %lf\n", scWeight);
		fprintf(inpFP, "elecWeight %lf\n", elecWeight);
		fprintf(inpFP, "hbondWeight %lf\n", hBondWeight);
		fprintf(inpFP, "hydrophobicityWeight %lf\n", hydrophobicityWeight);
		fprintf(inpFP, "simpleChargeWeight %lf\n", simpleChargeWeight);

		fprintf(inpFP, "applyVDWFilter %s\n", applyVdWFilter?"true":"false");
		fprintf(inpFP, "applyClashFilter %s\n", applyClashFilter?"true":"false");
		fprintf(inpFP, "applyPseudoGsolFilter %s\n", applyPseudoGsolFilter?"true":"false");
		fprintf(inpFP, "applyDispersionFilter %s\n", applyDispersionFilter?"true":"false");
		fprintf(inpFP, "applyBasicRerank %s\n", applyBasicRerank?"true":"false");
		fprintf(inpFP, "numThreads %d\n", 4);
		fprintf(inpFP, "breakDownScores false\n");

		fclose(inpFP);
	}
	return true;
}

