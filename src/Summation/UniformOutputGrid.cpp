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
#include <Summation/UniformOutputGrid.h>

using namespace CCVSummationModule;

UniformOutputGrid::UniformOutputGrid(float* output, float* origin, float* span, unsigned int* dimensions)
{
	m_Output = output;
	m_Origin = origin;
	m_Span = span;
	m_Dimensions = dimensions;
}

UniformOutputGrid::~UniformOutputGrid()
{
}

bool UniformOutputGrid::initialized()
{
	if(!m_Output || !m_Origin || !m_Span || !m_Dimensions)
	{
		return false;
	}
	return true;
}
