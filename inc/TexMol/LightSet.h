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
#ifndef CCV_TEXMOL_LIGHT_SET_H
#define CCV_TEXMOL_LIGHT_SET_H

typedef struct
{
	// enabled
	bool enabled;
	// position
	float position[4];
	// colors
	float ambient[4];
	float diffuse[4];
	float specular[4];
	// spotlight params
	float spotDirection[3];
	float spotExponent;
	float spotCutoff;
	// attenuation
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
} Light;

class LightSet
{
	public:
		LightSet();
		virtual ~LightSet();
		void init(int numberOfLights);
		Light* m_Lights;

	protected:
		void initLights(int numberOfLights);
};

#endif
