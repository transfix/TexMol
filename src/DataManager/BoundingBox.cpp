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
#include <DataManager/BoundingBox.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif
#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

BoundingBox::BoundingBox()
{
	m_Initialized = false;
	m_Min[0] = 0;
	m_Min[1] = 0;
	m_Min[2] = 0;
	m_Max[0] = 1;
	m_Max[1] = 1;
	m_Max[2] = 1;
}

BoundingBox::~BoundingBox()
{
}


bool BoundingBox::render()
{
	if (!m_Initialized)
	{
		return false;
	}
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	float backgroundColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, backgroundColor);
	glColor3f(1.0f-backgroundColor[0], 1.0f-backgroundColor[1], 1.0f-backgroundColor[2]);
	glBegin(GL_LINES);
	// draw the 4 vertical lines, y being up down
	glVertex3f(m_Min[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Max[2]);
	// draw 4 lines going across, x being left right
	glVertex3f(m_Min[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Max[2]);
	// draw 4 lines going into screen, z being in out
	glVertex3f(m_Min[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Min[1], m_Max[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Min[0], m_Max[1], m_Max[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Min[2]);
	glVertex3f(m_Max[0], m_Max[1], m_Max[2]);
	glEnd();
	glPopAttrib();
	return true;
}

void BoundingBox::reset()
{
	m_Initialized = false;
	m_Min[0] = 0;
	m_Min[1] = 0;
	m_Min[2] = 0;
	m_Max[0] = 1;
	m_Max[1] = 1;
	m_Max[2] = 1;
}

bool BoundingBox::setMinMax(float* min, float* max)
{
	if (min[0] >= max[0] || min[1] > max[1] || min[2] > max[2])
	{
		return false;
	}
	m_Min[0] = min[0];
	m_Min[1] = min[1];
	m_Min[2] = min[2];
	m_Max[0] = max[0];
	m_Max[1] = max[1];
	m_Max[2] = max[2];
	m_Initialized = true;
	return true;
}

bool BoundingBox::expandMinMax(float* min, float* max)
{
	if (!m_Initialized)
	{
		m_Min[0] = min[0];
		m_Min[1] = min[1];
		m_Min[2] = min[2];
		m_Max[0] = max[0];
		m_Max[1] = max[1];
		m_Max[2] = max[2];
		m_Initialized = true;
	}
	else
	{
		if (min[0] < m_Min[0])
		{
			m_Min[0] = min[0];
		}
		if (min[1] < m_Min[1])
		{
			m_Min[1] = min[1];
		}
		if (min[2] < m_Min[2])
		{
			m_Min[2] = min[2];
		}
		if (max[0] < m_Max[0])
		{
			m_Max[0] = max[0];
		}
		if (max[1] < m_Max[1])
		{
			m_Max[1] = max[1];
		}
		if (max[2] < m_Max[2])
		{
			m_Max[2] = max[2];
		}
	}
	return true;
}

bool BoundingBox::getMinMax(float* min, float* max) const
{
	if (!m_Initialized)
	{
		return false;
	}
	min[0] = m_Min[0];
	min[1] = m_Min[1];
	min[2] = m_Min[2];
	max[0] = m_Max[0];
	max[1] = m_Max[1];
	max[2] = m_Max[2];
	return true;
}
