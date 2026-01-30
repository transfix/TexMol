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
 * Note that per-pixel effects like those provided by the shader are not feasible with this system.
 */

#include <cmath>
#include <ImposterRenderer/MeshCylinderRenderer.h>
#include <UsefulMath/Vector.h>

GLUquadric* MeshCylinderRenderer::m_Quadric = 0;
const int MeshCylinderRenderer::LEVEL_OF_DETAIL = 10;

void MeshCylinderRenderer::DrawCylinderBuffer(const ExpandableBuffer<GLfloat>& position, const ExpandableBuffer<GLfloat>& color, const ExpandableBuffer<GLfloat>& textureCoord1, const ExpandableBuffer<GLfloat>& textureCoord2)
{
	// NOTE: leaked, but this is a singleton
	if (m_Quadric == 0)
	{
		m_Quadric = gluNewQuadric();
	}
	if( m_Quadric == NULL )
	{
		fprintf( stderr, "quadric allocation is failed\n");
	}
	// we extract the centers and radii from the encoding that is usually passed to the CG program
	const int numBalls = position.getNumObjects()/16;
	for (int i=0; i<numBalls; i++)
	{
		// endpoints
		const CCVOpenGLMath::Vector endpoint1(textureCoord1.get(i*16 + 0),
											  textureCoord1.get(i*16 + 1),
											  textureCoord1.get(i*16 + 2),
											  1);
		const CCVOpenGLMath::Vector endpoint2(textureCoord2.get(i*16 + 0),
											  textureCoord2.get(i*16 + 1),
											  textureCoord2.get(i*16 + 2),
											  1);
		// radius is stored as the Z of position
		const GLfloat radius = position.get(i*16 + 2);
		// extract color
		glColor3f(color.get(i*16 + 0),
				  color.get(i*16 + 1),
				  color.get(i*16 + 2));
		glPushMatrix();
		// gluCylinder draws on the z axis.
		// so we need to rotate cylinderUp to align with the z axis
		const CCVOpenGLMath::Vector cylinderUp = (endpoint2 - endpoint1);
		const GLfloat height = cylinderUp.norm();
		// convert cylinderUp to spherical coordinates.  OpenGL needs this in degrees.
		const GLfloat RAD_TO_DEG = 180.0f/M_PI;

		const GLfloat phi = acos(cylinderUp[2]/height) * RAD_TO_DEG;
		const GLfloat theta = atan2(cylinderUp[1],cylinderUp[0]) * RAD_TO_DEG;

		// prepare to draw
		// (rotate phi down, remaining aligned with x-axis, then rotate theta)
		glTranslatef(endpoint1[0], endpoint1[1], endpoint1[2]);
		glRotatef(theta, 0, 0, 1);
		glRotatef(phi, 0, 1, 0);

		gluCylinder(m_Quadric,
					radius, // GLdouble  base,
					radius, // GLdouble  top,
					height,
					LEVEL_OF_DETAIL, // GLint  slices,
					1); // GLint  stacks);
		glPopMatrix();
	}
}
