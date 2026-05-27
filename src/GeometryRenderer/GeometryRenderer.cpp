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
// GeometryRenderer.cpp: implementation of the GeometryRenderer class.

#include <algorithm>
//#include "DepthPeelingRenderer.h"
#include <Geometry/Geometry.h>
#include <Geometry/GeometryScene.h>
#include <GeometryRenderer/BumpMapRenderer.h>
#include <GeometryRenderer/GeometryRenderer.h>
#include <Geometry/Texture2D.h>
#include <iostream>
#include <math.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#if defined(__APPLE__)
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
#include <TexMol/compat.h>
#endif

using CCVTexMolGeometryRenderer::GeometryRenderer;
using CCVTexMolGeometryRenderer::BumpMapRenderer;

// Statics
//Geometry* GeometryRenderer::m_TransparentGeometry = 0;
std::vector<Geometry*> GeometryRenderer::m_TransparentGeometry;
std::vector<CCVOpenGLMath::Matrix*> GeometryRenderer::m_TransparentGeoMatrices;

GeometryRenderer::GeometryRenderer()
{
	m_RenderingTypes = new char*[getNumberOfRenderingTypes()];
	for (int i=0; i<getNumberOfRenderingTypes(); i++)
	{
		m_RenderingTypes[i] = new char[256];
	}
	strcpy(m_RenderingTypes[0], "Smooth");
	strcpy(m_RenderingTypes[1], "Flat");
	m_UseBumpMapping = false;
	m_BumpMapRenderer = 0;
}

GeometryRenderer::~GeometryRenderer()
{
	delete m_BumpMapRenderer;
	m_BumpMapRenderer = 0;
}

bool GeometryRenderer::render(GeometryScene* geometryScene, float* surfaceColors, const char* renderingTypeName, Plane plane, bool useClippingPlane, GLfloat surfaceAlpha)
{
	if (!geometryScene)
	{
		return false;
	}

	GLuint list;
	Geometry* geometry;
	for (int c=0; c<geometryScene->getNumberOfSceneArrayNodes(); c++)
	{
		geometry = geometryScene->getIth(c)->getGeometry();
		if (geometry)
		{
			list = geometryScene->getIth(c)->getInitializationCallList();
			glCallList(list);
			drawGeometry(geometry, surfaceColors, renderingTypeName,
						 plane, useClippingPlane, surfaceAlpha);
			list = geometryScene->getIth(c)->getFinalizationCallList();
			glCallList(list);
		}
	}
	return true;
}

void setClipPlane1()
{
	double plane0[] = { 0.0, 0.0, -1.0, -20 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE0, plane0);
	glEnable(GL_CLIP_PLANE0);
}

void setClipPlane2()
{
	double plane1[] = { 0.0, 0.0, +1.0, -20 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE1, plane1);
	glEnable(GL_CLIP_PLANE1);
}

bool GeometryRenderer::render(Geometry* geometry)
{
	Plane plane;
	return render(geometry, 0, "SMOOTH", plane, false);
}

bool GeometryRenderer::render(Geometry* geometry, float* surfaceColors, const char* renderingTypeName, Plane plane, bool useClippingPlane, float surfaceAlpha)
{
	if (!geometry)
	{
		return false;
	}

	drawGeometry(geometry, surfaceColors, renderingTypeName,
				 plane, useClippingPlane, surfaceAlpha);
	return true;
}

int GeometryRenderer::getNumberOfRenderingTypes()
{
	return SIZE;
}

char* GeometryRenderer::getRenderingTypeName(int index)
{
	if (index < 0 || index >= getNumberOfRenderingTypes())
	{
		return (char*)"Invalid";
	}
	return m_RenderingTypes[index];
}

GeometryRenderer::RenderingType GeometryRenderer::getRenderingType(const char* renderingTypeName)
{
	if (!renderingTypeName)
	{
		return INVALID;
	}
#ifdef _WIN32
	if (strcmpi(renderingTypeName, "Smooth"))
	{
		return SMOOTH;
	}
	if (strcmpi(renderingTypeName, "Flat"))
	{
		return FLAT;
	}
#else
	if (strcasecmp(renderingTypeName, "Smooth"))
	{
		return SMOOTH;
	}
	if (strcasecmp(renderingTypeName, "Flat"))
	{
		return FLAT;
	}
#endif
	return INVALID;
}

void GeometryRenderer::drawGeometry(Geometry* geometry, float* surfaceColors, const char* renderingTypeName,
									Plane plane, bool useClippingPlane, float surfaceAlpha)
{
	if (useClippingPlane)
	{
		double plane0[] = { plane.a(), plane.b(), plane.c(), plane.d() };
		glClipPlane(GL_CLIP_PLANE0, plane0);
		glEnable(GL_CLIP_PLANE0);
	}
	RenderingType renderingType = getRenderingType(renderingTypeName);
	// to fail or not to fail!
	if (renderingType == INVALID)
	{
		renderingType = FLAT;
	}
	{
		const bool transparentRender = (surfaceAlpha < 1.0);
		// draw with transparency
		if (transparentRender)
		{
			glColor4f(geometry->m_DiffuseColor[0],geometry->m_DiffuseColor[1],geometry->m_DiffuseColor[2],surfaceAlpha);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		// old
		else
		{
			glColor4fv(geometry->m_DiffuseColor);
		}
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, geometry->m_DiffuseColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, geometry->m_SpecularColor);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, geometry->m_AmbientColor);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, geometry->m_Shininess);
		glLineWidth(geometry->m_LineWidth);
		if (geometry->m_NumPoints)
		{
			if (geometry->m_RenderPoints)
			{
				geometry->GetReadyToDrawWire();
				drawPoints(geometry);
			}
		}
		if (geometry->m_NumLines)
		{
			if (geometry->m_RenderLines)
			{
				geometry->GetReadyToDrawWire();
				drawLines(geometry);
			}
		}
		if (geometry->m_NumTris)
		{
			geometry->GetReadyToDrawSmooth();
			if (geometry->m_RenderWireframe)
			{
				if (transparentRender)
				{
					glDisable(GL_BLEND);
				}
				drawTrisWireframe(geometry);
				if (transparentRender)
				{
					glEnable(GL_BLEND);
				}
			}
			if (geometry->m_RenderTriangles)
			{
				drawTris(geometry, surfaceColors, surfaceAlpha);
			}
		}
		if (geometry->m_NumQuads)
		{
			//geometry->GetReadyToDrawSmooth();
			geometry->GetReadyToDrawFlat();
			//drawQuads(geometry);
			drawFlatQuads(geometry);
		}
		if (transparentRender)
		{
			glDisable(GL_BLEND);
		}
	}
	if (useClippingPlane)
	{
		glDisable(GL_CLIP_PLANE0);
	}
}

void GeometryRenderer::drawPoints(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_POINT_BIT);
	glPointSize(geometry->m_PointSize);
	glEnable(GL_POINT_SMOOTH);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(geometry->m_UniquePointColors[0], geometry->m_UniquePointColors[1], geometry->m_UniquePointColors[2]);
	if (!geometry->useUniquePointColors() && geometry->m_PointColorsArray)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, geometry->m_PointColorsArray);
	}
	glVertexPointer(3, GL_FLOAT, 0, geometry->m_Points);
	glDrawArrays(GL_POINTS, 0, geometry->m_NumPoints);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (!geometry->useUniquePointColors() && geometry->m_PointColorsArray)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glPopAttrib();
}

void GeometryRenderer::drawLines(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(geometry->m_UniqueLineColors[0], geometry->m_UniqueLineColors[1], geometry->m_UniqueLineColors[2]);
	if (!geometry->useUniqueLineColors() && geometry->m_LineColors)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, geometry->m_LineColors);
	}
	glVertexPointer(3, GL_FLOAT, 0, geometry->m_LineVerts);
	glDrawElements(GL_LINES, geometry->m_NumLines*2, GL_UNSIGNED_INT, geometry->m_Lines);
	glDisableClientState(GL_VERTEX_ARRAY);
	if (!geometry->useUniqueLineColors() && geometry->m_LineColors)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glPopAttrib();
}

void GeometryRenderer::drawTris(Geometry* geometry, float* surfaceColors, float surfaceAlpha)
{
	const bool transparentRender = (surfaceAlpha < 1.0);
	// just draw normally
	if (!transparentRender)
	{
		drawTrisInternal(geometry, surfaceColors, surfaceAlpha);
	}
	// add to large geometry which we will sort and draw in the end
	else
	{
		// convert from diffuse to vertex coloring
		if (!geometry->m_TriVertColorsTransparent)
		{
			geometry->AllocateTriVertColors();
			for (int i=0; i<geometry->m_NumTriVerts; i++)
			{
				geometry->m_TriVertColorsTransparent[i*4 + 0] = geometry->m_DiffuseColor[0];
				geometry->m_TriVertColorsTransparent[i*4 + 1] = geometry->m_DiffuseColor[1];
				geometry->m_TriVertColorsTransparent[i*4 + 2] = geometry->m_DiffuseColor[2];
				geometry->m_TriVertColorsTransparent[i*4 + 3] = surfaceAlpha;
			}
		}
		else
		{
			// just update the alphas in the color array
			for (int i=0; i<geometry->m_NumTriVerts; i++)
			{
				geometry->m_TriVertColorsTransparent[i*4 + 3] = surfaceAlpha;
			}
		}
		/*// copy to form base geometry
		if (!m_TransparentGeometry)
		{
			m_TransparentGeometry = new Geometry(*geometry);
		}
		// merge geometry
		else
		{
			Geometry* newGeometry = m_TransparentGeometry->merge(geometry);
			delete m_TransparentGeometry;
			m_TransparentGeometry = newGeometry;
		}*/
		m_TransparentGeometry.push_back(new Geometry(*geometry));
		//get the current modelview matrix, which would have been used if the geometry was opaque
		GLfloat mv[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, mv);
		m_TransparentGeoMatrices.push_back(new CCVOpenGLMath::Matrix(mv[0], mv[1], mv[2], mv[3],
									     mv[4], mv[5], mv[6], mv[7],
									     mv[8], mv[9], mv[10], mv[11],
									     mv[12], mv[13], mv[14], mv[15]));
	}
}

void GeometryRenderer::drawTrisInternal(Geometry* geometry, float* surfaceColors, float surfaceAlpha, bool updateAlpha, bool enableColorBuffer)
{
	const bool transparentRender = (surfaceAlpha < 1.0);
	if (m_UseBumpMapping && m_BumpMapRenderer)
	{
		bumpMapTriangles(geometry, surfaceColors);
	}
	else
	{
		glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		if (geometry->m_TriVertTexCoords2D)
		{
			if (geometry->m_Use2DTriangleTexture && (geometry->m_Texture2D->m_Id != -1))
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, geometry->m_Texture2D->m_Id);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 0, geometry->m_TriVertTexCoords2D);
				// MJS: somewhere else this state is being reset
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
		// MJS: add support for 3D texture rendering
		else if (geometry->m_TriVertTexCoords)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, 0, geometry->m_TriVertTexCoords);
			// MJS: somewhere else this state is being reset
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		geometry->bindNormalBuffer();
		geometry->bindVertexBuffer();
		if (!updateAlpha)
		{
			// bind without updating
			glEnableClientState(GL_COLOR_ARRAY);
			if (geometry->m_TriVertColorsTransparent)
			{
				geometry->bindColorBuffer(true);
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1,1,1,1);
		}
		else
		{
			// use per-vertex colors
			if (geometry->m_TriVertColorsTransparent)
			{
				glEnableClientState(GL_COLOR_ARRAY);
				if (transparentRender && surfaceAlpha != geometry->m_TriVertColorsTransparent[3])
				{
					// update the alphas in the color array
					for (int i=0; i<geometry->m_NumTriVerts; i++)
					{
						geometry->m_TriVertColorsTransparent[i*4 + 3] = surfaceAlpha;
					}
				}
				// bugfix: flickering
				geometry->bindColorBuffer(true);
			}
			// use full-surface colors
			else if (surfaceColors)
			{
				glColor4f(surfaceColors[0], surfaceColors[1], surfaceColors[2], surfaceAlpha);
			}
			else
			{
				// surface has no color data (i.e. RAW files).  we'll just use the
				// DiffuseColor already set by drawTris.
			}
		}
		if (transparentRender)
		{
			{
				if(!enableColorBuffer)
				{
					// first render the geometry only to the depth buffer
					glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
					glDepthMask(GL_TRUE);
					glEnable(GL_DEPTH_TEST);
					glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
				}
				else
				{
					// turn color buffer back on
					glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
				//	glDepthMask(GL_TRUE);
					glDepthMask(GL_FALSE);
					glEnable(GL_DEPTH_TEST);
					// now draw BACK-HULL, using greater_than_or_equals for the depth
					// back-hull shadows were deemed "distracting", so i took it out
			//		 glDepthFunc(GL_GEQUAL);
			//		 glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
					// now draw FRONT-HULL, using less_than_or_equals for the depth
					glDepthFunc(GL_LEQUAL);
					glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
				}
				// restore default depth test
				glDepthFunc(GL_LESS);
				glDepthMask(GL_TRUE);
			}
			/*// NEWEST
			{
				// first render the geometry only to the depth buffer
				glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
				glDepthMask(GL_TRUE);
				glEnable(GL_DEPTH_TEST);
				glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
				// turn color buffer back on
				glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
				// now draw BACK-HULL, using greater_than_or_equals for the depth
				// back-hull shadows were deemed "distracting", so i took it out
				// glDepthFunc(GL_GEQUAL);
				// glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
				// now draw FRONT-HULL, using less_than_or_equals for the depth
				glDepthFunc(GL_LEQUAL);
				glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
				// restore default depth test
				glDepthFunc(GL_LESS);
			}*/
			// // NEW
			// glDisable(GL_DEPTH_TEST);
			// geometry->sortGeometryTris();
			// glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT,geometry->getDepthOrderedIndices());
			// OLDEST
			// OLD DELETE
			// hackish solution is to do a two-pass render with front,
			// then back culling, respectively
			// glDisable(GL_DEPTH_TEST);
			// glEnable(GL_CULL_FACE);
			// glCullFace(GL_FRONT);
			// glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
			// glCullFace(GL_BACK);
			// glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
			// glDisable(GL_CULL_FACE);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, geometry->m_NumTris*3, GL_UNSIGNED_INT, geometry->m_Tris);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		if (geometry->m_TriVertTexCoords2D)
		{
			if (geometry->m_Use2DTriangleTexture && (geometry->m_Texture2D->m_Id != -1))
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}
		else if (geometry->m_TriVertTexCoords)
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if (!surfaceColors && geometry->m_TriVertColorsTransparent)
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}
		glPopAttrib();
	}
}

void GeometryRenderer::drawTrisWireframe(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_LINE_BIT);
	// init wireframe gl state
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glEnable(GL_LINE_SMOOTH);
	glPolygonOffset(1.0, 1.0);
	glColor3f(geometry->m_UniqueWireframeColors[0], geometry->m_UniqueWireframeColors[1], geometry->m_UniqueWireframeColors[2]);
	glLineWidth(geometry->m_WireframeWidth);
	// render all lines of the triangle mesh
	glBegin(GL_LINES);
	for (int i=0; i<geometry->m_NumTris; i++)
	{
		// draw the 3 lines, this is quite expensive. Maybe cache in future.
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
		int triangleIndex = i*3+0;
		int v0, v1, v2;
		v0 = geometry->m_Tris[triangleIndex + 0];
		v1 = geometry->m_Tris[triangleIndex + 1];
		v2 = geometry->m_Tris[triangleIndex + 2];
		x1 = geometry->m_TriVerts[v0*3+0];
		y1 = geometry->m_TriVerts[v0*3+1];
		z1 = geometry->m_TriVerts[v0*3+2];
		x2 = geometry->m_TriVerts[v1*3+0];
		y2 = geometry->m_TriVerts[v1*3+1];
		z2 = geometry->m_TriVerts[v1*3+2];
		x3 = geometry->m_TriVerts[v2*3+0];
		y3 = geometry->m_TriVerts[v2*3+1];
		z3 = geometry->m_TriVerts[v2*3+2];
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v0*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v0*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v0*4+2]);
			}
		}
		glVertex3f(x1, y1, z1);
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v1*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v1*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v1*4+2]);
			}
		}
		glVertex3f(x2, y2, z2);
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v0*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v0*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v0*4+2]);
			}
		}
		glVertex3f(x1, y1, z1);
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v2*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v2*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v2*4+2]);
			}
		}
		glVertex3f(x3, y3, z3);
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v1*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v1*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v1*4+2]);
			}
		}
		glVertex3f(x2, y2, z2);
		if (!geometry->useUniqueWireframeColors())
		{
			if (geometry->m_TriVertColorsTransparent)
			{
				glColor3f(1.0f-geometry->m_TriVertColorsTransparent[v2*4+0],
						  1.0f-geometry->m_TriVertColorsTransparent[v2*4+1],
						  1.0f-geometry->m_TriVertColorsTransparent[v2*4+2]);
			}
		}
		glVertex3f(x3, y3, z3);
	}
	glEnd();
	glPopAttrib();
}

// okay for alpha...
void GeometryRenderer::drawFlatTris(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (geometry->m_TriFlatTexCoords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, geometry->m_TriFlatTexCoords);
	}
	glVertexPointer(3, GL_FLOAT, 0, geometry->m_TriFlatVerts);
	glNormalPointer(GL_FLOAT, 0, geometry->m_TriFlatNormals);
	glDrawArrays(GL_TRIANGLES, 0, geometry->m_NumTris*3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopAttrib();
}

// okay for alpha...
void GeometryRenderer::drawQuads(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (geometry->m_QuadVertTexCoords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, geometry->m_QuadVertTexCoords);
	}
	glVertexPointer(3, GL_FLOAT, 0, geometry->m_QuadVerts);
	glNormalPointer(GL_FLOAT, 0, geometry->m_QuadVertNormals);
	glDrawElements(GL_QUADS, geometry->m_NumQuads*4, GL_UNSIGNED_INT, geometry->m_Quads);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopAttrib();
}

void GeometryRenderer::drawFlatQuads(Geometry* geometry)
{
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_LIGHTING);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if (geometry->m_QuadFlatTexCoords)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3, GL_FLOAT, 0, geometry->m_QuadFlatTexCoords);
	}
	glVertexPointer(3, GL_FLOAT, 0, geometry->m_QuadFlatVerts);
	glNormalPointer(GL_FLOAT, 0, geometry->m_QuadFlatNormals);
	glDrawArrays(GL_QUADS, 0, geometry->m_NumQuads*4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopAttrib();
}

void GeometryRenderer::setClipPlanes(double aspectX, double aspectY, double aspectZ) const
{
	double plane0[] = { 0.0, 0.0, 1.0, -20 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE0, plane0);
	glEnable(GL_CLIP_PLANE0);
	double plane1[] = { 0.0, 0.0, -1.0, -20 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE1, plane1);
	glEnable(GL_CLIP_PLANE1);
	/*double plane2[] = { 0.0, -1.0, 0.0, 60 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE2, plane2);
	glEnable(GL_CLIP_PLANE2);

	double plane3[] = { 0.0, 1.0, 0.0, 60 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE3, plane3);
	glEnable(GL_CLIP_PLANE3);

	double plane4[] = { -1.0, 0.0, 0.0, 40 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE4, plane4);
	glEnable(GL_CLIP_PLANE4);

	double plane5[] = { 1.0, 0.0, 0.0, 40 + 0.00001 };
	glClipPlane(GL_CLIP_PLANE5, plane5);
	glEnable(GL_CLIP_PLANE5);*/
}

void GeometryRenderer::disableClipPlanes() const
{
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE5);
}

bool GeometryRenderer::initBumpMapRendering()
{
	m_BumpMapRenderer = new BumpMapRenderer;
	if (!m_BumpMapRenderer->initialize())
	{
		delete m_BumpMapRenderer;
		m_BumpMapRenderer = 0;
		return false;
	}
	return true;
}

void GeometryRenderer::useBumpMapping(bool use)
{
	m_UseBumpMapping = use;
}

void GeometryRenderer::bumpMapTriangles(Geometry* geometry, float* surfaceColors)
{
	if (!m_BumpMapRenderer)
	{
		return;
	}
	m_BumpMapRenderer->render(geometry, surfaceColors);
}
void GeometryRenderer::resetTransparentGeometry()
{
	for(int i = 0; i < m_TransparentGeometry.size(); i++)
	{
		delete m_TransparentGeometry[i];
	}
	m_TransparentGeometry.clear();
	for(int i = 0; i < m_TransparentGeoMatrices.size(); i++)
	{
		delete m_TransparentGeoMatrices[i];
	}
	m_TransparentGeoMatrices.clear();
	//delete m_TransparentGeometry;
	//m_TransparentGeometry = 0;
}

void GeometryRenderer::renderTransparentGeometry()
{
	if (!m_TransparentGeometry.empty())
	{
		//save GL matrix state
		GLint originalMatrixMode;
		glGetIntegerv(GL_MATRIX_MODE, &originalMatrixMode);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		GeometryRenderer gr;
		//first render all geometry to depth buffer
/*		for(int i = 0; i < m_TransparentGeometry.size(); i++)
		{
			glLoadMatrixf(m_TransparentGeoMatrices[i]->transpose().getMatrix());
			gr.drawTrisInternal(m_TransparentGeometry[i], 0, 0, false, false);
		}
*/
		//exam bounding box to approximate order of geometry layer
/*
		if( m_TransparentGeometry.size() > 1 )
		{
			int *sortedGeom = new int[ m_TransparentGeometry.size() ];
			float *sortedGeomDist = new float[ m_TransparentGeometry.size() ];
			m_TransparentGeometry[ 0 ]->CalculateExtents();
			float *rp = m_TransparentGeometry[ 0 ]->m_Center;
			sortedGeom[ 0 ] = 0;
			sortedGeomDist[ 0 ] = m_TransparentGeometry[ 0 ]->getAverageSquareDist( rp[0], rp[1], rp[2] );

			for(int i = 1; i < m_TransparentGeometry.size(); i++) {
				sortedGeom[ i ] = i;
				sortedGeomDist[ i ] = m_TransparentGeometry[ i ]->getAverageSquareDist( rp[0], rp[1], rp[2] );
			}
		
			for(int i = 0; i < m_TransparentGeometry.size(); i++)
				fprintf( stderr, "dist: %f\n", sortedGeomDist[ i ]);

			for(int i = 0; i < m_TransparentGeometry.size(); i++)
				for(int j = 0; j < m_TransparentGeometry.size()-1; j++)
				{
					if( sortedGeomDist[ j ] > sortedGeomDist[ j + 1 ] )
					{
						int itmp = sortedGeom[ j ];
						float ftmp = sortedGeomDist[ j ];
						sortedGeom[ j ] = sortedGeom[ j + 1 ];
						sortedGeomDist[ j ] = sortedGeomDist[ j + 1 ];
						sortedGeom[ j + 1 ] = itmp;
						sortedGeomDist[ j + 1 ] = ftmp;
					}
				}
			for(int i = 0; i < m_TransparentGeometry.size(); i++)
			{
				glLoadMatrixf(m_TransparentGeoMatrices[ sortedGeom[ i ] ]->transpose().getMatrix());
				gr.drawTrisInternal(m_TransparentGeometry[ sortedGeom[ i ] ], 0, 0, false, true);
			}
			delete[] sortedGeom;
			delete[] sortedGeomDist;
		}
		else
		{
			glLoadMatrixf(m_TransparentGeoMatrices[ 0 ]->transpose().getMatrix());
			gr.drawTrisInternal(m_TransparentGeometry[ 0 ], 0, 0, false, true);
		}
*/
		//render again to color buffer
		for(int i = 0; i < m_TransparentGeometry.size(); i++)
		{
			glLoadMatrixf(m_TransparentGeoMatrices[i]->transpose().getMatrix());
			gr.drawTrisInternal(m_TransparentGeometry[i], 0, 0, false, true);
		}

		//restore GL matrix state
		glPopMatrix();
		glMatrixMode(originalMatrixMode);
	}
}
