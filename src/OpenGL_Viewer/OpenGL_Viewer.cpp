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
#include <Geometry/GeometryScene.h>
#include <GeometryRenderer/GeometryRenderer.h>
#include <math.h>
#include <OpenGL_Viewer/Grid.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <OpenGL_Viewer/OrthographicView.h>
#include <OpenGL_Viewer/PerspectiveView.h>
#include <OpenGL_Viewer/Plane.h>
#include <OpenGL_Viewer/ViewFrustrum.h>
#include <stdlib.h>

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

OpenGL_Viewer::OpenGL_Viewer()
{
	m_Grid = new Grid();
	m_ViewFrustrum = new ViewFrustrum();
	m_View = 0;
}

OpenGL_Viewer::~OpenGL_Viewer()
{
	delete m_Grid;
	m_Grid = 0;
	delete m_ViewFrustrum;
	m_ViewFrustrum = 0;
}

bool OpenGL_Viewer::initParams()
{
	m_bShowGrid = true;
	m_ViewType = ORTHOGRAPHIC_VIEW;
	return initView();
}

bool OpenGL_Viewer::initView()
{
	int w, h;
	bool oldView = false;
	if (m_View)
	{
		w = m_View->m_Width;
		h = m_View->m_Height;
		oldView = true;
	}
	if (m_ViewType == ORTHOGRAPHIC_VIEW)
	{
		m_View = new OrthographicView(30.0);
		m_ViewDirection = Top;
	}
	else
	{
		// we have only perspective left currently!
		m_View = new PerspectiveView(30.0);
		m_Grid->setGridPerspective();
		m_ViewDirection = Perspective;
	}
	if (oldView)
	{
		m_View->m_Width = w;
		m_View->m_Height = h;
	}
	return true;
}

void OpenGL_Viewer::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);
	m_View->resizeWindow(w,h);
}

void OpenGL_Viewer::setFieldOfView(double fieldOfView)
{
	if (m_ViewDirection==Perspective)
	{
		(dynamic_cast <PerspectiveView*>(m_View))->setFieldOfView((float)fieldOfView/180.0f*3.141592653f);
	}
}

void OpenGL_Viewer::resetFieldOfView()
{
	if (m_ViewDirection==Perspective)
	{
		(dynamic_cast <PerspectiveView*>(m_View))->setFieldOfView(PerspectiveView::defaultFieldOfView/180.0f*3.141592653f);
	}
}

void OpenGL_Viewer::setGridSpacing(double gridspacing)
{
	m_Grid->SetGridSpacing((float)fabs(gridspacing));
}

void OpenGL_Viewer::resetGridSpacing()
{
	m_Grid->SetGridSpacing(Grid::defaultGridSpacing);
}

void OpenGL_Viewer::setGridExtent(const double gridExtent)
{
	m_Grid->SetGridSize((float)gridExtent);
}

void OpenGL_Viewer::resetGridExtent()
{
	m_Grid->SetGridSize(Grid::defaultGridExtent);
}

void OpenGL_Viewer::setMajorSpacing(unsigned int majorSpacing)
{
	m_Grid->SetMajorSpacing(majorSpacing);
}

void OpenGL_Viewer::resetMajorSpacing()
{
	m_Grid->SetMajorSpacing(Grid::defaultMajorSpacing);
}

void OpenGL_Viewer::setViewDirection(Views view)
{
	switch (view)
	{
	case Perspective:
		setViewDirectionPerspective();
		break;
	case Top:
		setViewDirectionTop();
		break;
	case Bottom:
		setViewDirectionBottom();
		break;
	case Left:
		setViewDirectionLeft();
		break;
	case Right:
		setViewDirectionRight();
		break;
	case Front:
		setViewDirectionFront();
		break;
	case Back:
		setViewDirectionBack();
		break;
	}
}

void OpenGL_Viewer::setViewDirectionPerspective()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = new PerspectiveView(size);
	m_Grid->setGridPerspective();
	m_ViewDirection = Perspective;
}

void OpenGL_Viewer::setViewDirectionTop()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Top(size);
	m_Grid->setGridTop();
	m_ViewDirection = Top;
}

void OpenGL_Viewer::setViewDirectionBottom()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Bottom(size);
	m_Grid->setGridBottom();
	m_ViewDirection = Bottom;
}

void OpenGL_Viewer::setViewDirectionRight()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Right(size);
	m_Grid->setGridRight();
	m_ViewDirection = Right;
}

void OpenGL_Viewer::setViewDirectionLeft()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Left(size);
	m_Grid->setGridLeft();
	m_ViewDirection = Left;
}

void OpenGL_Viewer::setViewDirectionFront()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Front(size);
	m_Grid->setGridFront();
	m_ViewDirection = Front;
}

void OpenGL_Viewer::setViewDirectionBack()
{
	float size = m_View->GetWindowSize();
	delete m_View;
	m_View = OrthographicView::Back(size);
	m_Grid->setGridBack();
	m_ViewDirection = Back;
}

void OpenGL_Viewer::resetViewDirection()
{
	setViewDirectionTop();
}

double OpenGL_Viewer::FieldOfView() const
{
	if (m_ViewDirection == Perspective)
	{
		return ((dynamic_cast <PerspectiveView*>(m_View))->getFieldOfView())*180.0f/3.141592653f;
	}
	else
	{
		return PerspectiveView::defaultFieldOfView;
	}
}

double OpenGL_Viewer::GridSpacing() const
{
	return m_Grid->GetGridSpacing();
}

double OpenGL_Viewer::GridExtent() const
{
	return m_Grid->GetGridSize();
}

unsigned int OpenGL_Viewer::MajorSpacing() const
{
	return m_Grid->GetMajorSpacing();
}

bool OpenGL_Viewer::bShowAxis() const
{
	return m_Grid->GetShowAxis();
}

bool OpenGL_Viewer::bShowGrid() const
{
	return m_bShowGrid;
}

void OpenGL_Viewer::renderMe()
{
	glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int error = glGetError();
	prepareTransformation();
	if (m_bShowGrid)
	{
		CCVTexMolGeometryRenderer::GeometryRenderer geometryRenderer;
		Plane p; // dummy
		geometryRenderer.render(m_Grid->GetMajorLines(), 0, "Smooth", p, false);
	}
}

void OpenGL_Viewer::protectedSetBackground(float r, float g, float b)
{
	m_BackgroundColor[0] = r;
	m_BackgroundColor[1] = g;
	m_BackgroundColor[2] = b;
}

void OpenGL_Viewer::getBackgroundColor(float* r, float* g, float* b)
{
	*r = m_BackgroundColor[0];
	*g = m_BackgroundColor[1];
	*b = m_BackgroundColor[2];
}

void OpenGL_Viewer::initializeGL()
{
	glClearColor(m_BackgroundColor[0], m_BackgroundColor[1], m_BackgroundColor[2], 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	initExtensions();
}

void OpenGL_Viewer::prepareTransformation()
{
	m_View->SetView();
	m_ViewFrustrum->update();
}

bool OpenGL_Viewer::enableFirstLight()
{
	GLfloat position[] = {70.0f, 50.0f, 100.0f, 0.0f};
	GLfloat diffuseColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat specularColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat ambientColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glDisable(GL_CULL_FACE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	GLfloat materialSpecularColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialEmissionColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecularColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, materialEmissionColor);
	glEnable(GL_COLOR_MATERIAL);
	return true;
}

bool OpenGL_Viewer::initExtensions()
{
	return true;
}

void OpenGL_Viewer::setViewType(int viewType)
{
	m_ViewType = viewType;
}

void OpenGL_Viewer::setView(const OpenGL_Viewer* openGL_Viewer)
{
	m_View->SetOrientation(*(openGL_Viewer->m_View));
	if (m_ViewDirection != openGL_Viewer->m_ViewDirection)
	{
		m_Grid = openGL_Viewer->m_Grid;
		m_ViewDirection = openGL_Viewer->m_ViewDirection;
	}
}

void OpenGL_Viewer::dontShare()
{
}

void OpenGL_Viewer::shareWith(OpenGL_Viewer* openGL_Viewer)
{
}

View* OpenGL_Viewer::getView()
{
	return m_View;
}

void OpenGL_Viewer::zoom(float zoomValue)
{
	m_View->zoom(zoomValue);
}

void OpenGL_Viewer::setView(View* view)
{
	m_View = view;
}

// Returns a plane parallel to the view plane.
Plane OpenGL_Viewer::getViewPlane()
{
	GLfloat modelview[16], projection[16];
	double combined[16];
	// inverse transform the plane normal using the opengl matrices
	// the view plane is 0.0, 0.0, -1.0, 0.0 in clip coordinate
	// first, get the modelview and projection matrices
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	// then concatinate the projection and modelview matrices
	concatMatrices(combined, modelview, projection);
	// then calculate the plane equation in object space
	// See http://www.opengl.org/developers/faqs/technical/viewcull.c
	// for how this operation was simplified
	Plane plane(
		combined[3]+combined[2],
		combined[7]+combined[6],
		combined[11]+ combined[10],
		0.0);
	plane.normalizeNormal();
	return plane;
}

Plane OpenGL_Viewer::getClipPlane(double* min, double* max, double clippingPlane)
{
	Plane plane = OpenGL_Viewer::getViewPlane();
	double ratiox = max[0] - min[0];
	double ratioy = max[1] - min[1];
	double ratioz = max[2] - min[2];
	// find the maximum ratio
	double largestSide = (ratiox > ratioy ? ratiox : ratioy);
	largestSide = (largestSide > ratioz ? largestSide : ratioz);
	// normalize so the max ratio is 1.0
	ratiox /= largestSide;
	ratioy /= largestSide;
	ratioz /= largestSide;
	double diagonal = sqrt(ratiox*ratiox + ratioy*ratioy + ratioz*ratioz);
	double nearestdist = (-0.5 * diagonal) + (clippingPlane * diagonal);
	plane[3] = nearestdist;
	// generate a point on the plane.
	double x, y, z;
	{
		double a = plane[0], b = plane[1], c = plane[2], d = plane[3];
		if ((a<-0.000000001) || (a>0.000000001))
		{
			y = 1, z = 1;
			x = (d-(b*y+c*z)) / a;
		}
		else if ((b<-0.000000001) || (b>0.000000001))
		{
			x = 1, z = 1;
			y = (d-(a*x+c*z)) / b;
		}
		else if ((c<-0.000000001) || (c>0.000000001))
		{
			x = 1, y = 1;
			z = (d-(a*x+b*y)) / c;
		}
		else
		{
			return plane;    // wierd case!!
		}
	}
	// move the point to 0-1], scale by largest side, translate to real position of volume.
	x += (max[0]-min[0])/(largestSide * 2.0);
	y += (max[1]-min[1])/(largestSide * 2.0);
	z += (max[2]-min[2])/(largestSide * 2.0);
	x *= largestSide;
	y *= largestSide;
	z *= largestSide;
	x += min[0];
	y += min[1];
	z += min[2];
	// update the dist with new point, keep old normal.
	plane[3] = -1*(plane[0]*x + plane[1]*y + plane[2]*z);
	return plane;
}

// static helper function to concat two matrices
void OpenGL_Viewer::concatMatrices(double* result, float* m1, float* m2)
{
	double mb00, mb01, mb02, mb03,
		   mb10, mb11, mb12, mb13,
		   mb20, mb21, mb22, mb23,
		   mb30, mb31, mb32, mb33;
	double mai0, mai1, mai2, mai3;
	mb00 = m2[0];
	mb01 = m2[1];
	mb02 = m2[2];
	mb03 = m2[3];
	mb10 = m2[4];
	mb11 = m2[5];
	mb12 = m2[6];
	mb13 = m2[7];
	mb20 = m2[8];
	mb21 = m2[9];
	mb22 = m2[10];
	mb23 = m2[11];
	mb30 = m2[12];
	mb31 = m2[13];
	mb32 = m2[14];
	mb33 = m2[15];
	for (int i = 0; i < 4; i++)
	{
		mai0 = m1[i*4+0];
		mai1 = m1[i*4+1];
		mai2 = m1[i*4+2];
		mai3 = m1[i*4+3];
		result[i*4+0] = (mai0 * mb00 + mai1 * mb10 + mai2 * mb20 + mai3 * mb30);
		result[i*4+1] = (mai0 * mb01 + mai1 * mb11 + mai2 * mb21 + mai3 * mb31);
		result[i*4+2] = (mai0 * mb02 + mai1 * mb12 + mai2 * mb22 + mai3 * mb32);
		result[i*4+3] = (mai0 * mb03 + mai1 * mb13 + mai2 * mb23 + mai3 * mb33);
	}
}

bool OpenGL_Viewer::printOpenGLError()
{
	GLenum glErr = glGetError();
	if (glErr == GL_NO_ERROR)
	{
		return true;
	}
	while (glErr != GL_NO_ERROR)
	{
		printf("%s\n", (const char*)(gluErrorString(glErr)));
		glErr = glGetError();
	}
	return false;
}

int OpenGL_Viewer::printOpenGLError(char* file, int line)
{
	GLenum glErr;
	int    retCode = 0;
	glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s, line %d is: %s\n", file, line, (const char*)(gluErrorString(glErr)));
		retCode = 1;
		glErr = glGetError();
	}
	return retCode;
}
