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
#ifndef CCV_TEXMOL_PATCH_COMPLEX_H
#define CCV_TEXMOL_PATCH_COMPLEX_H

#include <vector>

using namespace std;

namespace PDBParser
{
	class GroupOfAtoms;
}

namespace SurfacePatchComplex
{
	class ConvexPatch;
	class ConcavePatch;
	class ToroidalPatch;

	class PatchComplex
	{
		public:
			PatchComplex();
			PatchComplex(PDBParser::GroupOfAtoms* groupOfAtoms);
			virtual ~PatchComplex();
			virtual void deleteData();
			virtual void setMolecule(PDBParser::GroupOfAtoms* groupOfAtoms);
			virtual bool createComplex();
			virtual int getNumberOfConvexPatches();
			virtual int getNumberOfConcavePatches();
			virtual int getNumberOfToroidalPatches();
			std::vector<ConvexPatch*> m_ConvexPatches;
			std::vector<ConcavePatch*> m_ConcavePatches;
			std::vector<ToroidalPatch*> m_ToroidalPatches;

		protected:
			PDBParser::GroupOfAtoms* m_Molecule;
			bool m_Initialized;
	};
};

#endif
