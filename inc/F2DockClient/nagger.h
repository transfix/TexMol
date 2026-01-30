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
#ifndef NAGGER_H
#define NAGGER_H

#include <QThread>
#include <QMutex>

#include <F2DockClient/F2DockClient.h>

#include <string>
#include <iostream>

using namespace std;

#define NAGDELAY 60

class NaggerThread : public QThread
{
        Q_OBJECT

	public:
        NaggerThread(QObject *parent = 0);
        ~NaggerThread();
	
	void addJob(string id, int status);
	void removeJob(string id);
	void clearJobs();

	signals:
        void jobStatusChanged(QString jobId, int status);

	protected:
        void run();

	private:
	struct JOBDESC
	{
		string id;
		int oldStatus;
		int newStatus;
	};

	QMutex mutex;

	F2DockClient *client;

	vector<JOBDESC> jobs;
};

#endif
