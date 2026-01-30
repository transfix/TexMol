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
#ifndef CONNPAR
#define CONNPAR

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class ConnectionParams
{
	bool isLocal;
	string hostName;
	int port;

	public:
	ConnectionParams();
	~ConnectionParams();

	void setLocal(bool l) {isLocal = l;}
	void setHostName(string s) {hostName = s;}
	void setPort(int p) {port = p;}

	bool getLocal(void) {return isLocal;}
	string getHostName(void) {return hostName;}
	int getPort(void) {return port;}

	bool init();
	
	bool load(string fileName);
	bool save(string fileName);
};



#endif
