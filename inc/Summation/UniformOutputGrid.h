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
#ifndef CCV_UNIFORM_OUTPUT_GRID_H
#define CCV_UNIFORM_OUTPUT_GRID_H

#include <Utility/utility.h>
#include <Summation/OutputGrid.h>

namespace CCVSummationModule
{
	class UniformOutputGrid
	 : public OutputGrid
	{
		public:
			UniformOutputGrid(float* output, float* origin, float* span, unsigned int* dimensions);
			virtual ~UniformOutputGrid();
			virtual bool initialized();
			float* m_Output;
			float* m_Origin;
			float* m_Span;
			unsigned int* m_Dimensions;
	};
};

#endif
