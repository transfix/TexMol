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
#include <F2DockClient/commandLineControl.h>

#include <F2DockClient/quadGenControl.h>
#include <F2DockClient/f2dGenControl.h>
#include <F2DockClient/rerankControl.h>
#include <F2DockClient/dockControl.h>

#include <F2DockClient/fileManager.h>



CommandLineControl::CommandLineControl()
{
	init();
}


CommandLineControl::~CommandLineControl()
{

}


void CommandLineControl::init()
{
	client = new F2DockClient();
	cout<<"Initialized XmlRPC connection"<<endl;
	job = new JobParams();
	cout<<"Initialized job parameters"<<endl;
}


void CommandLineControl::my_delay(clock_t n)
{
	clock_t start_time = clock();
	clock_t end_time = n * 1000 + start_time;
	while(clock() != end_time);
}


bool CommandLineControl::waitWhileRunning()
{
	int old_status = job->getBasicParams()->getJobStatusInt();
	int new_status;
	string id = job->getBasicParams()->getJobId();
	int error = 0;

	while(1)
	{
		my_delay(3000);

		cout << "Checking status of job " << id << endl;
			
		new_status = client->getJobStatus(id);

		if(new_status == -1)
		{
			error ++;

			if(error == 20)
			{	
				cout<< " Got too many connection errors. Probable server crash. contact admin." << endl;
				return false;
			}
		}
	
		else 
		{
			error = 0;

			if (new_status != old_status)
			{
				if(!job->getBasicParams()->setJobStatus(new_status))
				{
					cout << " Server Error: bad status report " << endl;					
					return false;		
				}

				cout << "Job is now in "<< job->getBasicParams()->getJobStatus() << " state" <<endl;
				old_status = new_status;

				if(!job->getBasicParams()->isRunning())		// job is not running any more
				{
					if(!job->getBasicParams()->doRetrieve())	// job terminated due to error
					{
						cout << " Server Error: bad status report " << endl;					
						return false;
					}

					else		// job done
					{
						return true;
					}
				}
			}
		}
	}
}


void CommandLineControl::saveResults()
{
	FileManager *fm = new FileManager();
	fm->setBasicParams(job->getBasicParams());

	fm->retrieveFiles();
}


bool CommandLineControl::handleDockingRequest(string fileName)
{
	job->getBasicParams()->setJobType(DOCKING);

	DockControl dockCont;

	dockCont.setDockingParams(job->getDockingParams());

	dockCont.setBasicParams(job->getBasicParams());

	if(dockCont.setParamsFromFile(fileName))
	{
		if(client->submitRequest(job))
		{
			cout<<"Job submitted. Waiting ..." <<endl;

			if(waitWhileRunning())
			{
				cout<<"Saving results ..." <<endl;

				saveResults();		

				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

int CommandLineControl::handleDockingRequestNonBlocking(string fileName)
{
	job->getBasicParams()->setJobType(DOCKING);

	DockControl dockCont;

	dockCont.setDockingParams(job->getDockingParams());

	dockCont.setBasicParams(job->getBasicParams());

	if(dockCont.setParamsFromFile(fileName))
	{
		if(client->submitRequest(job))
		{
			cout<<"Job submitted. " <<endl;

			return job->getBasicParams()->getJobIdInt();

/*			if(waitWhileRunning())
			{
				cout<<"Saving results ..." <<endl;

				saveResults();		

				return true;
			}
			else
				return false;
*/
		}
		else
		{
			cout<<"Could not submit job"<<endl;
			return -1;
		}
	}
	else
	{
		cout<<"Could not parse input file"<<endl;
	}
	return -1;
}

int CommandLineControl::getStatus(string fileName, int id)
{
	job->getBasicParams()->setJobType(DOCKING);

	DockControl dockCont;

	dockCont.setDockingParams(job->getDockingParams());

	dockCont.setBasicParams(job->getBasicParams());

	job->getBasicParams()->setJobId(id);

	if(dockCont.setParamsFromFile(fileName))
	{
		cout << "Checking status of job " << id << endl;
			
		int status = client->getJobStatus(job->getBasicParams()->getJobId());

		if(!job->getBasicParams()->setJobStatus(status))
		{
			cout << " Server Error: bad status report " << endl;					
			return -1;		
		}

		cout << "Job is now in "<< job->getBasicParams()->getJobStatus() << " state" <<endl;

		return job->getBasicParams()->getJobStatusInt();
	}

	return -1;
}

bool CommandLineControl::saveAllResults(string fileName, int id)
{
	job->getBasicParams()->setJobType(DOCKING);

	DockControl dockCont;

	dockCont.setDockingParams(job->getDockingParams());

	dockCont.setBasicParams(job->getBasicParams());

	job->getBasicParams()->setJobId(id);

	string outputName = fileName;
	outputName.erase(outputName.size() - 4);

	job->getBasicParams()->setJobTitle(outputName);

	outputName += ".out";

//	job->getBasicParams()->setDockingOutputName(outputName,0);


	if(dockCont.setParamsFromFile(fileName))
	{
		cout << "Checking status of job " << id << endl;
			
		int status = client->getJobStatus(job->getBasicParams()->getJobId());

		if(!job->getBasicParams()->setJobStatus(status))
		{
			cout << " Server Error: bad status report " << endl;					
			return false;		
		}

		cout << "Job is now in "<< job->getBasicParams()->getJobStatus() << " state" <<endl;

		if(!job->getBasicParams()->isRunning())		// job is not running any more
		{
			if(!job->getBasicParams()->doRetrieve())	// job terminated due to error
			{
				cout << " Server Error: bad status report " << endl;					
				return false;
			}
			else		// job done
			{
				saveResults();
			
				return true;
			}
		}

		return false;
	}

	return false;
}

bool CommandLineControl::handleRerankingRequest(string fileName)
{
	job->getBasicParams()->setJobType(RERANKING);
	
	RerankControl rerankCont;

	rerankCont.setRerankingParams(job->getRerankingParams());
	rerankCont.setBasicParams(job->getBasicParams());

	if(rerankCont.setParamsFromFile(fileName))
	{
		if(client->submitRequest(job))
		{
			cout<<"Job submitted. Waiting ..." <<endl;

			if(waitWhileRunning())
			{
				cout<<"Saving results ..." <<endl;

				saveResults();		

				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

bool CommandLineControl::handleF2dGenRequest(string fileName)
{
	job->getBasicParams()->setJobType(F2DGEN);

	F2dGenControl f2dGenCont;

	f2dGenCont.setF2dGenParams(job->getF2dGenParams());
	f2dGenCont.setBasicParams(job->getBasicParams());

	if(f2dGenCont.setParamsFromFile(fileName))
	{
		if(client->submitRequest(job))
		{
			cout<<"Job submitted. Waiting ..." <<endl;

			if(waitWhileRunning())
			{
				cout<<"Saving results ..." <<endl;

				saveResults();		

				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

bool CommandLineControl::handleQuadGenRequest(string fileName)
{
	job->getBasicParams()->setJobType(QUADGEN);
	
	QuadGenControl quadGenCont;

	quadGenCont.setQuadGenParams(job->getQuadGenParams());
	quadGenCont.setBasicParams(job->getBasicParams());

	if(quadGenCont.setParamsFromFile(fileName))
	{
		if(client->submitRequest(job))
		{
			cout<<"Job submitted. Waiting ..." <<endl;

			if(waitWhileRunning())
			{
				cout<<"Saving results ..." <<endl;

				saveResults();		

				return true;
			}
			else
				return false;

		}
		else
			return false;
	}
	return false;
}

