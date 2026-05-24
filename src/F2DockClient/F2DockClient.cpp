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
#include <F2DockClient/F2DockClient.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <TexMol/compat.h>

using namespace XmlRpc;
using namespace std;

F2DockClient::F2DockClient()
{
	FILE* parfile;
	parfile = fopen("host.ini", "rt");
	if (!parfile)
	{
		cout<<"Host file not found. Using default values"<<endl;
		sprintf(hostName,"prism2.ices.utexas.edu");
		port = 8091;
	}
	else
	{
		fscanf(parfile, "%s", hostName);
		fscanf(parfile, "%d", &port);
		fclose(parfile);
		cout<<"Hostname =  "<<hostName<<endl;
		cout<<"Port = "<< port<<endl;
	}
}


F2DockClient::~F2DockClient()
{

}



bool F2DockClient::submitRequest(JobParams *jobPar)
{
	XmlRpcClient c(jobPar->getConnectionParams()->getHostName().c_str(), jobPar->getConnectionParams()->getPort());

	string line;

	if(!jobPar->getBasicParams()->getReceptorPDBAvailable())
	{
		args[0] = string("false");
		args[1] = string("");
	}
	else
	{
		args[0] = string("true");
		ifstream receptorPDBFile(jobPar->getBasicParams()->getReceptorPDBName().c_str());

		if (!receptorPDBFile)
		{
			cerr << "Unable to open receptor pdb File ";
			return false;
		}

		string receptorPDB;

		while (getline(receptorPDBFile, line))
		{
			receptorPDB += line + "\n";
		}
		receptorPDBFile.close();

		args[1] = receptorPDB;
	}

	if(!jobPar->getBasicParams()->getLigandPDBAvailable())
	{
		args[2] = string("false");
		args[3] = string("");
	}
	else
	{
		args[2] = string("true");
		ifstream ligandPDBFile(jobPar->getBasicParams()->getLigandPDBName().c_str());

		if (!ligandPDBFile)
		{
			cerr << "Unable to open ligand pdb File ";
			return false;
		}

		string ligandPDB;

		while (getline(ligandPDBFile, line))
		{
			ligandPDB += line + "\n";
		}
		ligandPDBFile.close();

		args[3] = ligandPDB;
	}



	if(!jobPar->getBasicParams()->getReceptorPQRAvailable())
	{
		args[4] = string("false");
		args[5] = string("");
	}
	else
	{
		args[4] = string("true");
		ifstream receptorPQRFile(jobPar->getBasicParams()->getReceptorPQRName().c_str());

		if (!receptorPQRFile)
		{
			cerr << "Unable to open receptor pqr File ";
			return false;
		}

		string receptorPQR;

		while (getline(receptorPQRFile, line))
		{
			receptorPQR += line + "\n";
		}
		receptorPQRFile.close();

		args[5] = receptorPQR;
	}

	if(!jobPar->getBasicParams()->getLigandPQRAvailable())
	{
		args[6] = string("false");
		args[7] = string("");
	}
	else
	{
		args[6] = string("true");
		ifstream ligandPQRFile(jobPar->getBasicParams()->getLigandPQRName().c_str());

		if (!ligandPQRFile)
		{
			cerr << "Unable to open ligand pqr File ";
			return false;
		}

		string ligandPQR;

		while (getline(ligandPQRFile, line))
		{
			ligandPQR += line + "\n";
		}
		ligandPQRFile.close();

		args[7] = ligandPQR;
	}



	if(!jobPar->getBasicParams()->getReceptorF2dAvailable())
	{
		args[8] = string("false");
		args[9] = string("");
	}
	else
	{
		args[8] = string("true");
		ifstream receptorF2dFile(jobPar->getBasicParams()->getReceptorF2dName().c_str());

		if (!receptorF2dFile)
		{
			cerr << "Unable to open receptor f2d File ";
			return false;
		}

		string receptorF2d;

		while (getline(receptorF2dFile, line))
		{
			receptorF2d += line + "\n";
		}
		receptorF2dFile.close();

		args[9] = receptorF2d;
	}

	if(!jobPar->getBasicParams()->getLigandF2dAvailable())
	{
		args[10] = string("false");
		args[11] = string("");
	}
	else
	{
		args[10] = string("true");
		ifstream ligandF2dFile(jobPar->getBasicParams()->getLigandF2dName().c_str());

		if (!ligandF2dFile)
		{
			cerr << "Unable to open ligand f2d File ";
			return false;
		}

		string ligandF2d;

		while (getline(ligandF2dFile, line))
		{
			ligandF2d += line + "\n";
		}
		ligandF2dFile.close();

		args[11] = ligandF2d;
	}



	if(!jobPar->getBasicParams()->getReceptorRAWNAvailable())
	{
		args[12] = string("false");
		args[13] = string("");
	}
	else
	{
		args[12] = string("true");
		ifstream receptorRAWNFile(jobPar->getBasicParams()->getReceptorRAWNName().c_str());

		if (!receptorRAWNFile)
		{
			cerr << "Unable to open receptor rawn File ";
			return false;
		}

		string receptorRAWN;

		while (getline(receptorRAWNFile, line))
		{
			receptorRAWN += line + "\n";
		}
		receptorRAWNFile.close();

		args[13] = receptorRAWN;
	}

	if(!jobPar->getBasicParams()->getLigandRAWNAvailable())
	{
		args[14] = string("false");
		args[15] = string("");
	}
	else
	{
		args[14] = string("true");
		ifstream ligandRAWNFile(jobPar->getBasicParams()->getLigandRAWNName().c_str());

		if (!ligandRAWNFile)
		{
			cerr << "Unable to open ligand rawn File ";
			return false;
		}

		string ligandRAWN;

		while (getline(ligandRAWNFile, line))
		{
			ligandRAWN += line + "\n";
		}
		ligandRAWNFile.close();

		args[15] = ligandRAWN;
	}



	if(!jobPar->getBasicParams()->getReceptorQuadAvailable())
	{
		args[16] = string("false");
		args[17] = string("");
	}
	else
	{
		args[16] = string("true");
		ifstream receptorQuadFile(jobPar->getBasicParams()->getReceptorQuadName().c_str());

		if (!receptorQuadFile)
		{
			cerr << "Unable to open receptor quad File ";
			return false;
		}

		string receptorQuad;

		while (getline(receptorQuadFile, line))
		{
			receptorQuad += line + "\n";
		}
		receptorQuadFile.close();

		args[17] = receptorQuad;
	}

	if(!jobPar->getBasicParams()->getLigandQuadAvailable())
	{
		args[18] = string("false");
		args[19] = string("");
	}
	else
	{
		args[18] = string("true");
		ifstream ligandQuadFile(jobPar->getBasicParams()->getLigandQuadName().c_str());

		if (!ligandQuadFile)
		{
			cerr << "Unable to open ligand quad File ";
			return false;
		}

		string ligandQuad;

		while (getline(ligandQuadFile, line))
		{
			ligandQuad += line + "\n";
		}
		ligandQuadFile.close();

		args[19] = ligandQuad;
	}



	if(!jobPar->getBasicParams()->getDockingInputAvailable())
	{
		args[20] = string("false");
		args[21] = string("");
	}
	else
	{
		args[20] = string("true");
		ifstream dockingInputFile(jobPar->getBasicParams()->getDockingInputFileName().c_str());

		if (!dockingInputFile)
		{
			cerr << "Unable to open docking input File ";
			return false;
		}

		string dockingInput;

		while (getline(dockingInputFile, line))
		{
			dockingInput += line + "\n";
		}
		dockingInputFile.close();

		args[21] = dockingInput;
	}



	if(!jobPar->getBasicParams()->getRerankingInputAvailable())
	{
		args[22] = string("false");
		args[23] = string("");
	}
	else
	{
		args[22] = string("true");
		ifstream rerankingInputFile(jobPar->getBasicParams()->getRerankingInputFileName().c_str());

		if (!rerankingInputFile)
		{
			cerr << "Unable to open reranking input File ";
			return false;
		}

		string rerankingInput;

		while (getline(rerankingInputFile, line))
		{
			rerankingInput += line + "\n";
		}
		rerankingInputFile.close();

		args[23] = rerankingInput;
	}



	if(!jobPar->getBasicParams()->getF2dGenInputAvailable())
	{
		args[24] = string("false");
		args[25] = string("");
	}
	else
	{
		args[24] = string("true");
		ifstream f2dGenInputFile(jobPar->getBasicParams()->getF2dGenInputFileName().c_str());

		if (!f2dGenInputFile)
		{
			cerr << "Unable to open f2dgen input File ";
			return false;
		}

		string f2dGenInput;

		while (getline(f2dGenInputFile, line))
		{
			f2dGenInput += line + "\n";
		}
		f2dGenInputFile.close();

		args[25] = f2dGenInput;
	}



	if(!jobPar->getBasicParams()->getQuadGenInputAvailable())
	{
		args[26] = string("false");
		args[27] = string("");
	}
	else
	{
		args[26] = string("true");
		ifstream quadGenInputFile(jobPar->getBasicParams()->getQuadGenInputFileName().c_str());

		if (!quadGenInputFile)
		{
			cerr << "Unable to open quadgen input File ";
			return false;
		}

		string quadGenInput;

		while (getline(quadGenInputFile, line))
		{
			quadGenInput += line + "\n";
		}
		quadGenInputFile.close();

		args[27] = quadGenInput;
	}



	if(!jobPar->getBasicParams()->getRMSDAvailable())
	{
		args[28] = string("false");
		args[29] = string("");
	}
	else
	{
		args[28] = string("true");
		ifstream RMSDFile(jobPar->getBasicParams()->getRmsdFileName().c_str());

		if (!RMSDFile)
		{
			cerr << "Unable to open rmsd File ";
			return false;
		}

		string RMSD;

		while (getline(RMSDFile, line))
		{
			RMSD += line + "\n";
		}
		RMSDFile.close();

		args[29] = RMSD;
	}



	if(!jobPar->getBasicParams()->getDockingOutputAvailable())
	{
		args[30] = string("false");
		args[31] = string("");
	}
	else
	{
		args[30] = string("true");
		ifstream dockingOutputFile(jobPar->getBasicParams()->getDockingOutputFileName().c_str());

		if (!dockingOutputFile)
		{
			cerr << "Unable to open docking output File "<<jobPar->getBasicParams()->getDockingOutputFileName().c_str()<<endl;
			return false;
		}

		string dockingOutput;

		while (getline(dockingOutputFile, line))
		{
			dockingOutput += line + "\n";
		}
		dockingOutputFile.close();

		args[31] = dockingOutput;
	}


	if(!jobPar->getBasicParams()->getRerankingOutputAvailable())
	{
		args[32] = string("false");
		args[33] = string("");
	}
	else
	{
		args[32] = string("true");
		ifstream rerankingOutputFile(jobPar->getBasicParams()->getRerankingOutputFileName().c_str());

		if (!rerankingOutputFile)
		{
			cerr << "Unable to open reranking output File ";
			return false;
		}

		string rerankingOutput;

		while (getline(rerankingOutputFile, line))
		{
			rerankingOutput += line + "\n";
		}
		rerankingOutputFile.close();

		args[33] = rerankingOutput;
	}

	if(jobPar->getBasicParams()->getPerformRerank())
		args[34] = string("true");
	else
		args[34] = string("false");

	if(jobPar->getBasicParams()->getStoreIntermediateFiles())
		args[35] = string("true");
	else
		args[35] = string("false");

	args[36] = jobPar->getBasicParams()->getJobTypeInt();

	cout<<"Prepared the XMLRPC values. Submitting request"<<endl;

	if (c.execute("Submit", args, id))
	{
		if(!c.isFault())
		{
			int jobId = int(id);

			if(jobId==-1)
			{
				std::cout << "Error in server\n\n";
				return false;
			}
				
			cout<<"Request submitted"<<endl;
			jobPar->getBasicParams()->setJobId(jobId);
			jobPar->getBasicParams()->setJobStatus(SUBMITTED);
	
			return true;
		}
		else
		{
			std::cout << "Error in server\n\n";
			return false;
		}
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return false;
	}
	return true;
}



bool F2DockClient::getResult(int id, string fileName, int index)
{
	XmlRpcClient c(hostName, port);
	ofstream outputFP;

	args[0] = id;
	args[1] = index;

	if (c.execute("GetOutput", args, result))
	{
		if(!c.isFault())
		{
			string file = string(result);
			if (!file.compare("INCOMPLETE"))
			{
				cerr << "File not ready.";
				return false;
			}
			else
			{
				outputFP.open(fileName.c_str());

				if (!outputFP)
				{
					cerr << "Error creating file: "<<fileName<<endl;
					return false;
				}

				outputFP << file;
				outputFP.close();
				return true;
			}
		}
	}

	cerr << "Server Error";
	return false;
}


int F2DockClient::getJobStatus(string id)
{
	XmlRpcClient c(hostName, port);
	
	int res;

	if (c.execute("getJobStatus", id, result))
	{
		if(!c.isFault())
		{
			res = int(result);

			if(res==-1)
			{
				std::cout << "Error in server\n\n";
				return -1;
			}
			
				
			return res;
		}
		else
		{
			std::cout << "Error in server\n\n";
			return -1;
		}
	}
	else
	{
		std::cout << "Error calling server\n\n";
		return -1;
	}
}
