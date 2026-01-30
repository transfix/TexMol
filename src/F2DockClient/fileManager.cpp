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

#include <F2DockClient/fileManager.h>

FileManager::FileManager()
{
	client = new F2DockClient();
}

FileManager::~FileManager()
{

}


void FileManager::retrieveFiles()
{
	if(basPar->isRunning()) return;

	int jobId = basPar->getJobIdInt();
	string id = basPar->getJobId();

	string jobTitle = basPar->getJobTitle();

	string receptorNameFragment, ligandNameFragment;
	string pqr_extension ("pqr");
	string f2d_extension ("f2d");
	string rawn_extension ("rawn");
	string quad_extension ("quad");
	bool success = true;

	if(basPar->getReceptorPDBAvailable()) receptorNameFragment.assign(basPar->getReceptorPDBName(), 0, basPar->getReceptorPDBName().length()-3);
	else if(basPar->getReceptorPQRAvailable()) receptorNameFragment.assign(basPar->getReceptorPQRName(), 0, basPar->getReceptorPQRName().length()-3);
	else if(basPar->getReceptorRAWNAvailable()) receptorNameFragment.assign(basPar->getReceptorRAWNName(), 0, basPar->getReceptorRAWNName().length()-4);

	if(basPar->getLigandPDBAvailable()) ligandNameFragment.assign(basPar->getLigandPDBName(), 0, basPar->getLigandPDBName().length()-3);
	else if(basPar->getLigandPQRAvailable()) ligandNameFragment.assign(basPar->getLigandPQRName(), 0, basPar->getLigandPQRName().length()-3);
	else if(basPar->getLigandRAWNAvailable()) ligandNameFragment.assign(basPar->getLigandRAWNName(), 0, basPar->getLigandRAWNName().length()-4);


	if(basPar->getStoreReceptorPQR())
	{
		string receptorPQRName = receptorNameFragment;
		receptorPQRName += pqr_extension;

		cout<<"Saving "<< receptorPQRName << endl;

		if( client->getResult(jobId, receptorPQRName, RECEPTOR_PQR) )
		{
			basPar->setReceptorPQRAvailable(true);
			basPar->setReceptorPQRName(receptorPQRName);
		}
	}

	if(basPar->getStoreReceptorF2d())
	{
		string receptorF2dName = receptorNameFragment;
		receptorF2dName += f2d_extension;

		cout<<"Saving "<< receptorF2dName << endl;

		if( client->getResult(jobId, receptorF2dName, RECEPTOR_F2D) )
		{
			basPar->setReceptorF2dAvailable(true);
			basPar->setReceptorF2dName(receptorF2dName);
		}
	}

	if(basPar->getStoreReceptorRAWN())
	{
		string receptorRAWNName = receptorNameFragment;
		receptorRAWNName += rawn_extension;

		cout<<"Saving "<< receptorRAWNName << endl;

		if( client->getResult(jobId, receptorRAWNName, RECEPTOR_RAWN) )
		{
			basPar->setReceptorRAWNAvailable(true);
			basPar->setReceptorRAWNName(receptorRAWNName);
		}
	}

	if(basPar->getStoreReceptorQuad())
	{
		string receptorQuadName = receptorNameFragment;
		receptorQuadName += quad_extension;

		cout<<"Saving "<< receptorQuadName << endl;

		if( client->getResult(jobId, receptorQuadName, RECEPTOR_QUAD) )
		{
			basPar->setReceptorQuadAvailable(true);
			basPar->setReceptorQuadName(receptorQuadName);
		}
	}

	if(basPar->getStoreLigandPQR())
	{
		string ligandPQRName = ligandNameFragment;
		ligandPQRName += pqr_extension;

		cout<<"Saving "<< ligandPQRName << endl;

		if( client->getResult(jobId, ligandPQRName, LIGAND_PQR) )
		{
			basPar->setLigandPQRAvailable(true);
			basPar->setLigandPQRName(ligandPQRName);
		}
	}

	if(basPar->getStoreLigandF2d())
	{
		string ligandF2dName = ligandNameFragment;
		ligandF2dName += f2d_extension;

		cout<<"Saving "<< ligandF2dName << endl;

		if( client->getResult(jobId, ligandF2dName, LIGAND_F2D) )
		{
			basPar->setLigandF2dAvailable(true);
			basPar->setLigandF2dName(ligandF2dName);
		}
	}

	if(basPar->getStoreLigandRAWN())
	{
		string ligandRAWNName = ligandNameFragment;
		ligandRAWNName += rawn_extension;

		cout<<"Saving "<< ligandRAWNName << endl;

		if( client->getResult(jobId, ligandRAWNName, LIGAND_RAWN) )
		{
			basPar->setLigandRAWNAvailable(true);
			basPar->setLigandRAWNName(ligandRAWNName);
		}
	}

	if(basPar->getStoreLigandQuad())
	{
		string ligandQuadName = ligandNameFragment;
		ligandQuadName += quad_extension;

		cout<<"Saving "<< ligandQuadName << endl;

		if( client->getResult(jobId, ligandQuadName, LIGAND_QUAD) )
		{
			basPar->setLigandQuadAvailable(true);
			basPar->setLigandQuadName(ligandQuadName);
		}
	}

	if(basPar->getJobTypeInt() == DOCKING)
	{
/*
		string dockingOutputName = jobTitle;
		string dock_output_extension (".out");
		dockingOutputName += dock_output_extension;

		cout<<"Saving "<< dockingOutputName << endl;

		if( client->getResult(jobId, dockingOutputName, DOCKING_OUT) )
		{
			basPar->setDockingOutputAvailable(true);
			basPar->setDockingOutputName(dockingOutputName);
		}
*/
		if( client->getResult(jobId, basPar->getDockingOutputFileName(), DOCKING_OUT) )
		{
			basPar->setDockingOutputAvailable(true);			
		}

	}

	if(basPar->getJobTypeInt() == RERANKING || ( (basPar->getJobTypeInt() == DOCKING) && basPar->getPerformRerank() ) )
	{
		string rerank_output_extension (".rerank.out");
		string rerankingOutputName = jobTitle;
		rerankingOutputName += rerank_output_extension;

		cout<<"Saving "<< rerankingOutputName << endl;

		if( client->getResult(jobId, rerankingOutputName, RERANKING_OUT) )
		{
			basPar->setRerankingOutputAvailable(true);
			basPar->setRerankingOutputName(rerankingOutputName);
		}
	}

	basPar->setJobStatus(AVAILABLE);
}


