/*
  Copyright 2011 The University of Texas at Austin

	Advisor: Chandrajit Bajaj <bajaj@cs.utexas.edu>

  This file is part of MolSurf.

  MolSurf is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.


  MolSurf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef CCV_TEXMOL_GEOMETRY_TEXTURE_2D_H
#define CCV_TEXMOL_GEOMETRY_TEXTURE_2D_H

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

class Texture2D
{
	public:
		Texture2D(int width, int height, unsigned char* data, int numBytesPerEntry);
		virtual ~Texture2D();
		Texture2D(const Texture2D& copy);
		Texture2D& operator=(const Texture2D& copy);
		Texture2D& set(const Texture2D& copy);
		bool enable(bool enable);
		GLsizei m_Width;
		GLsizei m_Height;
		unsigned char* m_Data;
		GLuint m_Id;
		int m_NumBytesPerEntry;
};

#endif
