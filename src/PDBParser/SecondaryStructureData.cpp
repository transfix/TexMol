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
#include <PDBParser/SecondaryStructureData.h>

using PDBParser::SecondaryStructureData;

SecondaryStructureData::SecondaryStructureData()
{
}

SecondaryStructureData::SecondaryStructureData(const SecondaryStructureData& secondaryStructureData)
{
	m_ResSequenceNumberPairList = secondaryStructureData.m_ResSequenceNumberPairList;
	// The geometry is defined as a 7 tuple, containing
	// x1, y1, z1, x2, y2, z2, radius
	m_StrandGeometry = secondaryStructureData.m_StrandGeometry;
	// end for sheets
	// Used for Helices
	m_HelixGeometry = secondaryStructureData.m_HelixGeometry;
}

SecondaryStructureData::~SecondaryStructureData()
{
}
