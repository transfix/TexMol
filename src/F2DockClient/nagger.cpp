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

#include <F2DockClient/nagger.h>
#include <TexMol/compat.h>

NaggerThread::NaggerThread(QObject* par)  : QThread(par)
{
	client = new F2DockClient();
	cout<<"Started nagger"<<endl;
}

NaggerThread::~NaggerThread()
{
	clearJobs();
}

void NaggerThread::addJob(string id, int status)
{
	mutex.lock();
	JOBDESC newjob;
	newjob.id = id;
	newjob.oldStatus = status;
	jobs.push_back(newjob);
	mutex.unlock();
	
	if(!isRunning()) start();
}

void NaggerThread::clearJobs()
{
	mutex.lock();
	jobs.clear();
	mutex.unlock();
}

void NaggerThread::removeJob(string id)
{
	mutex.lock();
	
	for(int i=0;i<jobs.size();i++)
	{
		if (id.compare(jobs[i].id) == 0)
			jobs.erase(jobs.begin()+i);
	}	

	mutex.unlock();
}

void NaggerThread::run()
{
	while (1)
	{
		sleep(NAGDELAY);

		mutex.lock();
		if(jobs.size()>0)
		{
			for(int i=0;i<jobs.size();i++)
			{
				cout<<"Nagging for job "<< jobs[i].id <<endl;
			
				jobs[i].newStatus = client->getJobStatus(jobs[i].id);
	
				if (jobs[i].newStatus != jobs[i].oldStatus)
				{
					jobs[i].oldStatus = jobs[i].newStatus;

					emit jobStatusChanged(QString(jobs[i].id.c_str()), jobs[i].newStatus);
				}
			}
		}
		mutex.unlock();
	}
}

