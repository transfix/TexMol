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
/* A fallback replacement for the CG-Impostor system currently used to draw Spheres.
 * This class will read the CG-encoded data and interpret it to draw Mesh instances.
 * Note that per-pixel effects like those provided by the shader are not feasible with this system.
 */

#include <ImposterRenderer/MeshSphereRenderer.h>
#include <UsefulMath/Vector.h>

GLUquadric* MeshSphereRenderer::m_Quadric = 0;
const int MeshSphereRenderer::MAX_LEVEL_OF_DETAIL = 20;
const int MeshSphereRenderer::MIN_LEVEL_OF_DETAIL = 1;

void MeshSphereRenderer::DrawSphereBuffer(const ExpandableBuffer<GLfloat>& position, const ExpandableBuffer<GLfloat>& color)
{
	// NOTE: leaked, but this is a singleton
	if (m_Quadric == 0)
	{
		m_Quadric = gluNewQuadric();
	}
	// we extract the centers and radii from the encoding that is usually passed to the CG program
	const int numBalls = position.getNumObjects()/16;
	for (int i=0; i<numBalls; i++)
	{
		// sphere center
		const CCVOpenGLMath::Vector center(position.get(i*16 + 0),
										   position.get(i*16 + 1),
										   position.get(i*16 + 2),
										   1);
		// radius is stored as the Alpha of the color
		const GLfloat radius = color.get(i*16 + 3);
		// extract color
		glColor3f(color.get(i*16 + 0),
				  color.get(i*16 + 1),
				  color.get(i*16 + 2));
		// determine LOD (visible pixels)
		glPushMatrix();
		glTranslatef(center[0], center[1], center[2]);
		gluSphere(m_Quadric, radius, MAX_LEVEL_OF_DETAIL, MAX_LEVEL_OF_DETAIL);
		glPopMatrix();
	}
}
