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

#include <F2DockClient/visualizationParams.h>

using namespace std;


VisualizationParams::VisualizationParams()
{
	init();
}


VisualizationParams::~VisualizationParams()
{

}


bool VisualizationParams::init(void)
{
	receptorVisType = UOFB_SURF_VIS;
	ligandVisType = UOFB_SURF_VIS;

	highlightInterface = false;
	multipleLigand = false;
	return true;
}


bool VisualizationParams::load(string fileName)
{
	string visFileName(fileName);
	visFileName += ".vis";

	ifstream FP(visFileName.c_str());	

	if(!FP)
	{
		cerr << "Error loading job (.vis)"<<endl;
		return false;
	}
	else
	{
		FP >> receptorVisType;
		FP >> ligandVisType;
		FP >> highlightInterface;
		FP >> multipleLigand;
		FP.close();
	}
	return true;
}


bool VisualizationParams::save(string fileName)
{
	string visFileName(fileName);
	visFileName += ".vis";

	ofstream FP(visFileName.c_str());	

	if(!FP)
	{
		cerr << "Error saving job (.vis)"<<endl;
		return false;
	}
	else
	{
		FP << receptorVisType << endl;
		FP << ligandVisType << endl;
		FP << highlightInterface << endl;
		FP << multipleLigand << endl;
		FP.close();
	}
	return true;
}

