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
#ifndef CCV_CORRELATION_H
#define CCV_CORRELATION_H

#include <vector>
#include <UsefulMath/Matrix.h>

class SimpleVolumeData;
namespace PDBParser
{
	class GroupOfAtoms;
};

class Correlation
{
	public:
		Correlation(SimpleVolumeData* sData, PDBParser::GroupOfAtoms* molecule);
		virtual ~Correlation();
		bool getCriticalPoints(SimpleVolumeData* sData);
		double correlate(CCVOpenGLMath::Matrix transformation);

	protected:
		SimpleVolumeData* m_SimpleVolumeData;
		PDBParser::GroupOfAtoms* m_Molecule;
		std::vector<double>* m_CriticalPoints;
		// need these to scale the maps to be the same range to compare
		double m_MinValue1;
		double m_MinValue2;
		double m_MaxValue1;
		double m_MaxValue2;
};

#endif
