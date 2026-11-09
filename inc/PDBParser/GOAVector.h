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
#ifndef __GOAVECTOR_H__
#define __GOAVECTOR_H__

#include <Utility/utility.h>
#include <PDBParser/Atom.h>

namespace PDBParser
{
	class Atom;
	// Implements 3-D vector ops
	class GOAVector
	{
		public:
			GOAVector();
			GOAVector(double ax, double ay, double az);
			GOAVector(Atom* atom);
			~GOAVector();

			void print();
			void norm();

			double x, y, z;
	};
	GOAVector* subtractAtoms(Atom* atomA, Atom* atomB);
	GOAVector* scalarProduct(double c, GOAVector* v1);
	GOAVector* crossProduct(GOAVector* v1, GOAVector* v2);
	double dotProduct(GOAVector* v1, GOAVector* v2);
	double dotProductAngle(GOAVector* v1, GOAVector* v2);
};

#endif
