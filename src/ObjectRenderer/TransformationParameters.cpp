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
#include <ObjectRenderer/TransformationParameters.h>

using namespace ObjectRendererLibrary;

TransformationParameters::TransformationParameters(double* rotations,double* translations,int numberOfTransformations,bool randomizeColors, float averageRadius)
{
	m_Rotations = rotations;
	m_Translations = translations;
	m_NumberOfTransformations = numberOfTransformations;
	m_RandomizeColors = randomizeColors;
	m_AverageRadius = averageRadius;
}

TransformationParameters::~TransformationParameters()
{
}

void TransformationParameters::setAverageRadius(float averageRadius)
{
	m_AverageRadius = averageRadius;
}

float TransformationParameters::getAverageRadius()
{
	return m_AverageRadius;
}

bool TransformationParameters::hasTransformations()
{
	if (m_Rotations && m_Translations && (m_NumberOfTransformations>0))
	{
		return true;
	}
	return false;
}

int TransformationParameters::getNumberOfTransformations()
{
	return m_NumberOfTransformations;
}

bool TransformationParameters::getTransformations(float* trans, int transformationIndex)
{
	if (!trans || !m_Rotations || !m_Translations)
	{
		return false;
	}
	if (transformationIndex>= m_NumberOfTransformations)
	{
		return false;
	}
	trans[0]  = (float)m_Rotations[transformationIndex*9+0];
	trans[1]  = (float)m_Rotations[transformationIndex*9+3];
	trans[2]  = (float)m_Rotations[transformationIndex*9+6];
	trans[3]  = (float)0;
	trans[4]  = (float)m_Rotations[transformationIndex*9+1];
	trans[5]  = (float)m_Rotations[transformationIndex*9+4];
	trans[6]  = (float)m_Rotations[transformationIndex*9+7];
	trans[7]  = (float)0;
	trans[8]  = (float)m_Rotations[transformationIndex*9+2];
	trans[9]  = (float)m_Rotations[transformationIndex*9+5];
	trans[10] = (float)m_Rotations[transformationIndex*9+8];
	trans[11] = (float)0;
	trans[12] = (float)m_Translations[transformationIndex*3+0];
	trans[13] = (float)m_Translations[transformationIndex*3+1];
	trans[14] = (float)m_Translations[transformationIndex*3+2];
	trans[15] = (float)1;
	return true;
}

bool TransformationParameters::useRandomColors()
{
	return m_RandomizeColors;
}
