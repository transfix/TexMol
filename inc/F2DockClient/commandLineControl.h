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
#ifndef COMMAND_LINE_CONTROL
#define COMMAND_LINE_CONTROL

#include <F2DockClient/jobParams.h>
#include <F2DockClient/F2DockClient.h>
#include <ctime>

using namespace std;


class CommandLineControl
{
	private:
	JobParams* job;

	F2DockClient *client;	

	void saveResults();
	bool waitWhileRunning();

	void my_delay(clock_t n);

	public:
	CommandLineControl();
	~CommandLineControl();

	void init();
	
	// Actions corresponding to events generated from the Command line
	bool handleDockingRequest(string inputFileName);
	bool handleRerankingRequest(string inputFileName);
	bool handleF2dGenRequest(string inputFileName);
	bool handleQuadGenRequest(string inputFileName);

	bool saveAllResults(string fileName, int id);
	int getStatus(string fileName, int id);
	int handleDockingRequestNonBlocking(string fileName);

};

#endif
