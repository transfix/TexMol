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
#ifndef F2DOCKCLIENT
#define F2DOCKCLIENT

#include <XmlRPC/XmlRpc.h>
#include <F2DockClient/jobParams.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace XmlRpc;
using namespace std;

class F2DockClient
{
	private:
		XmlRpcValue args, id, result;
		int port;
		char hostName[100];

	public:
		F2DockClient();
		~F2DockClient();

		bool submitRequest(JobParams *jobPar);
		bool getResult(int id, string fileName, int index);
		int getJobStatus(string id);
};

#endif
