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
#include <F2DockClient/f2dGenParams.h>

using namespace std;


F2dGenParams::F2dGenParams()
{
	init();
}


F2dGenParams::~F2dGenParams()
{
	
}


bool F2dGenParams::init()
{
	resolution = NORMAL_RES_F2D;
	dim = 128;
	
	return true;
}


bool F2dGenParams::load(string fileName)
{
	string f2dGenFileName(fileName);
	f2dGenFileName += ".f2dgen";

	ifstream FP(f2dGenFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.f2dGen)"<<endl;
		return false;
	}
	else
	{
		FP >> resolution;
		FP.close();
	}
	return true;
}


bool F2dGenParams::save(string fileName)
{
	string f2dGenFileName(fileName);
	f2dGenFileName += ".f2dgen";

	ofstream FP(f2dGenFileName.c_str());	

	if(!FP)
	{
		cerr << "Error saving job (.f2dgen)"<<endl;
		return false;
	}
	else
	{
		FP << resolution <<endl;
		FP.close();
	}
	return true;
}


bool F2dGenParams::prepareInputFile(string fileName)
{
	ofstream FP(fileName.c_str());	

	if(!FP)
	{
		cerr << "Error creating f2dgen input file"<<endl;
		return false;
	}
	else
	{
		FP << dim <<endl;
		FP.close();
	}
	return true;
}


bool F2dGenParams::setResolution(int resType)
{
	resolution = resType;

	if(resolution == FINE_RES_F2D)
	{
		dim = 256;
	}

	else if(resolution == NORMAL_RES_F2D)
	{
		dim = 128;
	}

	else if(resolution == COARSE_RES_F2D)
	{
		dim = 64;
	}
	
	else return false;

	return true;
}
