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
#include <OpenGL_Viewer/View.h>
#include <RayTracer/Buffer.h>
#include <RayTracer/Sampler.h>

using namespace CCVRayTracer;

Sampler::Sampler(View* view)
{
	m_View = view;
	m_Buffer = 0;
}

Sampler::~Sampler()
{
	delete m_Buffer;
}

void Sampler::reset()
{
	if (!m_Buffer)
	{
		m_Buffer = new Buffer();
	}
	m_Buffer->clear(m_View->m_Width, m_View->m_Height);
}

float* Sampler::getImage()
{
	if (!m_Buffer)
	{
		return 0;
	}
	return m_Buffer->m_Pixels;
}
