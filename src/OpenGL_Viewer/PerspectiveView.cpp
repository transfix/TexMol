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
#include <OpenGL_Viewer/PerspectiveView.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Ray.h>
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

using namespace CCVOpenGLMath;

const float PerspectiveView::defaultFieldOfView = 30.0f;

PerspectiveView::PerspectiveView(float windowsize) : View(windowsize)
{
	m_FieldOfView = defaultFieldOfView/180.0f*3.1415926f;
}

PerspectiveView::~PerspectiveView()
{
}

View* PerspectiveView::clone() const
{
	return new PerspectiveView(*this);
}

CCVOpenGLMath::Ray PerspectiveView::GetPickRay(int x, int y) const
{
	y = InvertY(y);
	float fx = (float)x-(float)m_Width/2.0f;
	float fy = (float)y-(float)m_Height/2.0f;
	float objectDx = fx * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	float objectDy = fy * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	float distance;
	distance = m_WindowSize / 2.0f / (float)tan(m_FieldOfView/2.0f);
	CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix::translation(0.0f, 0.0f, distance);
	matrix.preMultiplication(m_Orientation.buildMatrix());
	matrix.preMultiplication(CCVOpenGLMath::Matrix::translation(m_Target));
	return CCVOpenGLMath::Ray(matrix*CCVOpenGLMath::Vector(0.0f,0.0f,0.0f,1.0f), matrix*CCVOpenGLMath::Vector(objectDx,objectDy,-distance,0.0f));
}

CCVOpenGLMath::Vector PerspectiveView::GetScreenPoint(const CCVOpenGLMath::Vector& p) const
{
	float distance = m_WindowSize / 2.0f / (float)tan(m_FieldOfView/2.0f);
	CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix::translation(-m_Target);
	matrix.preMultiplication(m_Orientation.inverse().buildMatrix());
	matrix.preMultiplication(CCVOpenGLMath::Matrix::translation(0.0f, 0.0f, -distance));
	CCVOpenGLMath::Vector result = matrix*p;
	result[0] = result[0] * distance / -result[2];
	result[1] = result[1] * distance / -result[2];
	result[2] = 0.0;
	result[3] = 1.0;
	float scale = 1.0f / (m_WindowSize / (m_Height<m_Width?m_Height: m_Width));
	result[0] = result[0] / (m_WindowSize / (m_Height<m_Width?m_Height: m_Width));
	result[1] = result[1] / (m_WindowSize / (m_Height<m_Width?m_Height: m_Width));
	result[0] = (float)result[0]+(float)m_Width/2.0f;
	result[1] = (float)result[1]+(float)m_Height/2.0f;
	result[1] = (float)InvertY((int)(result[1]));
	return result;
}

void PerspectiveView::SetView()
{
	if( m_flagTiledViewMode )
	{
		const double nearPlane = m_WindowSize*0.1; //0.1;

		double Hnear	= m_WindowSize;
		double nearDist	= Hnear / 2.0f / (float)tan(m_FieldOfView/2.0f);
		Hnear		*= nearPlane / nearDist;
		double Wnear	= Hnear * ( (double)m_Width / (double)m_Height );
	
		double cx	= Wnear*0.5;	
		double cy	= Hnear*0.5;	
		double dx	= Wnear / (double)m_nTilesX;
		double dy	= Hnear / (double)m_nTilesY;
		double left	= dx * m_curTileX	- cx;
		double right	= dx * (m_curTileX + 1)	- cx;
		double bottom	= dy * m_curTileY	- cy;
		double top	= dy * (m_curTileY + 1)	- cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum( left, right, bottom, top, nearPlane, 1400000.0 );

		CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix::translation(-m_Target);
		matrix.preMultiplication(m_Orientation.conjugate().buildMatrix());
		matrix.preMultiplication(CCVOpenGLMath::Matrix::translation(0.0f, 0.0f, -nearDist));

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matrix.getMatrix());
	}
	else
	{
		float distance;
		double nearPlane = m_WindowSize*0.1; //0.1;
		distance = m_WindowSize / 2.0f / (float)tan(m_FieldOfView/2.0f);
		CCVOpenGLMath::Matrix matrix = CCVOpenGLMath::Matrix::translation(-m_Target);
		matrix.preMultiplication(m_Orientation.conjugate().buildMatrix());
		matrix.preMultiplication(CCVOpenGLMath::Matrix::translation(0.0f, 0.0f, -distance));
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (m_Height>m_Width)
		{
			float angle = 2.0f*atan((double)m_Height / (double)m_Width * tan(m_FieldOfView / 2.0));
			gluPerspective(angle*180.0/(3.1415925), (double)m_Width / (double)m_Height, nearPlane, 1400000.0);
		}
		else
		{
			gluPerspective(m_FieldOfView *180.0/(3.1415925), (double)m_Width / (double)m_Height, nearPlane, 1400000.0);
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matrix.getMatrix());
	}
}


void PerspectiveView::defaultTransformation(int xNew, int yNew)
{
	mouseTrackBallRotate(xNew, yNew);
}

void PerspectiveView::setFieldOfView(float angle)
{
	m_FieldOfView = angle;
}

float PerspectiveView::getFieldOfView()
{
	return m_FieldOfView;
}

void PerspectiveView::updateCamera()
{
}

Ray* PerspectiveView::getPrimaryRay(int xScreenPos, int yScreenPos)
{
	return 0;
}
