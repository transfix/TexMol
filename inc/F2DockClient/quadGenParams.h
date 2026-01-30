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
#ifndef QUADPAR
#define QUADPAR

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>

using namespace std;

class QuadGenParams
{
	private:
	double dim;

	int resolution;

	public:
	enum {FINE_RES_QUAD, NORMAL_RES_QUAD, COARSE_RES_QUAD};

	QuadGenParams();
	~QuadGenParams();
	
	bool init();
	bool save(string fileName);
	bool load(string fileName);
	bool prepareInputFile(string fileName);
	
	bool setResolution(int resType);
	int getResolution(void) {return resolution;}
	double getDimention(void) {return dim;}
};


#endif
