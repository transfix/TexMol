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
#include <GeometryRenderer/TransparentVolumeRenderer.h>
#include <Geometry/Geometry.h>
#include <GeometryRenderer/GeometryRenderer.h>


Geometry* TransparentVolumeRenderer::m_ContourGeometry = 0;
Geometry* TransparentVolumeRenderer::m_VolumeGeometry = 0;

void TransparentVolumeRenderer::render(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn)
{
	// save state
	const float alpha = volumeData->getAlpha();

	// render with geometry renderer
	static CCVTexMolGeometryRenderer::GeometryRenderer gr;
	// get geometry
	if (m_ContourGeometry!=0)
	{
		delete m_ContourGeometry;
	}
	m_ContourGeometry = volumeData->getGeometry();
	if (m_VolumeGeometry!=0)
	{
		delete m_VolumeGeometry;
	}
	m_VolumeGeometry = volumeRendererExtn->getGeometry();
	if (m_ContourGeometry!=0)
	{
		// tell geometry renderer to draw with triangles
		m_ContourGeometry->m_RenderTriangles = true;
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		enableContourRendererGLState(volumeData, volumeRendererExtn);
		// exectue render, should be using texturemapped triangles
		gr.render(m_ContourGeometry,0,"SMOOTH",Plane(), false,alpha);
		glPopAttrib();
	}

	// build volume list
	// if(m_VolumeGeometry!=0) {
	//         // tell geometry renderer to draw with triangles
	//         m_VolumeGeometry->m_RenderTriangles = true;
	//         glPushAttrib( GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//         glPushMatrix();
	//         {
	//                 enableVolumeRendererGLState(volumeData, volumeRendererExtn);
	//                 // exectue render, should be using texturemapped triangles ?
	//                 GLfloat black[4] = { 0, 0, 0, 1.0f };
	//                 // gr.render(volumeGeometry,black,"SMOOTH",Plane(), false,alpha);
	//                 gr.render(m_VolumeGeometry,0,"SMOOTH",Plane(), false,alpha);
	//         }
	//         glPopMatrix();
	//         glPopAttrib();
	// }
}

void TransparentVolumeRenderer::enableContourRendererGLState(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn)
{
	glDisable(GL_CULL_FACE);
	// setup clipping
	Plane plane = volumeData->getClippingPlane();
	double plane0[] = { plane.a(), plane.b(), plane.c(), plane.d() };
	glClipPlane(GL_CLIP_PLANE0, plane0);
	glEnable(GL_CLIP_PLANE0);
}

void TransparentVolumeRenderer::enableVolumeRendererGLState(VolumeData* volumeData, VolumeRendererExtn* volumeRendererExtn)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	volumeRendererExtn->setNearPlane(volumeData->getClippingPlaneExtent());
	float largestSide = volumeRendererExtn->getLargestSide();
	// get it to right pos by translating and scaling
	// do it in right OpenGL order
	// 3. Shift by min vals
	double minX, minY, minZ, maxX, maxY, maxZ;
	volumeRendererExtn->getExtents(minX, minY, minZ, maxX, maxY, maxZ);
	glTranslatef(minX, minY, minZ);
	// 2. Scale by largest dimension
	glScalef(largestSide, largestSide, largestSide);
	// 1. Shift by 0.5 in max, something smaller elsewhere to make it go from 0 - <=1
	glTranslatef((maxX-minX)/(largestSide * 2.0), (maxY-minY)/(largestSide * 2.0), (maxZ-minZ)/(largestSide * 2.0));
	volumeRendererExtn->prepareToRenderGeometry();
}

