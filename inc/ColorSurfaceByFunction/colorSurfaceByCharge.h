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
#ifndef CCV_COLOR_SURFACE_CHARGE_H
#define CCV_COLOR_SURFACE_CHARGE_H

#include <string>
#include <Utility/utility.h>
#include <Blurmaps/BlurMapsDataManager.h>
#include <Blurmaps/GOABlur.h>
#include <PDBParser/Atom.h>
#include <PDBParser/CollectionData.h>
#include <PDBParser/ElementInformation.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/ResidueInformation.h>



using namespace std;

class ColorSurfaceByCharge
{
	public:
		ColorSurfaceByCharge(string pqrFileName, string SurfaceFileName, string coloredSurfaceFileName, int mode, double blobbyness, double distanceCutoff);
		virtual ~ColorSurfaceByCharge();

		bool colorify();

	private:
		string pqrFileName;
		string surfaceFileName;
		string coloredSurfaceFileName;
		int mode;			// 0 = Gaussian, 1 = average	
		double distanceCutoff;	
		double blobbyness;
};


#endif
