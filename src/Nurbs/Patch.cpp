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
#include <cstdio>
#include <Nurbs/Patch.h>
#include <Nurbs/TrimmingCurve.h>

using namespace std;
using namespace CCV_NURBS;

Patch::Patch()
{
	m_NumberOfUKnots = 0;
	m_UKnots = 0;
	m_NumberOfVKnots = 0;
	m_VKnots = 0;
	m_UStride = 0;
	m_VStride = 0;
	m_ControlArray = 0;
	m_UOrder = 0;
	m_VOrder = 0;
	m_Type = GL_MAP2_VERTEX_4;
	m_GluSamplingTolerance = 50;
	m_GluDisplayMode = GLU_FILL;
	m_GluCulling = GL_FALSE;
	m_GluAutoLoadMatrix = GL_TRUE;
	m_NurbSurface = gluNewNurbsRenderer();
}

Patch::~Patch()
{
	deleteData();
	gluDeleteNurbsRenderer(m_NurbSurface);
}

void Patch::deleteData()
{
	delete []m_UKnots;
	delete []m_VKnots;
	delete []m_ControlArray;
	for (int i=0; i<m_TrimmingCurves.size(); i++)
	{
		delete m_TrimmingCurves[i];
	}
	m_TrimmingCurves.clear();
}

Patch::Patch(const Patch& copy)
{
	set(copy);
}

Patch& Patch::operator=(const Patch& copy)
{
	return set(copy);
}

Patch& Patch::set(const Patch& copy)
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
		m_NumberOfVKnots = copy.m_NumberOfVKnots;
		if (copy.m_VKnots && copy.m_NumberOfVKnots)
		{
			m_VKnots = new float[copy.m_NumberOfVKnots];
			for (int i=0; i<copy.m_NumberOfVKnots; i++)
			{
				m_VKnots[i] = copy.m_VKnots[i];
			}
		}
		m_UStride = copy.m_UStride;
		m_VStride = copy.m_VStride;
		if (copy.m_ControlArray)
		{
			int numControlPoints = (copy.m_NumberOfUKnots-copy.m_UOrder)*(copy.m_NumberOfVKnots-copy.m_VOrder);
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
		m_VOrder = copy.m_VOrder;
		m_Type = copy.m_Type;
		m_GluSamplingTolerance = copy.m_GluSamplingTolerance;
		m_GluDisplayMode = copy.m_GluDisplayMode;
		m_GluCulling = copy.m_GluCulling;
		m_GluAutoLoadMatrix = copy.m_GluAutoLoadMatrix;
	}
	return *this;
}

bool Patch::render()
{
	{
		int c =0;
		while (glGetError()!=GL_NO_ERROR && c<10)
		{
			c++;
		}
	}
	glColor3f(0, 1, 0);
	gluNurbsProperty(m_NurbSurface, GLU_SAMPLING_TOLERANCE, m_GluSamplingTolerance);
	gluNurbsProperty(m_NurbSurface, GLU_CULLING, m_GluCulling);
	gluNurbsProperty(m_NurbSurface, GLU_AUTO_LOAD_MATRIX, m_GluAutoLoadMatrix);
	gluNurbsProperty(m_NurbSurface, GLU_DISPLAY_MODE, m_GluDisplayMode);
	gluBeginSurface(m_NurbSurface);
	gluNurbsSurface(m_NurbSurface,
					m_NumberOfUKnots, m_UKnots,
					m_NumberOfVKnots, m_VKnots,
					m_UStride,
					m_VStride,
					m_ControlArray,
					m_UOrder, m_VOrder,
					m_Type);
	for (int i=0; i<m_TrimmingCurves.size(); i++)
	{
		gluBeginTrim(m_NurbSurface);
		m_TrimmingCurves[i]->render();
		gluEndTrim(m_NurbSurface);
	}
	gluEndSurface(m_NurbSurface);
	{
		GLenum error;
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Patch rendering: GL Error: %s\n", (char*)gluErrorString(error));
		}
	}
	return true;
}
