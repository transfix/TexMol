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
#include <F2DockClient/basicParams.h>
#include <time.h>

using namespace std;

BasicParams::BasicParams()
{
	init();
}

BasicParams::~BasicParams()
{

}


bool BasicParams::init()
{
	isReceptorPDBAvailable = false;
	isLigandPDBAvailable = false;

	isReceptorPQRAvailable = false;
	isLigandPQRAvailable = false;

	isReceptorF2dAvailable = false;
	isLigandF2dAvailable = false;

	isReceptorRAWNAvailable = false;
	isLigandRAWNAvailable = false;

	isReceptorQuadAvailable = false;
	isLigandQuadAvailable = false;

	isRMSDAvailable = false;

	isDockingInputAvailable = false;
	isRerankingInputAvailable = false;
	isF2dGenInputAvailable = false;
	isQuadGenInputAvailable = false;

	isDockingOutputAvailable = false;
	isRerankingOutputAvailable = false;

	performRerank = false;
	storeIntermediateFiles = false;

	storeReceptorPQR = false;
	storeLigandPQR = false;
	storeReceptorF2d = false;
	storeLigandF2d = false;
	storeReceptorRAWN = false;
	storeLigandRAWN = false;
	storeReceptorQuad = false;
	storeLigandQuad = false;

	jobId = -1;
	jobType = DOCKING;
	status = INIT;

	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime (buffer,80,"%d%b%y%H%M%S",timeinfo);

	jobTitle = buffer;
	createIOFileNames(jobTitle);

	return true;
}


void BasicParams::createIOFileNames(string name)
{
	dockingInputFileName = name;
	rerankingInputFileName = name;
	f2dGenInputFileName = name;
	quadGenInputFileName = name;

	dockingOutputFileName = name;
	rerankingOutputFileName = name;

	dockingInputFileName += ".dock.inp";
	rerankingInputFileName += ".rerank.inp";
	f2dGenInputFileName += ".f2dGen.inp";
	quadGenInputFileName += ".quadGen.inp";

	dockingOutputFileName += ".dock.out";
	rerankingOutputFileName += ".rerank.out";
}



//set routines

bool BasicParams::setJobTitle(string name)
{
	jobTitle = name;
	createIOFileNames(jobTitle);
	return true;
}


bool BasicParams::setJobStatus(int st)
{
	if(st>AVAILABLE || st<INIT) 
		return false; 
	status = st; 
	return true;
}


bool BasicParams::setJobType(int t)
{
	if(t>QUADGEN) 
		return false; 
	jobType = t; 
	return true;
}



bool BasicParams::setReceptorPDBName(string fileName, int mode)	//mode = 0 means file would be opened and hence should already exist. mode = 1 means that it would be saved later
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".pdb") == 0 || fileName.compare(fileName.size()-4,4,".PDB") == 0)) return false;

	receptorPDBName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isReceptorPDBAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setLigandPDBName(string fileName, int mode)
{

  if (fileName.size() < 4) return false;


	if(!(fileName.compare(fileName.size()-4,4,".pdb") == 0 || fileName.compare(fileName.size()-4,4,".PDB") == 0)) return false;

	ligandPDBName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isLigandPDBAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setReceptorPQRName(string fileName, int mode)
{

  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".pqr") == 0 || fileName.compare(fileName.size()-4,4,".PQR") == 0)) return false;

	receptorPQRName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isReceptorPQRAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setLigandPQRName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".pqr") == 0 || fileName.compare(fileName.size()-4,4,".PQR") == 0)) return false;

	ligandPQRName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isLigandPQRAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setReceptorF2dName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".f2d") == 0 || fileName.compare(fileName.size()-4,4,".F2D") == 0)) return false;

	receptorF2dName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isReceptorF2dAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setLigandF2dName(string fileName, int mode)
{

  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".f2d") == 0 || fileName.compare(fileName.size()-4,4,".F2D") == 0)) return false;

	ligandF2dName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isLigandF2dAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setReceptorRAWNName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-5,5,".rawn") == 0 || fileName.compare(fileName.size()-5,5,".RAWN") == 0)) return false;

	receptorRAWNName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isReceptorRAWNAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setLigandRAWNName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-5,5,".rawn") == 0 || fileName.compare(fileName.size()-5,5,".RAWN") == 0)) return false;

	ligandRAWNName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isLigandRAWNAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setReceptorQuadName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-5,5,".quad") == 0 || fileName.compare(fileName.size()-5,5,".QUAD") == 0)) return false;

	receptorQuadName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isReceptorQuadAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setLigandQuadName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-5,5,".quad") == 0 || fileName.compare(fileName.size()-5,5,".QUAD") == 0)) return false;

	ligandQuadName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isLigandQuadAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setRMSDName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".txt") == 0 || fileName.compare(fileName.size()-5,5,".rmsd") == 0)) return false;

	rmsdFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP) 
		return false;
	else
	{
		isRMSDAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setDockingOutputName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

//	if(!(fileName.compare(fileName.size()-4,4,".out") == 0 || fileName.compare(fileName.size()-4,4,".OUT") == 0)) return false;

	dockingOutputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isDockingOutputAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setDockingInputName(string fileName, int mode)
{
	if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".inp") == 0 || fileName.compare(fileName.size()-4,4,".INP") == 0)) return false;

	dockingInputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isDockingInputAvailable = true;
		FP.close();
	}
	return true;
}


bool BasicParams::setRerankingOutputName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

//	if(!(fileName.compare(fileName.size()-4,4,".out") == 0 || fileName.compare(fileName.size()-4,4,".OUT") == 0)) return false;

	rerankingOutputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isRerankingOutputAvailable = true;
		FP.close();
	}
	return true;
}

bool BasicParams::setRerankingInputName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".inp") == 0 || fileName.compare(fileName.size()-4,4,".INP") == 0)) return false;

	rerankingInputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isRerankingInputAvailable = true;
		FP.close();
	}
	return true;
}
		
bool BasicParams::setF2dGenInputName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".inp") == 0 || fileName.compare(fileName.size()-4,4,".INP") == 0)) return false;

	f2dGenInputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isF2dGenInputAvailable = true;
		FP.close();
	}
	return true;
}

bool BasicParams::setQuadGenInputName(string fileName, int mode)
{
  if (fileName.size() < 4) return false;

	if(!(fileName.compare(fileName.size()-4,4,".inp") == 0 || fileName.compare(fileName.size()-4,4,".INP") == 0)) return false;

	quadGenInputFileName = fileName;

	if(mode) return true;

	ifstream FP(fileName.c_str());

	if(!FP)
		return false;
	else
	{
		isQuadGenInputAvailable = true;
		FP.close();
	}
	return true;
}


// get routines

string BasicParams::getJobId()
{
	ostringstream out;
	out << jobId;
	return out.str();
}


string BasicParams::getJobTitle()
{
	return jobTitle;
}

int BasicParams::getJobTypeInt()
{
	return jobType;
}

string BasicParams::getJobType()
{
	if(jobType == DOCKING)
	{
		string str("Docking");
		return str;
	}
	else if(jobType == RERANKING)
	{
		string str("Reranking");
		return str;
	}
	else if(jobType == F2DGEN)
	{
		string str("F2d Generation");
		return str;
	}
	else if(jobType == QUADGEN)
	{
		string str("Quad Generation");
		return str;
	}

	string str("Unknown");
	return str;
}


string BasicParams::getJobStatus()
{
	if(status == INIT)
	{
		string str("Initialized");
		return str;
	}
	else if(status == SUBMITTED)
	{
		string str("Submitted");
		return str;
	}
	else if(status == RUNNING)
	{
		string str("Running");
		return str;
	}
	else if(status == COMPLETED)
	{
		string str("Completed");
		return str;
	}
	else if(status == AVAILABLE)
	{
		string str("Available");
		return str;
	}

	string str("Unknown");
	return str;
}


// load and save routines

bool BasicParams::save(string fileName)
{
	string basicFileName(fileName);
	basicFileName += ".basic";

	ofstream FP(basicFileName.c_str());

	if(!FP)
	{
		cerr << "Error saving job (.basic)"<<endl;
		return false;
	}
	else
	{
		FP << jobId << endl;
		FP << jobType << endl;
		FP << status << endl;

		FP << isReceptorPDBAvailable << endl;
		if(isReceptorPDBAvailable) 
			FP << receptorPDBName << endl;
		else
			FP << " NA " <<endl;

		FP << isReceptorPQRAvailable << endl;
		if(isReceptorPQRAvailable) 
			FP << receptorPQRName << endl;
		else
			FP << " NA " <<endl;

		FP << isReceptorF2dAvailable << endl;
		if(isReceptorF2dAvailable) 
			FP << receptorF2dName << endl;
		else
			FP << " NA " <<endl;

		FP << isReceptorRAWNAvailable << endl;
		if(isReceptorRAWNAvailable) 
			FP << receptorRAWNName << endl;
		else
			FP << " NA " <<endl;

		FP << isReceptorQuadAvailable << endl;
		if(isReceptorQuadAvailable) 
			FP << receptorQuadName << endl;
		else
			FP << " NA " <<endl;

		FP << isLigandPDBAvailable << endl;
		if(isLigandPDBAvailable) 
		FP << ligandPDBName << endl;
		else
			FP << " NA " <<endl;

		FP << isLigandPQRAvailable << endl;
		if(isLigandPQRAvailable) 
			FP << ligandPQRName << endl;
		else
			FP << " NA " <<endl;

		FP << isLigandF2dAvailable << endl;
		if(isLigandF2dAvailable) 
			FP << ligandF2dName << endl;
		else
			FP << " NA " <<endl;

		FP << isLigandRAWNAvailable << endl;
		if(isLigandRAWNAvailable) 
			FP << ligandRAWNName << endl;
		else
			FP << " NA " <<endl;

		FP << isLigandQuadAvailable << endl;
		if(isLigandQuadAvailable) 
			FP << ligandQuadName << endl;
		else
			FP << " NA " <<endl;

		FP << isRMSDAvailable << endl;	
		if(isRMSDAvailable) 
			FP << rmsdFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isDockingInputAvailable << endl;
		if(isDockingInputAvailable) 
			FP << dockingInputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isRerankingInputAvailable << endl;
		if(isRerankingInputAvailable) 
			FP << rerankingInputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isF2dGenInputAvailable << endl;
		if(isF2dGenInputAvailable) 
			FP << f2dGenInputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isQuadGenInputAvailable << endl;
		if(isQuadGenInputAvailable) 
			FP << quadGenInputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isDockingOutputAvailable << endl;
		if(isDockingOutputAvailable) 
			FP << dockingOutputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << isRerankingOutputAvailable << endl;
		if(isRerankingOutputAvailable) 
			FP << rerankingOutputFileName << endl;
		else
			FP << " NA " <<endl;

		FP << performRerank << endl;
		FP << storeIntermediateFiles << endl;

		FP.close();
	}

	return true;
}


bool BasicParams::load(string fileName)
{
	string basicFileName(fileName);
	basicFileName += ".basic";

	ifstream FP(basicFileName.c_str());

	int temp;

	if(!FP)
	{
		cerr << "Error loading job (.basic)"<<basicFileName.c_str()<<endl;
		return false;
	}
	else
	{
		FP >> jobId;
		FP >> jobType;
		FP >> status;

		FP >> temp;
		if(temp) isReceptorPDBAvailable = true;
		else  isReceptorPDBAvailable = false;

		FP >> receptorPDBName;

		FP >> temp;
		if(temp) isReceptorPQRAvailable = true;
		else  isReceptorPQRAvailable = false;

		FP >> receptorPQRName;

		FP >> temp;
		if(temp) isReceptorF2dAvailable = true;
		else  isReceptorF2dAvailable = false;

		FP >> receptorF2dName;

		FP >> temp;
		if(temp) isReceptorRAWNAvailable = true;
		else  isReceptorRAWNAvailable = false;

		FP >> receptorRAWNName;

		FP >> temp;
		if(temp) isReceptorQuadAvailable = true;
		else  isReceptorQuadAvailable = false;

		FP >> receptorQuadName;

		FP >> temp;
		if(temp) isLigandPDBAvailable = true;
		else  isLigandPDBAvailable = false;

		FP >> ligandPDBName;

		FP >> temp;
		if(temp) isLigandPQRAvailable = true;
		else  isLigandPQRAvailable = false;

		FP >> ligandPQRName;

		FP >> temp;
		if(temp) isLigandF2dAvailable = true;
		else  isLigandF2dAvailable = false;

		FP >> ligandF2dName;

		FP >> temp;
		if(temp) isLigandRAWNAvailable = true;
		else  isLigandRAWNAvailable = false;

		FP >> ligandRAWNName;

		FP >> temp;
		if(temp) isLigandQuadAvailable = true;
		else  isLigandQuadAvailable = false;

		FP >> ligandQuadName;

		FP >> temp;
		if(temp) isRMSDAvailable = true;
		else  isRMSDAvailable = false;

		FP >> rmsdFileName;

		FP >> temp;
		if(temp) isDockingInputAvailable = true;
		else  isDockingInputAvailable = false;

		FP >> dockingInputFileName;

		FP >> temp;
		if(temp) isRerankingInputAvailable = true;
		else  isRerankingInputAvailable = false;

		FP >> rerankingInputFileName;

		FP >> temp;
		if(temp) isF2dGenInputAvailable = true;
		else  isF2dGenInputAvailable = false;

		FP >> f2dGenInputFileName;

		FP >> temp;
		if(temp) isQuadGenInputAvailable = true;
		else  isQuadGenInputAvailable = false;

		FP >> quadGenInputFileName;

		FP >> temp;
		if(temp) isDockingOutputAvailable = true;
		else  isDockingOutputAvailable = false;

		FP >> dockingOutputFileName;

		FP >> temp;
		if(temp) isRerankingOutputAvailable = true;
		else  isRerankingOutputAvailable = false;

		FP >> rerankingOutputFileName;

		FP >> temp;
		if(temp) performRerank = true;
		else  performRerank = false;

		FP >> temp;
		if(temp) storeIntermediateFiles = true;
		else  storeIntermediateFiles = false;

		FP.close();
	}

	return true;
}
