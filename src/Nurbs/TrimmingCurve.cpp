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
#include <Nurbs/TrimmingCurve.h>

using namespace CCV_NURBS;

TrimmingCurve::TrimmingCurve()
{
	m_NumberOfUKnots = 0;
	m_UKnots = 0;
	m_UStride = 0;
	m_ControlArray = 0;
	m_UOrder = 0;
	m_Type = GLU_MAP1_TRIM_3;
	m_NurbCurve = gluNewNurbsRenderer();
}

TrimmingCurve::~TrimmingCurve()
{
	deleteData();
	gluDeleteNurbsRenderer(m_NurbCurve);
}

void TrimmingCurve::deleteData()
{
	delete []m_UKnots;
	delete []m_ControlArray;
}

TrimmingCurve::TrimmingCurve(const TrimmingCurve& copy)
{
	set(copy);
}

TrimmingCurve& TrimmingCurve::operator=(const TrimmingCurve& copy)
{
	return set(copy);
}

TrimmingCurve& TrimmingCurve::set(const TrimmingCurve& copy)
{
	if (this!=&copy)
	{
		deleteData();
		m_NumberOfUKnots = copy.m_NumberOfUKnots;
		if (copy.m_UKnots && copy.m_NumberOfUKnots)
		{
			m_UKnots = new float[copy.m_NumberOfUKnots];
			for (int i=0; i<copy.m_NumberOfUKnots; i++)
			{
				m_UKnots[i] = copy.m_UKnots[i];
			}
		}
		m_UStride = copy.m_UStride;
		if (copy.m_ControlArray)
		{
			int numControlPoints = (copy.m_NumberOfUKnots-copy.m_UOrder);
			if (numControlPoints > 0)
			{
				m_ControlArray = new float[numControlPoints*4];
				for (int i=0; i<numControlPoints*4; i++)   // point  +  weight : homogenous coordinate
				{
					m_ControlArray[i] = copy.m_ControlArray[i];
				}
			}
		}
		m_UOrder = copy.m_UOrder;
		m_Type = copy.m_Type;
	}
	return *this;
}

bool TrimmingCurve::render()
{
	return true;
}
