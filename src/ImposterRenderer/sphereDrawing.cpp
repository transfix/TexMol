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
#include <ImposterRenderer/sphereDrawing.h>
#include <math.h>

#define XC (float).52573111
#define ZC (float).85065080
#define YC (float)0.0

static GLuint faces[20][3] =
{
	{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
	{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
	{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
	{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
};

static GLfloat vdata[12][3] =
{
	{-XC,YC,ZC},{XC,YC,ZC},{-XC,YC,-ZC},{XC,YC,-ZC},
	{YC,ZC,XC},{YC,ZC,-XC},{YC,-ZC,XC},{YC,-ZC,-XC},
	{ZC,XC,YC},{-ZC,XC,YC},{ZC,-XC,YC},{-ZC,-XC,YC}
};

// Draw a triangulation
void DrawCurve(float* v1, float* v2, float* v3, int s)
{
	if (s == 0)
	{
		glNormal3fv(v1);
		glVertex3fv(v1);
		glNormal3fv(v2);
		glVertex3fv(v2);
		glNormal3fv(v3);
		glVertex3fv(v3);
	}
	else
	{
		float v12[3], v23[3], v13[3], l;
		v12[0] = (v1[0] + v2[0]);
		v12[1] = (v1[1] + v2[1]);
		v12[2] = (v1[2] + v2[2]);
		l = (float) sqrt(v12[0]*v12[0] + v12[1]*v12[1] + v12[2]*v12[2]);
		v12[0] /= l;
		v12[1] /= l;
		v12[2] /= l;
		v23[0] = (v2[0] + v3[0]);
		v23[1] = (v2[1] + v3[1]);
		v23[2] = (v2[2] + v3[2]);
		l = (float) sqrt(v23[0]*v23[0] + v23[1]*v23[1] + v23[2]*v23[2]);
		v23[0] /= l;
		v23[1] /= l;
		v23[2] /= l;
		v13[0] = (v1[0] + v3[0]);
		v13[1] = (v1[1] + v3[1]);
		v13[2] = (v1[2] + v3[2]);
		l = (float) sqrt(v13[0]*v13[0] + v13[1]*v13[1] + v13[2]*v13[2]);
		v13[0] /= l;
		v13[1] /= l;
		v13[2] /= l;
		DrawCurve(v1, v12, v13, s-1);
		DrawCurve(v2, v23, v12, s-1);
		DrawCurve(v3, v13, v23, s-1);
		DrawCurve(v12, v23, v13, s-1);
	}
}

// Draw a sphere using openGL.
// Arg c specifies the level of refinement for the triangulated sphere, where 0 is the coarsest refinement.
void CCVImposterRenderer::VoroDrawSphere(int c)
{
	glBegin(GL_TRIANGLES);
	for (int i=0; i<20; i++)
	{
		DrawCurve(&vdata[faces[i][1]][0], &vdata[faces[i][0]][0], &vdata[faces[i][2]][0], c);
	}
	glEnd();
}
