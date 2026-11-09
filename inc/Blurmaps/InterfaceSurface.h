/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef CCV_INTERFACE_SURFACE_H
#define CCV_INTERFACE_SURFACE_H

#include <string>
#include <Utility/utility.h>


using namespace std;

class InterfaceSurface
{
	public:
		InterfaceSurface(string SurfaceFileName1, string SurfaceFileName2, double interfaceWidth, bool coloredInput);

		virtual ~InterfaceSurface();

		bool detect(double & area1, double & area2);

	private:
		string surfaceFileName1;
		string surfaceFileName2;
		string interfaceFileName;
		string highlightedSurfaceFileName1;
		string highlightedSurfaceFileName2;
		string interIndexFileName1;
		string interIndexFileName2;
		double interfaceWidth;
		bool coloredInput;
};


#endif
