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
#include <F2DockClient/quadGenParams.h>

using namespace std;

QuadGenParams::QuadGenParams()
{
	init();
}


QuadGenParams::~QuadGenParams()
{
	
}


bool QuadGenParams::init()
{
	resolution = NORMAL_RES_QUAD;
	dim = 128;

	return true;
}


bool QuadGenParams::load(string fileName)
{
	string quadGenFileName(fileName);
	quadGenFileName += ".quadgen";

	ifstream FP(quadGenFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.quadGen)"<<endl;
		return false;
	}
	else
	{
		FP >> resolution;
		FP.close();
	}
	return true;
}


bool QuadGenParams::save(string fileName)
{
	string quadGenFileName(fileName);
	quadGenFileName += ".quadgen";

	ofstream FP(quadGenFileName.c_str());	

	if(!FP)
	{
		cerr << "Error saving job (.quadgen)"<<endl;
		return false;
	}
	else
	{
		FP << resolution <<endl;
		FP.close();
	}
	return true;
}


bool QuadGenParams::prepareInputFile(string fileName)
{
	ofstream FP(fileName.c_str());	

	if(!FP)
	{
		cerr << "Error creating quadgen input file"<<endl;
		return false;
	}
	else
	{
		FP << dim <<endl;
		FP.close();
	}
	return true;
}


bool QuadGenParams::setResolution(int resType)
{
	resolution = resType;

	if(resolution == FINE_RES_QUAD)
	{
		dim = 256;
	}

	else if(resolution == NORMAL_RES_QUAD)
	{
		dim = 128;
	}

	else if(resolution == COARSE_RES_QUAD)
	{
		dim = 64;
	}

	else return false;

	return true;
}

