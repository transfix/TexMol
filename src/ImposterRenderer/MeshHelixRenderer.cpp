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
 * Note that per-pixel effects like those provided by the shader are not feasible with this system.
 */

#include <cmath>
#include <ImposterRenderer/MeshHelixRenderer.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Vector.h>

GLUquadric* MeshHelixRenderer::m_Quadric = 0;
const int MeshHelixRenderer::LEVEL_OF_DETAIL = 10;
// constants that approximate Vinay's Helix drawing
const GLfloat MeshHelixRenderer::HEIGHT_STEP = 0.03f;
const GLfloat MeshHelixRenderer::STRIP_HEIGHT = 0.6;
const GLfloat MeshHelixRenderer::HEIGHT_TO_COILS = 0.08f;

void MeshHelixRenderer::DrawHelixBuffer(const ExpandableBuffer<GLfloat>& position, const ExpandableBuffer<GLfloat>& color, const ExpandableBuffer<GLfloat>& textureCoord1, const ExpandableBuffer<GLfloat>& textureCoord2)
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
		// gluHelix draws on the z axis.
		// so we need to rotate helixUp to align with the z axis
		const CCVOpenGLMath::Vector helixUp = (endpoint2 - endpoint1);
		const GLfloat height = helixUp.norm();
		// convert helixUp to spherical coordinates.  OpenGL needs this in degrees.
		const GLfloat RAD_TO_DEG = 180.0f/M_PI;
		const GLfloat phi = acos(helixUp[2]/height);
		const GLfloat theta = atan2(helixUp[1],helixUp[0]);
		// prepare to draw
		// (rotate phi down, remaining aligned with x-axis, then rotate theta)
		glTranslatef(endpoint1[0], endpoint1[1], endpoint1[2]);
		glRotatef(theta * RAD_TO_DEG, 0, 0, 1);
		glRotatef(phi * RAD_TO_DEG, 0, 1, 0);
		// construct a triangle strip helix
		const GLfloat HEIGHT_TO_THETA = (1.0f / height) * 360.0f * HEIGHT_TO_COILS;
		int nAngles = height / HEIGHT_STEP;
		int nVertices = 2 * nAngles;
		GLfloat vertices[nVertices][3];
		GLfloat normals[nVertices][3];
		GLfloat heightIter = 0;
		for (int i = 0; i < nVertices; i+=2)
		{
			GLfloat thetaIter = heightIter * HEIGHT_TO_THETA;
			normals[i+1][0] = normals[i][0] = cos(thetaIter);
			normals[i+1][1] = normals[i][1] = sin(thetaIter);
			normals[i+1][2] = normals[i][2] = 0;
			vertices[i][0] = radius * cos(thetaIter);
			vertices[i][1] = radius * sin(thetaIter);
			vertices[i][2] = heightIter + STRIP_HEIGHT;
			vertices[i+1][0] = radius * cos(thetaIter);
			vertices[i+1][1] = radius * sin(thetaIter);
			vertices[i+1][2] = heightIter;
			heightIter += HEIGHT_STEP;
		}
		// render
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glNormalPointer(GL_FLOAT, 0, normals);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, nVertices);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glPopMatrix();
	}
}
