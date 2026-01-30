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
#include <TexMol/LightSet.h>

LightSet::LightSet()
{
}

LightSet::~LightSet()
{
	delete []m_Lights;
	m_Lights = 0;
}

void LightSet::init(int numberOfLights)
{
	m_Lights = new Light[numberOfLights];
	initLights(numberOfLights);
}

void LightSet::initLights(int numberOfLights)
{
	if (!m_Lights)
	{
		return;
	}
	for (int i=0; i<numberOfLights; i++)
	{
		// enabled
		m_Lights[i].enabled = false;
		// positions
		m_Lights[i].position[0] = m_Lights[i].position[1] = m_Lights[i].position[2] = m_Lights[i].position[3] = 0;
		// colors
		m_Lights[i].ambient[0] = m_Lights[i].ambient[1] = m_Lights[i].ambient[2] = 0;
		m_Lights[i].ambient[3] = 1;
		m_Lights[i].diffuse[0] = m_Lights[i].diffuse[1] = m_Lights[i].diffuse[2] = m_Lights[i].diffuse[3] = 1;
		m_Lights[i].specular[0] = m_Lights[i].specular[1] = m_Lights[i].specular[2] = m_Lights[i].specular[3] = 1;
		// spotlight params
		m_Lights[i].spotDirection[0] = 0;
		m_Lights[i].spotDirection[1] = 0;
		m_Lights[i].spotDirection[2] = -1;
		m_Lights[i].spotExponent = 0;
		m_Lights[i].spotCutoff = 180;
		// attenuation
		m_Lights[i].constantAttenuation = 1;
		m_Lights[i].linearAttenuation = 0;
		m_Lights[i].quadraticAttenuation = 0;
	}
}
