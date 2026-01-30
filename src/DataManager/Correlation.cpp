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
#include <Blurmaps/BlurMapsDataManager.h>
#include <DataManager/Correlation.h>
#include <DataManager/CriticalPointExtractor.h>
#include <math.h>
#include <PDBParser/GroupOfAtoms.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <stdio.h>

Correlation::Correlation(SimpleVolumeData* sData, PDBParser::GroupOfAtoms* molecule)
{
	m_SimpleVolumeData = sData;
	m_Molecule = molecule;
	m_CriticalPoints = 0;
	m_MinValue1 = 0;
	m_MinValue2 = 0;
	m_MaxValue1 = 0;
	m_MaxValue2 = 0;
}

Correlation::~Correlation()
{
	delete m_CriticalPoints;
}

bool Correlation::getCriticalPoints(SimpleVolumeData* sData)
{
	if (!sData)
	{
		return false;
	}
	if (sData->getNumberOfVariables() < 1)
	{
		return false;
	}
	float* data = 0;
	if (sData->getNumberOfVariables() < 4 && sData->getType(0) == SimpleVolumeData::FLOAT)
	{
		data = (float*)(sData->getData(0));
	}
	if (sData->getNumberOfVariables() >= 4 && sData->getType(3) == SimpleVolumeData::FLOAT)
	{
		data = (float*)(sData->getData(3));
	}
	if (sData == 0)
	{
		return false;
	}
	if (m_CriticalPoints)
	{
		m_CriticalPoints->clear();
		delete m_CriticalPoints;
		m_CriticalPoints = 0;
	}
	m_CriticalPoints = new std::vector<double>;
	double cutoff = 10;
	if (!CriticalPointExtractor::findCriticalPoints(sData->getWidth(), sData->getHeight(), sData->getDepth(),
			data, m_CriticalPoints, cutoff))
	{
		return false;
	}
	// translate according to span and origin.
	// multiply by span, add origin
	int numCriticalPoints = m_CriticalPoints->size();
	if (numCriticalPoints/4 == 0)
	{
		return false;
	}
	{
		for (int i=0; i<numCriticalPoints/4; i++)
		{
			m_CriticalPoints->at(4*i+0) = m_CriticalPoints->at(4*i+0)*sData->getSpanX() + sData->getMinX();
			m_CriticalPoints->at(4*i+1) = m_CriticalPoints->at(4*i+1)*sData->getSpanY() + sData->getMinY();
			m_CriticalPoints->at(4*i+2) = m_CriticalPoints->at(4*i+2)*sData->getSpanZ() + sData->getMinZ();
		}
	}
	return true;
}

double Correlation::correlate(CCVOpenGLMath::Matrix transformation)
{
	if (!m_SimpleVolumeData || !m_Molecule)
	{
		return 0;
	}
	double mMin[3], mMax[3];
	{
		m_Molecule->getMinMax(&(mMin[0]), &(mMin[1]), &(mMin[2]), &(mMax[0]), &(mMax[1]), &(mMax[2]));
		{
			CCVOpenGLMath::Vector transformedPoint = transformation*CCVOpenGLMath::Vector(mMin[0],mMin[1],mMin[2],1);
			mMin[0] = transformedPoint[0];
			mMin[1] = transformedPoint[1];
			mMin[2] = transformedPoint[2];
		}
		{
			CCVOpenGLMath::Vector transformedPoint = transformation*CCVOpenGLMath::Vector(mMax[0],mMax[1],mMax[2],1);
			mMax[0] = transformedPoint[0];
			mMax[1] = transformedPoint[1];
			mMax[2] = transformedPoint[2];
		}
	}
	float sMin[3], sMax[3];
	sMin[0] = m_SimpleVolumeData->getMinX();
	sMin[1] = m_SimpleVolumeData->getMinY();
	sMin[2] = m_SimpleVolumeData->getMinZ();
	sMax[0] = m_SimpleVolumeData->getMaxX();
	sMax[1] = m_SimpleVolumeData->getMaxY();
	sMax[2] = m_SimpleVolumeData->getMaxZ();
	if (sMin[0] >= mMax[0] ||
			sMin[1] >= mMax[1] ||
			sMin[2] >= mMax[2] ||
			sMax[0] <= mMin[0] ||
			sMax[1] <= mMin[1] ||
			sMax[2] <= mMin[2])
	{
		return 0;
	}
	if (!m_CriticalPoints)
	{
		if (!m_SimpleVolumeData->getMinMax(0, &m_MinValue1, &m_MaxValue1))
		{
			return 0;
		}
		if (m_MaxValue1 - m_MinValue1 < 0.0001)
		{
			return 0;
		}
		SimpleVolumeData* sData = 0;
		// Create the volume
		{
			float blobbiness = -2.3f;
			int level = PDBParser::ATOM_TYPE ;
			int dim1 = 128, dim2 = 128, dim3 = 128;
			PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType = PDBParser::GroupOfAtoms::VDW_RADIUS;
			sData = BlurMapsDataManager::getVolume(m_Molecule, "", dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY, false, blobbiness, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType, level);
			if (!sData)
			{
				return 0;
			}
		}
		// Done creating volume?
		if (!sData->getMinMax(0, &m_MinValue2, &m_MaxValue2))
		{
			return 0;
		}
		if (m_MaxValue2 - m_MinValue2 < 0.0001)
		{
			return 0;
		}
		if (!getCriticalPoints(sData))
		{
			return 0;
		}
		delete sData;
		sData = 0;
	}
	// Perform correlation. Get value of vol at same position and compare.
	{
		int numCriticalPoints = m_CriticalPoints->size();
		double sumOfDiffs = 0;
		double sumOfMaxs = 0;
		if (numCriticalPoints/4 == 0)
		{
			return false;
		}
		{
			for (int i=0; i<numCriticalPoints/4; i++)
			{
				CCVOpenGLMath::Vector transformedPoint = transformation*CCVOpenGLMath::Vector(m_CriticalPoints->at(4*i+0),
						m_CriticalPoints->at(4*i+1),
						m_CriticalPoints->at(4*i+2),
						1);
				double xNew = transformedPoint[0];
				double yNew = transformedPoint[1];
				double zNew = transformedPoint[2];
				double val1 = m_SimpleVolumeData->getValueAt(0,
							  xNew,
							  yNew,
							  zNew);
				{
					if (m_MinValue1 < 0)
					{
						m_MinValue1 = 0;
					}
					if (m_MaxValue1 <= 0)
					{
						val1 = 0;
					}
					else if (val1 < 0)
					{
						val1 = 0;
					}
					else
					{
						val1 = (val1 - m_MinValue1) / (m_MaxValue1 - m_MinValue1) * 255.0;
					}
				}
				double val2 = (m_CriticalPoints->at(4*i+3) - m_MinValue2) / (m_MaxValue2 - m_MinValue2) * 255.0;
				sumOfDiffs += fabs(val1-val2);
				sumOfMaxs += (val1>val2)? val1:val2;
			}
		}
		return 1-sumOfDiffs/sumOfMaxs;
	}
	return 0;
}
