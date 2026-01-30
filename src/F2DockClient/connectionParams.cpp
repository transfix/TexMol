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
#include <F2DockClient/connectionParams.h>

using namespace std;



ConnectionParams::ConnectionParams()
{
	init();
}


ConnectionParams::~ConnectionParams()
{

}


bool ConnectionParams::init()
{
	ifstream hostFile("host.ini");

	string local("prism2.ices.utexas.edu");

	if(!hostFile)
	{
		isLocal = false;
		hostName = local;
		port = 8091;
	}
	else
	{
		isLocal = false;
		port = 8091;

		hostFile >> hostName;

		if(hostName == local) 
			isLocal = true;
		else
			hostFile >> port;	
	}

	return true;
}
	

bool ConnectionParams::load(string fileName)
{
	string connFileName(fileName);
	connFileName += ".conn";

	ifstream FP(connFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.conn)"<<endl;
		return false;
	}
	else
	{
		FP >> isLocal;
		FP >> hostName;
		FP >> port;
		FP.close();
	}
	return true;
}


bool ConnectionParams::save(string fileName)
{
	string connFileName(fileName);
	connFileName += ".conn";

	ofstream FP(connFileName.c_str());	

	if(!FP)
	{
		cerr << "Error saving job (.conn)"<<endl;
		return false;
	}
	else
	{
		FP << isLocal << endl;
		FP << hostName << endl;
		FP << port << endl;
		FP.close();
	}
	return true;
}
