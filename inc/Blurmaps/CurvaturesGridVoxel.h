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
// CurvaturesGridVoxel.h: interface for the CurvaturesGridVoxel class.

#ifndef CCV_CURVATURESGRIDVOXEL_H
#define CCV_CURVATURESGRIDVOXEL_H

#include <Utility/utility.h>
#include <UsefulMath/Tuple.h>

using CCVOpenGLMath::Tuple;

class CurvaturesGridVoxel
{
	public:
		CurvaturesGridVoxel();
		virtual ~CurvaturesGridVoxel();
		void addAtom(Tuple const atom);

		int getNumberOfAtoms();
		Tuple getAtom(int index);

	protected:
		std::vector<Tuple> m_Atoms;
};

#endif
