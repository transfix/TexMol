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
#ifndef GEOMETRYPARSERCN_H
#define GEOMETRYPARSERCN_H

#include <Utility/utility.h>
#include <ComputeNormals/geometry.h>
#include <ComputeNormals/Surface.h>

using namespace COMPUTENORMALS;

namespace COMPUTENORMALS
{
	class Vertex;
	class Face;
	class Surface;
	class GeometryParser
	{
		public:
			GeometryParser();
			Surface* SurfaceTriangulation(FILE* fp);
			int ReadHowmanyComponents(FILE* fp);
		private:
			int howmany, normalFlag, colorFlag;
	};
}

#endif
