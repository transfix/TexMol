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
#include <math.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <VolumeLibrary/ClipCube.h>
#include <VolumeLibrary/Polygon.h>
#include <VolumeLibrary/RendererBase.h>

using namespace OpenGLVolumeRendering;

RendererBase::RendererBase() : m_PolygonArray(100)
{
	initFlags();
	initAspectRatio();
	m_NumberOfPlanesRendered = 0;
	m_VertexArray = 0;
	m_TextureArray = 0;
	m_VertexArraySize = 0;
	m_TriangleArray = 0;
	m_TriangleArraySize = 0;
	m_NumVertices = 0;
	m_NumTriangles = 0;
}

RendererBase::~RendererBase()
{
	deallocateMemory();
}

// Initializes the renderer.
// Should be called again if the renderer is moved to a different openGL context.
// If this returns false, do not try to use it to do volumeRendering
bool RendererBase::initRenderer()
{
	m_BaseInitialized = true;
	setQuality(1.0);
	setNearPlane(0.0);
	setTextureSubCube(0,0,0,1,1,1);
	setDataSubVolume(0,0,0,1,1,1);
	setHintDimensions(0,0,0);
	return true;
}

// Sets the aspect ratio of the dataset.
bool RendererBase::setAspectRatio(double ratioX, double ratioY, double ratioZ)
{
	double maxratio;
	if (ratioX!=0.0 && ratioY!=0.0 && ratioZ!=0.0)
	{
		m_RatioX = ratioX;
		m_RatioY = ratioY;
		m_RatioZ = ratioZ;
		// find the maximum ratio
		maxratio = (m_RatioX > m_RatioY ? m_RatioX : m_RatioY);
		maxratio = (maxratio > m_RatioZ ? maxratio : m_RatioZ);
		// normalize so the max ratio is 1.0
		m_RatioX /= maxratio;
		m_RatioY /= maxratio;
		m_RatioZ /= maxratio;
		return true;
	}
	else
	{
		return false;
	}
}

// Specifies the portion of the uploaded texture that should be rendered.
// The extenst should range from 0 to 1.
bool RendererBase::setTextureSubCube(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	m_TextureSubCubeExtent.setExtents(minX, minY, minZ, maxX, maxY, maxZ);
	return true;
}

// Specifies that we are rendering a subportion of the full data.
// Used for out of core rendering.  The extents should range from 0 to 1.
bool RendererBase::setDataSubVolume(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	m_DataSubCubeExtent.setExtents(minX, minY, minZ, maxX, maxY, maxZ);
	return true;
}

// Used for out of core rendering.  The dimensions of the full dataset
bool RendererBase::setHintDimensions(unsigned int hintDimX, unsigned int hintDimY, unsigned int hintDimZ)
{
	m_HintDimX = hintDimX;
	m_HintDimY = hintDimY;
	m_HintDimZ = hintDimZ;
	return true;
}

// Quality is a number from 0 to 1.  Lower means faster.
bool RendererBase::setQuality(double quality)
{
	m_Quality = (quality>0.0?quality:0.0);
	m_Quality = (m_Quality<1.0?m_Quality:1.0);
	return true;
}

double RendererBase::getQuality() const
{
	return m_Quality;
}

// nearPlane is a number from 0 to 1.  0 means no clipping takes place.
// 1 means the entire volume is clipped.
bool RendererBase::setNearPlane(double nearPlane)
{
	m_NearPlane = (nearPlane>0.0?nearPlane:0.0);
	m_NearPlane = (m_NearPlane<1.0?m_NearPlane:1.0);
	return true;
}

double RendererBase::getNearPlane()
{
	return m_NearPlane;
}

// Returns the number of planes rendered in the last call to
// renderVolume.
int RendererBase::getNumberOfPlanesRendered() const
{
	return m_NumberOfPlanesRendered;
}

// Allocates memory for the vertices and triangles
bool RendererBase::allocateMemory(unsigned int numVerts, unsigned int numTriangles)
{
	// only allocate new memory if the old arrays aren't already
	// big enough
	if (numVerts>m_VertexArraySize)
	{
		if (!allocateVertexArray(numVerts))
		{
			m_NumVertices = 0;
			m_NumTriangles = 0;
			return false;
		}
	}
	if (numTriangles>m_TriangleArraySize)
	{
		if (!allocateTriangleArray(numTriangles))
		{
			m_NumVertices = 0;
			m_NumTriangles = 0;
			return false;
		}
	}
	m_NumVertices = numVerts;
	m_NumTriangles = numTriangles;
	return true;
}

// Deallocates the memory for vertices and triangles
void RendererBase::deallocateMemory()
{
	delete [] m_VertexArray;
	m_VertexArraySize = 0;
	delete [] m_TriangleArray;
	m_TriangleArraySize = 0;
}

// Allocate the vertex array
bool RendererBase::allocateVertexArray(unsigned int numVerts)
{
	delete [] m_VertexArray;
	delete [] m_TextureArray;
	m_VertexArray = new float[numVerts*3];
	m_TextureArray = new float[numVerts*3];
	if (m_VertexArray && m_TextureArray)
	{
		m_VertexArraySize = numVerts;
		return true;
	}
	else
	{
		m_VertexArraySize = 0;
		m_VertexArray = 0;
		m_TextureArray = 0;
		return false;
	}
}

// Allocate the triangle array
bool RendererBase::allocateTriangleArray(unsigned int numTriangles)
{
	delete [] m_TriangleArray;
	m_TriangleArray = new unsigned int[numTriangles*3];
	if (m_TriangleArray)
	{
		m_TriangleArraySize = numTriangles;
		return true;
	}
	else
	{
		m_TriangleArraySize = 0;
		return false;
	}
}

// Converts the polygon array to traingle and vertex arrays
void RendererBase::convertToTriangles()
{
	// determine the number of triangles and vertices
	int numTriangles = 0;
	int numVerts = 0;
	for (int c=0; c<m_PolygonArray.getNumPolygons(); c++)
	{
		numTriangles+=m_PolygonArray.getPolygon(c)->getNumTriangles();
		numVerts+=m_PolygonArray.getPolygon(c)->getNumVerts();
	}
	// set up the space for the triangles
	allocateMemory(numVerts, numTriangles);
	// fill up the arrays
	numTriangles = 0;
	numVerts = 0;
	double* vertex;
	double* texture;
	// for each polygon
	for (int c=0; c<m_PolygonArray.getNumPolygons(); c++)
	{
		// fill in the vertices
		for (int d=0; d<m_PolygonArray.getPolygon(c)->getNumVerts(); d++)
		{
			vertex = m_PolygonArray.getPolygon(c)->getVert(d);
			texture = m_PolygonArray.getPolygon(c)->getTexCoord(d);
			m_VertexArray[(numVerts+d)*3+0] = (float)vertex[0];
			m_VertexArray[(numVerts+d)*3+1] = (float)vertex[1];
			m_VertexArray[(numVerts+d)*3+2] = (float)vertex[2];
			m_TextureArray[(numVerts+d)*3+0] = (float)texture[0];
			m_TextureArray[(numVerts+d)*3+1] = (float)texture[1];
			m_TextureArray[(numVerts+d)*3+2] = (float)texture[2];
		}
		// fill in the triangles
		for (int d=0; d<m_PolygonArray.getPolygon(c)->getNumTriangles()*3; d++)
		{
			m_TriangleArray[(numTriangles*3)+d] = numVerts + m_PolygonArray.getPolygon(c)->getVertexForTriangles(d);
		}
		numTriangles+=m_PolygonArray.getPolygon(c)->getNumTriangles();
		numVerts+=m_PolygonArray.getPolygon(c)->getNumVerts();
	}
}

// Sets the aspectRatio to a default value.
bool RendererBase::initAspectRatio()
{
	m_RatioX = 1.0;
	m_RatioY = 1.0;
	m_RatioZ = 1.0;
	return true;
}

// Sets all flags to default values.
bool RendererBase::initFlags()
{
	m_BaseInitialized = false;
	return true;
}

// Returns the distance between planes.
double RendererBase::getIntervalWidth() const
{
	double cellWidthX = m_RatioX / m_HintDimX;
	double cellWidthY = m_RatioY / m_HintDimY;
	double cellWidthZ = m_RatioZ / m_HintDimZ;
	// find the minimum cell width
	double minWidth = (cellWidthX < cellWidthY ? cellWidthX : cellWidthY);
	minWidth = (minWidth < cellWidthZ ? minWidth : cellWidthZ);
//	return minWidth / 2.0 * ((1.0-m_Quality)*(1.0-m_Quality) * 10.0 + 1.0);
	// for test high quality volume rendering
	return minWidth / 5.0 * ((1.0-m_Quality)*(1.0-m_Quality) * 10.0 + 1.0);
}

// Returns a distance that is past the entire volume.
double RendererBase::getFurthestDistance() const
{
	return 0.5 * sqrt(m_RatioX*m_RatioX + m_RatioY*m_RatioY + m_RatioZ*m_RatioZ);
}

// Returns a distance that is before the entire volume.
double RendererBase::getNearestDistance() const
{
	double diagonal = sqrt(m_RatioX*m_RatioX + m_RatioY*m_RatioY + m_RatioZ*m_RatioZ);
	return (-0.5 * diagonal) +
		   (m_NearPlane * diagonal);
}

Plane RendererBase::getViewPlane()
{
	return OpenGL_Viewer::getViewPlane();
}


// Computes the polygons that need to be rendered
void RendererBase::computePolygons()
{
	m_PolygonArray.clearPolygons();
	Plane plane = getViewPlane();
	ClipCube cube(m_RatioX, m_RatioY, m_RatioZ,
				  m_TextureSubCubeExtent.m_MinX,
				  m_TextureSubCubeExtent.m_MinY,
				  m_TextureSubCubeExtent.m_MinZ,
				  m_TextureSubCubeExtent.m_MaxX,
				  m_TextureSubCubeExtent.m_MaxY,
				  m_TextureSubCubeExtent.m_MaxZ);
	Polygon polygon(0);
	for (double z = getFurthestDistance(); z>getNearestDistance(); z-=getIntervalWidth())
	{
		plane[3] = z;
		if (cube.clipPlane(polygon, plane))
		{
			m_PolygonArray.addPolygon(polygon);
		}
	}

	m_NumberOfPlanesRendered = m_PolygonArray.getNumPolygons();
}

Geometry* RendererBase::getGeometry()
{
	computePolygons();
	convertToTriangles();
	// numTriangles, numVerts
	Geometry* returnValue = new Geometry;
	returnValue->AllocateTris(m_NumVertices, m_NumTriangles);
	returnValue->AllocateTriTexCoords();
	for (int i=0; i<m_NumVertices; i++)
	{
		returnValue->m_TriVerts[i*3+0] = m_VertexArray[i*3+0];
		returnValue->m_TriVerts[i*3+1] = m_VertexArray[i*3+1];
		returnValue->m_TriVerts[i*3+2] = m_VertexArray[i*3+2];
		returnValue->m_TriVertTexCoords[i*3+0] = m_TextureArray[i*3+0];
		returnValue->m_TriVertTexCoords[i*3+1] = m_TextureArray[i*3+1];
		returnValue->m_TriVertTexCoords[i*3+2] = m_TextureArray[i*3+2];
	}
	for (int i=0; i<m_NumTriangles; i++)
	{
		returnValue->m_Tris[i*3+0] = m_TriangleArray[i*3+0];
		returnValue->m_Tris[i*3+1] = m_TriangleArray[i*3+1];
		returnValue->m_Tris[i*3+2] = m_TriangleArray[i*3+2];
	}
	return returnValue;
}

void RendererBase::prepareToRenderGeometry()
{
}
