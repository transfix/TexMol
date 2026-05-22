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
/* A fallback replacement for the CG-Impostor system currently used to draw Cylinders.
 * This class will read the CG-encoded data and interpret it to draw Mesh instances.
 * Note that per-pixel effects like those provided by the shader are not feasible with this
 * system.
 * */

#ifndef __MESHCYLINDERRENDERER_H__
#define __MESHCYLINDERRENDERER_H__

#include <GL/glew.h>
#include <GL/glu.h>

#include <stdio.h>
#include <ImposterRenderer/ExpandableBuffer.h>

class MeshCylinderRenderer
{
	public:
		/* this function uses gluCylinder to draw the actual components */
		static void DrawCylinderBuffer(const ExpandableBuffer<GLfloat>& position, const ExpandableBuffer<GLfloat>& color, const ExpandableBuffer<GLfloat>& textureCoord1, const ExpandableBuffer<GLfloat>& textureCoord2);
	private:
		static GLUquadric* m_Quadric;
		static const int LEVEL_OF_DETAIL;
};

#endif
