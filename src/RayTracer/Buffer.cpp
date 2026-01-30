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
#include <memory.h>
#include <RayTracer/Buffer.h>
#include <stdio.h>
#include <stdlib.h>

using namespace CCVRayTracer;

Buffer::Buffer()
{
	m_Pixels = 0;
	m_Width = 0;
	m_Height = 0;
}

Buffer::~Buffer()
{
	delete []m_Pixels;
}

void Buffer::clear(int width, int height)
{
	if (m_Width != width || m_Height != height)
	{
		m_Width = width;
		m_Height = height;
		if (m_Pixels)
		{
			delete []m_Pixels;
		}
		m_Pixels = new float[m_Width*m_Height*4];
	}
	memset(m_Pixels, '0', m_Width*m_Height*4);
	for (int i=0; i<m_Width*m_Height*4; i++)
	{
		m_Pixels[i] = (float)rand()/(RAND_MAX);
	}
}

void Buffer::addColor(int xPos, int yPos, ColorManager::Color color)
{
	int pos = yPos*m_Width + xPos;
	if (pos >= m_Width*m_Height)
	{
		return;
	}
	m_Pixels[pos*4+0] = color[0];
	m_Pixels[pos*4+1] = color[1];
	m_Pixels[pos*4+2] = color[2];
	m_Pixels[pos*4+3] = color[3];
}
