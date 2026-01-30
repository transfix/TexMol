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
#include <math.h>
#include <OpenGL_Viewer/View.h>
#include <RayTracer/Buffer.h>
#include <RayTracer/SimpleSampler.h>
#include <stdio.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Ray.h>

using namespace CCVRayTracer;
using namespace CCVOpenGLMath;

SimpleSampler::SimpleSampler(View* view)
	: Sampler(view)
{
	reset();
}

SimpleSampler::~SimpleSampler()
{
}

void SimpleSampler::reset()
{
	Sampler::reset();
	m_XPos = m_YPos = 0;
	m_View->updateCamera();
}

CCVOpenGLMath::Ray* SimpleSampler::nextRay()
{
	if (m_YPos > m_Buffer->m_Height-1)
	{
		return 0;
	}
	return m_View->getPrimaryRay(m_XPos, m_YPos);
}

// for simplesampler, this is a good place to increment to next ray pos.
// we should be careful for more complicated sampling.
void SimpleSampler::addSample(ColorManager::Color color)
{
	m_Buffer->addColor(m_XPos, m_YPos, color);
	if (m_XPos == m_Buffer->m_Width-1)
	{
		m_YPos++;
		m_XPos = 0;
	}
	else
	{
		m_XPos++;
	}
}
