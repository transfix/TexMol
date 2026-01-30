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
#include <F2DockClient/f2dockMasterControl.h>

F2dockMasterControl::F2dockMasterControl()
{
	init();
}

F2dockMasterControl::~F2dockMasterControl()
{
	jobs.clear();
}
	
void F2dockMasterControl::init()
{
	client = new F2DockClient();
}

	
// Submissions

bool F2dockMasterControl::handleDockingRequest()
{
	JobParams *newJob = new JobParams();
	newJob->getBasicParams()->setJobType(DOCKING);
	
	DockDialog dockDial;
	DockControl dockCont;

	dockCont.setDockingParams(newJob->getDockingParams());
	dockCont.setBasicParams(newJob->getBasicParams());
	dockDial.setController(&dockCont);

	if(dockDial.exec() == QDialog::Accepted)
	{
		if(client->submitRequest(newJob))
		{
			jobs.push_back(newJob);
			return true;
		}
	}

	return false;
}

bool F2dockMasterControl::handleRerankingRequest()
{
	JobParams *newJob = new JobParams();
	newJob->getBasicParams()->setJobType(RERANKING);
	
	RerankDialog rerankDial;
	RerankControl rerankCont;

	rerankCont.setRerankingParams(newJob->getRerankingParams());
	rerankCont.setBasicParams(newJob->getBasicParams());

	rerankDial.setController(&rerankCont);

	if(rerankDial.exec() == QDialog::Accepted)
	{
		if(client->submitRequest(newJob))
		{
			jobs.push_back(newJob);
			return true;
		}
	}

	return false;
}

bool F2dockMasterControl::handleF2dGenRequest()
{
	JobParams *newJob = new JobParams();
	newJob->getBasicParams()->setJobType(F2DGEN);
	
	F2dGenDialog f2dGenDial;
	F2dGenControl f2dGenCont;

	f2dGenCont.setF2dGenParams(newJob->getF2dGenParams());
	f2dGenCont.setBasicParams(newJob->getBasicParams());

	f2dGenDial.setController(&f2dGenCont);

	if(f2dGenDial.exec() == QDialog::Accepted)
	{
		if(client->submitRequest(newJob))
		{
			jobs.push_back(newJob);
			return true;
		}
	}

	return false;
}

bool F2dockMasterControl::handleQuadGenRequest()
{
	JobParams *newJob = new JobParams();
	newJob->getBasicParams()->setJobType(QUADGEN);
	
	QuadGenDialog quadGenDial;
	QuadGenControl quadGenCont;

	quadGenCont.setQuadGenParams(newJob->getQuadGenParams());
	quadGenCont.setBasicParams(newJob->getBasicParams());

	quadGenDial.setController(&quadGenCont);

	if(quadGenDial.exec() == QDialog::Accepted)
	{
		if(client->submitRequest(newJob))
		{
			jobs.push_back(newJob);
			return true;
		}
	}

	return false;
}



// Load, Save etc.

bool F2dockMasterControl::createJob(string inputFileName, string outputFileName)
{
	JobParams* creatingJob = new JobParams();
	cout<<"Initialized job parameters"<<endl;

	creatingJob->getBasicParams()->setJobType(DOCKING);

	DockControl dockCont;
	dockCont.setDockingParams(creatingJob->getDockingParams());
	dockCont.setBasicParams(creatingJob->getBasicParams());

	if(dockCont.setParamsFromFile(inputFileName))
	{
		if(creatingJob->getBasicParams()->setDockingOutputName(outputFileName,0))
		{
			jobs.push_back(creatingJob);
			return true;
		}
	}
	
	return false;
}

bool F2dockMasterControl::saveJob(int index)
{
	if(index < jobs.size())
	{
		JobParams *savingJob = jobs[index];

		return savingJob->save(savingJob->getBasicParams()->getJobTitle());	
	}
	return false;
}


bool F2dockMasterControl::loadJob(string fileName)
{
	JobParams *loadingJob = new JobParams();

	if(loadingJob->load(fileName))
	{
		jobs.push_back(loadingJob);
		return true;
	}
	return false;
}

bool F2dockMasterControl::closeJob(int index)
{
	cout<< index<<endl;

	if(index < jobs.size())
	{
		if(saveJob(index))
		{
			jobs.erase(jobs.begin() + index);
		
			return true;
		}
	}

	return false;
}


// result display

bool F2dockMasterControl::updateJobStatus(string id, int status)
{
	for(int i=0; i<jobs.size(); i++)
	{
		if(id.compare(jobs[i]->getBasicParams()->getJobId()) == 0 )
		{
			jobs[i]->getBasicParams()->setJobStatus(status);
		}

		if(jobs[i]->getBasicParams()->doRetrieve())	
		{
			FileManager *fm = new FileManager();
			fm->setBasicParams(jobs[i]->getBasicParams());
			fm->retrieveFiles();
		}
	}

	return true;
}


bool F2dockMasterControl::showResults(int index)
{
	if(index<jobs.size())
	{
		int jobType = jobs[index]->getBasicParams()->getJobTypeInt();

			if(jobType == DOCKING)
			{
				return showDockingResult(index);
			}
			else if(jobType == RERANKING)
			{
				return showRerankingResult(index);
			}
			else if(jobType == F2DGEN)
			{
				return showF2dGenResult(index);
			}
			else if(jobType == QUADGEN)
			{
				return showQuadGenResult(index);
			}
	}
	return false;
}

bool F2dockMasterControl::showDetails(int index)
{
	return true;
}
	

// get methods to read data to show on the UI

string F2dockMasterControl::getJobId(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getJobId();
	else return NULL;
}

string F2dockMasterControl::getJobType(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getJobType();
	else return NULL;
}

string F2dockMasterControl::getJobStatus(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getJobStatus();
	else return NULL;
}

int F2dockMasterControl::getJobStatusInt(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getJobStatusInt();
	else return NULL;
}

string F2dockMasterControl::getJobTitle(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getJobTitle();
	else return NULL;

}

bool F2dockMasterControl::isRunning(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->isRunning();
	else return false;
}


string F2dockMasterControl::getReceptor(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getReceptor();
	else return NULL;

}

string F2dockMasterControl::getLigand(int index)
{
	if(index < jobs.size()) return jobs[index]->getBasicParams()->getLigand();
	else return NULL;

}


//private methods

ResultControl* F2dockMasterControl::getResultControl(int index)
{
	resultCont = new ResultControl();

	cout<<"here 2"<<endl;

	resultCont->setVisualizationParams(jobs[index]->getVisualizationParams());
	resultCont->setBasicParams(jobs[index]->getBasicParams());
	resultCont->setDockingParams(jobs[index]->getDockingParams());
	resultCont->setRerankingParams(jobs[index]->getRerankingParams());

	cout<<"here 3"<<endl;

	return resultCont;
}


bool F2dockMasterControl::showDockingResult(int index)
{
/*	resultDial = new ResultDialog();
	resultCont = new ResultControl();

	resultCont->setVisualizationParams(jobs[index]->getVisualizationParams());
	resultCont->setBasicParams(jobs[index]->getBasicParams());

	if(resultDial.setController(&resultCont))
	{
		if(resultDial.exec() == QDialog::Accepted)
			return true;
	}*/

	return false;
}



bool F2dockMasterControl::showRerankingResult(int index)
{
/*	ResultDialog resultDial;
	ResultControl resultCont;

	resultCont.setVisualizationParams(jobs[index]->getVisualizationParams());
	resultCont.setBasicParams(jobs[index]->getBasicParams());

	if(resultDial.setController(&resultCont))
	{
		if(resultDial.exec() == QDialog::Accepted)
			return true;
	}
*/
	return false;
}



bool F2dockMasterControl::showF2dGenResult(int index)
{
	return true;
}



bool F2dockMasterControl::showQuadGenResult(int index)
{
	return true;
}

