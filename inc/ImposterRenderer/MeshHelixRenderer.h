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
/* A fallback replacement for the CG-Impostor system currently used to draw Helices.
 * This class will read the CG-encoded data and interpret it to draw Mesh instances.
 * Note that per-pixel effects like those provided by the shader are not feasible with this
 * system.
 * */

#ifndef __MESHHELIXRENDERER_H__
#define __MESHHELIXRENDERER_H__

#ifdef _WIN32
#  include <TexMol/compat.h>
#endif
#include <GL/glew.h>
#if defined(__APPLE__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

#include <stdio.h>
#include <ImposterRenderer/ExpandableBuffer.h>

class MeshHelixRenderer
{
	public:
		/* this function uses gluHelix to draw the actual components */
		static void DrawHelixBuffer(const ExpandableBuffer<GLfloat>& position, const ExpandableBuffer<GLfloat>& color, const ExpandableBuffer<GLfloat>& textureCoord1, const ExpandableBuffer<GLfloat>& textureCoord2);
	private:
		static GLUquadric* m_Quadric;
		static const int LEVEL_OF_DETAIL;
		static const GLfloat HEIGHT_STEP;
		static const GLfloat STRIP_HEIGHT;
		static const GLfloat HEIGHT_TO_COILS;
};

#endif
