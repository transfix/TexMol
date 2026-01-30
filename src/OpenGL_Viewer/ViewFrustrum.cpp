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
// NOTE: Thanks to Dion Picco at flipcode.com for his tutorial on view frustrum culling.

#include <math.h>
#include <OpenGL_Viewer/ViewFrustrum.h>
#include <stdio.h>
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

ViewFrustrum::ViewFrustrum()
{
	update();
}

ViewFrustrum::~ViewFrustrum()
{
}

// tests if a sphere is within the frustrum
ViewFrustrum::POSITION ViewFrustrum::intersects(double* center, double radius)
{
	// various distances
	double fDistance;
	// calculate our distances to each of the planes
	for (int i = 0; i < 6; ++i)
	{
		// find the distance to this plane (dot product + d)
		fDistance = m_Planes[i].a()*center[0] + m_Planes[i].b()*center[1] + m_Planes[i].c()*center[2] + m_Planes[i].d();
		// if this distance is < -sphere.radius, we are outside
		if (fDistance < -radius)
		{
			return OUTSIDE;
		}
		// else if the distance is between +- radius, then we intersect
		if ((float)fabs(fDistance) < radius)
		{
			return INTERSECTS;
		}
	}
	// otherwise we are fully in view
	return INSIDE;
}

bool ViewFrustrum::sideOfPlane(Plane plane, CCVOpenGLMath::Vector point)
{
	float dist;
	dist = plane.m_A*point[0] + plane.m_B*point[1] + plane.m_C*point[2] + plane.m_D;
	if (dist < 0)
	{
		return false;
	}
	return true;
}

ViewFrustrum::POSITION ViewFrustrum::intersects(double* min, double* max)
{
	CCVOpenGLMath::Vector vCorner[8];
	int iTotalIn = 0;
	// get the corners of the box into the vCorner array
	CCVOpenGLMath::Vector::getCorners(min, max, vCorner);
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p)
	{
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i)
		{
			// test this point against the planes
			if (!sideOfPlane(m_Planes[p], vCorner[i]))
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
		{
			return OUTSIDE;
		}
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
	{
		return INSIDE;
	}
	// we must be partly in then otherwise
	return INTERSECTS;
}

void ViewFrustrum::getMinMaxArea(double* min, double* max, double* minArea, double* maxArea)
{
}

void ViewFrustrum::print()
{
	printf("*****\n");
	printf("L: [%f,%f,%f] %f\n", m_Planes[0].m_A, m_Planes[0].m_B, m_Planes[0].m_C, m_Planes[0].m_D);
	printf("R: [%f,%f,%f] %f\n", m_Planes[1].m_A, m_Planes[1].m_B, m_Planes[1].m_C, m_Planes[1].m_D);
	printf("T: [%f,%f,%f] %f\n", m_Planes[2].m_A, m_Planes[2].m_B, m_Planes[2].m_C, m_Planes[2].m_D);
	printf("B: [%f,%f,%f] %f\n", m_Planes[3].m_A, m_Planes[3].m_B, m_Planes[3].m_C, m_Planes[3].m_D);
	printf("N: [%f,%f,%f] %f\n", m_Planes[4].m_A, m_Planes[4].m_B, m_Planes[4].m_C, m_Planes[4].m_D);
	printf("F: [%f,%f,%f] %f\n", m_Planes[5].m_A, m_Planes[5].m_B, m_Planes[5].m_C, m_Planes[5].m_D);
}

void ViewFrustrum::update()
{
	// form the MVP matrix
	GLfloat mv[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	GLdouble p[16];
	glGetDoublev(GL_PROJECTION_MATRIX, p);
	m_MVP_Matrix[0]  = mv[0]*p[0]+ mv[1]*p[4]+ mv[2]*p[8]+ mv[3]*p[12];
	m_MVP_Matrix[1]  = mv[0]*p[1]+ mv[1]*p[5]+ mv[2]*p[9]+ mv[3]*p[13];
	m_MVP_Matrix[2]  = mv[0]*p[2]+ mv[1]*p[6]+ mv[2]*p[10]+mv[3]*p[14];
	m_MVP_Matrix[3] = mv[0]*p[3]+ mv[1]*p[7]+ mv[2]*p[11]+mv[3]*p[15];
	m_MVP_Matrix[4]  = mv[4]*p[0]+ mv[5]*p[4]+ mv[6]*p[8]+ mv[7]*p[12];
	m_MVP_Matrix[5]  = mv[4]*p[1]+ mv[5]*p[5]+ mv[6]*p[9]+ mv[7]*p[13];
	m_MVP_Matrix[6]  = mv[4]*p[2]+ mv[5]*p[6]+ mv[6]*p[10]+mv[7]*p[14];
	m_MVP_Matrix[7] = mv[4]*p[3]+ mv[5]*p[7]+ mv[6]*p[11]+mv[7]*p[15];
	m_MVP_Matrix[8]  = mv[8]*p[0]+ mv[9]*p[4]+ mv[10]*p[8]+ mv[11]*p[12];
	m_MVP_Matrix[9]  = mv[8]*p[1]+ mv[9]*p[5]+ mv[10]*p[9]+ mv[11]*p[13];
	m_MVP_Matrix[10] = mv[8]*p[2]+ mv[9]*p[6]+ mv[10]*p[10]+mv[11]*p[14];
	m_MVP_Matrix[11] = mv[8]*p[3]+ mv[9]*p[7]+ mv[10]*p[11]+mv[11]*p[15];
	m_MVP_Matrix[12] = mv[12]*p[0]+ mv[13]*p[4]+ mv[14]*p[8]+ mv[15]*p[12];
	m_MVP_Matrix[13] = mv[12]*p[1]+ mv[13]*p[5]+ mv[14]*p[9]+ mv[15]*p[13];
	m_MVP_Matrix[14]= mv[12]*p[2]+ mv[13]*p[6]+ mv[14]*p[10]+mv[15]*p[14];
	m_MVP_Matrix[15]= mv[12]*p[3]+ mv[13]*p[7]+ mv[14]*p[11]+mv[15]*p[15];
	// form the 6 planes of the frustrum
	m_Planes[0].m_A = m_MVP_Matrix[3]  + m_MVP_Matrix[0];
	m_Planes[0].m_B = m_MVP_Matrix[7]  + m_MVP_Matrix[4];
	m_Planes[0].m_C = m_MVP_Matrix[11] + m_MVP_Matrix[8];
	m_Planes[0].m_D = m_MVP_Matrix[15] + m_MVP_Matrix[12];
	m_Planes[1].m_A = m_MVP_Matrix[3]  - m_MVP_Matrix[0];
	m_Planes[1].m_B = m_MVP_Matrix[7]  - m_MVP_Matrix[4];
	m_Planes[1].m_C = m_MVP_Matrix[11] - m_MVP_Matrix[8];
	m_Planes[1].m_D = m_MVP_Matrix[15] - m_MVP_Matrix[12];
	m_Planes[2].m_A = m_MVP_Matrix[3]  - m_MVP_Matrix[1];
	m_Planes[2].m_B = m_MVP_Matrix[7]  - m_MVP_Matrix[5];
	m_Planes[2].m_C = m_MVP_Matrix[11] - m_MVP_Matrix[9];
	m_Planes[2].m_D = m_MVP_Matrix[15] - m_MVP_Matrix[13];
	m_Planes[3].m_A = m_MVP_Matrix[3]  + m_MVP_Matrix[1];
	m_Planes[3].m_B = m_MVP_Matrix[7]  + m_MVP_Matrix[5];
	m_Planes[3].m_C = m_MVP_Matrix[11] + m_MVP_Matrix[9];
	m_Planes[3].m_D = m_MVP_Matrix[15] + m_MVP_Matrix[13];
	m_Planes[4].m_A = m_MVP_Matrix[3]  + m_MVP_Matrix[2];
	m_Planes[4].m_B = m_MVP_Matrix[7]  + m_MVP_Matrix[6];
	m_Planes[4].m_C = m_MVP_Matrix[11] + m_MVP_Matrix[10];
	m_Planes[4].m_D = m_MVP_Matrix[15] + m_MVP_Matrix[14];
	m_Planes[5].m_A = m_MVP_Matrix[3]  - m_MVP_Matrix[2];
	m_Planes[5].m_B = m_MVP_Matrix[7]  - m_MVP_Matrix[6];
	m_Planes[5].m_C = m_MVP_Matrix[11] - m_MVP_Matrix[10];
	m_Planes[5].m_D = m_MVP_Matrix[15] - m_MVP_Matrix[14];
	for (int i=0; i<6; i++)
	{
		m_Planes[i].normalizeNormal();
	}
}
