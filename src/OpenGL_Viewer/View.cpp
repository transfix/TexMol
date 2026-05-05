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
#include <iostream>
#include <math.h>
#include <OpenGL_Viewer/View.h>

#ifdef _WIN32
#include <windows.h>
#endif
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

//#include <GL/gl.h>

View::View(float windowsize)
	: m_Target(0.0f, 0.0f, 0.0f, 1.0f), m_Orientation()
{
	m_WindowSize = windowsize;
	m_bDragStarted = false;
	m_Width = -1;
	m_Height = -1;
	m_hasSavedViewport = false;
	m_viewport[0] = 0;
	m_viewport[1] = 0;
	m_viewport[2] = 0;
	m_viewport[3] = 0;
	m_flagTiledViewMode = false;
}

View::~View()
{
}

void View::SetOrientation(const View& view)
{
	m_Orientation = view.m_Orientation;
}

void View::setTarget(const CCVOpenGLMath::Vector& target)
{
	m_Target = target;
	m_TranslatedTarget = target;
}

void View::resizeWindow(int w, int h)
{
	m_Width = w;
	m_Height = h;
}

void View::startDrag(int x, int y)
{
	y = InvertY(y);
	m_bDragStarted = true;
	savePosition(x,y);
}

void View::pan(int xNew, int yNew)
{
	yNew = InvertY(yNew);
	protected_pan((float)(xNew - m_xOld), (float)(yNew - m_yOld));
	savePosition(xNew, yNew);
}

void View::rotate(int xNew, int yNew)
{
	mouseTrackBallRotate(xNew, yNew);
}

void View::mouseWorldAxisRotate(int xNew, int yNew)
{
	yNew = InvertY(yNew);
	rotateWorldAxis((float)(xNew - m_xOld)/100.0f, (float)(yNew - m_yOld)/100.0f);
	savePosition(xNew, yNew);
}

void View::mouseTrackBallRotate(int xNew, int yNew)
{
	yNew = InvertY(yNew);
	rotateTrackBall((float)m_xOld, (float)m_yOld, (float)xNew, (float)yNew);
	savePosition(xNew, yNew);
}

void View::zoom(int xNew, int yNew)
{
	yNew = InvertY(yNew);
	zoom(-(float)(yNew - m_yOld));
	savePosition(xNew, yNew);
}

void View::savePosition(int x, int y)
{
	m_xOld = x;
	m_yOld = y;
}

void View::defaultTransformation(int xNew, int yNew)
{
	pan(xNew, yNew);
}

CCVOpenGLMath::Vector View::getPan(int xDiff, int yDiff)
{
	yDiff *= -1;
	float dx = (float)(xDiff);
	float dy = (float)(yDiff);
	CCVOpenGLMath::Vector up(m_Orientation.applyRotation(CCVOpenGLMath::Vector(0.0f, 1.0f, 0.0f, 0.0f)));
	CCVOpenGLMath::Vector right(m_Orientation.applyRotation(CCVOpenGLMath::Vector(1.0f, 0.0f, 0.0f, 0.0f)));
	float objectDx = dx * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	float objectDy = dy * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	CCVOpenGLMath::Vector displacement(right*objectDx+up*objectDy);
        m_TranslatedTarget -= displacement;
	return displacement;
}

CCVOpenGLMath::Matrix View::getRotation(float xDiff, float yDiff)
{
	yDiff *= -1;
	xDiff /= 100.0f;
	yDiff /= 100.0f;
	CCVOpenGLMath::Matrix m;
	CCVOpenGLMath::Quaternion xRot = CCVOpenGLMath::Quaternion::rotation(-xDiff, 0.0f, 1.0f, 0.0f);
	CCVOpenGLMath::Quaternion yRot = CCVOpenGLMath::Quaternion::rotation(yDiff, 1.0f, 0.0f, 0.0f);
	m.postMultiplication(xRot.buildMatrix().inverse());
	m.postMultiplication(yRot.buildMatrix().inverse());
	return m;
}

CCVOpenGLMath::Matrix View::getRotationTC(float xDiff, float yDiff)
{
	yDiff *= -1;
	xDiff /= 100.0f;
	yDiff /= 100.0f;
	CCVOpenGLMath::Matrix m;
	CCVOpenGLMath::Quaternion xRot = CCVOpenGLMath::Quaternion::rotation(-xDiff, 0.0f, 1.0f, 0.0f);
	CCVOpenGLMath::Quaternion yRot = CCVOpenGLMath::Quaternion::rotation(yDiff, 1.0f, 0.0f, 0.0f);

	m.postMultiplication( CCVOpenGLMath::Matrix::translation( m_Target ) );
	m.postMultiplication(xRot.buildMatrix().inverse());
	m.postMultiplication(yRot.buildMatrix().inverse());
	m.postMultiplication( CCVOpenGLMath::Matrix::translation( -m_Target ) );
	return m;
}

float View::getScale(int xDiff, int yDiff)
{
	if (yDiff > 0)
	{
		return 0.9f;
	}
	return 1.1f;
}

void View::protected_pan(float dx, float dy)
{
	CCVOpenGLMath::Vector up(m_Orientation.applyRotation(CCVOpenGLMath::Vector(0.0f, 1.0f, 0.0f, 0.0f)));
	CCVOpenGLMath::Vector right(m_Orientation.applyRotation(CCVOpenGLMath::Vector(1.0f, 0.0f, 0.0f, 0.0f)));
	float objectDx = dx * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	float objectDy = dy * m_WindowSize / (m_Height<m_Width?m_Height: m_Width);
	CCVOpenGLMath::Vector displacement(right*objectDx+up*objectDy);
	m_Target-=displacement;
}

void View::protected_rotate(float dx, float dy)
{
	m_Orientation.postMultiply(CCVOpenGLMath::Quaternion::rotation(-dx, 0.0f, 1.0f, 0.0f));
	m_Orientation.postMultiply(CCVOpenGLMath::Quaternion::rotation(dy, 1.0f, 0.0f, 0.0f));
}

void View::rotateWorldAxis(float dx, float dy)
{
	m_Orientation.preMultiply(CCVOpenGLMath::Quaternion::rotation(-dx, 0.0f, 0.0f, 1.0f));
	m_Orientation.postMultiply(CCVOpenGLMath::Quaternion::rotation(dy, 1.0f, 0.0f, 0.0f));
}

static float projectToBall(float ballSize, float x, float y)
{
	float d, t, z;
	d = (float)sqrt(x * x + y * y);
	if (d < ballSize * 0.70710678118654752440)    /* Inside sphere. */
	{
		z = (float)sqrt(ballSize * ballSize - d * d);
	}
	else                  /* On hyperbola. */
	{
		t = (float)(ballSize / 1.41421356237309504880);
		z = t * t / d;
	}
	return z;
}

void View::rotateTrackBall(float x1, float y1, float x2, float y2)
{
	float ballSize = m_WindowSize/2.0f;
	// find the eye coordinates of the mouse position
	float objectX1 = (float)(x1-m_Width/2) * m_WindowSize / (float)(m_Height<m_Width?m_Height: m_Width);
	float objectY1 = (float)(y1-m_Height/2) * m_WindowSize / (float)(m_Height<m_Width?m_Height: m_Width);
	float objectX2 = (float)(x2-m_Width/2) * m_WindowSize / (float)(m_Height<m_Width?m_Height: m_Width);
	float objectY2 = (float)(y2-m_Height/2) * m_WindowSize / (float)(m_Height<m_Width?m_Height: m_Width);
	float objectZ1 = projectToBall(ballSize, objectX1, objectY1);
	float objectZ2 = projectToBall(ballSize, objectX2, objectY2);
	CCVOpenGLMath::Vector v1(objectX1, objectY1, objectZ1, 0.0f);
	CCVOpenGLMath::Vector v2(objectX2, objectY2, objectZ2, 0.0f);
	CCVOpenGLMath::Vector axis = v1.cross(v2);
	CCVOpenGLMath::Vector distanceDirection = v2-v1;
	float distanceScalar = distanceDirection.norm() / (2.0f * ballSize);
	// adjust so its not out of range
	if (distanceScalar > 1.0f)
	{
		distanceScalar=1.0f;
	}
	if (distanceScalar < -1.0f)
	{
		distanceScalar=-1.0f;
	}
	float angle = 2.0f * (float)asin(distanceScalar);
	m_Orientation.postMultiply(CCVOpenGLMath::Quaternion::rotation(-angle, axis));
}

void View::zoom(float dz)
{
	float amount = 0.005f;
	float incamount = 0.007f/10.0f;
	for (int c=0; c<10; c++)
	{
		m_WindowSize += m_WindowSize*incamount*(dz);
	}
}

float View::GetWindowSize()
{
	return m_WindowSize;
}


void View::SetWindowSize(float size)
{
	m_WindowSize = size;
}

int View::InvertY(int y) const
{
	return m_Height-y-1;
}

CCVOpenGLMath::Quaternion View::getOrientation() const
{
	return m_Orientation;
}

void View::setOrientation(const CCVOpenGLMath::Quaternion& orientation)
{
	m_Orientation = orientation;
}

CCVOpenGLMath::Vector View::getTarget() const
{
	return m_Target;
}

void View::saveCurrentViewport()
{
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	m_hasSavedViewport = true;
}

void View::resetSavedViewport()
{
	if( m_hasSavedViewport )
		glViewport( m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3] );
}

int View::SetTiledViewON(int _tileResolX, int _nTilesX )
{
	m_tileResolX	= _tileResolX;
	m_tileResolY	= _tileResolX;

	m_nTilesX	= _nTilesX;
	m_nTilesY	= ceil( _nTilesX * ( (double)m_Height / (double)m_Width ) );

	m_curTileX	= 0;
	m_curTileY	= 0;

	m_flagTiledViewMode = true;

        return m_nTilesY;
/*
	double nearPlane = m_WindowSize*0.1; //0.1;

	const double Hnear	= m_WindowSize;
	const double nearDist	= Hnear / 2.0f / (float)tan(m_FieldOfView/2.0f);
	const double Wnear	= Hnear * ( (double)m_Width / (double)m_Height );
	const double angW	= atan( Wnear / nearDist );
	const double angH	= atan( Hnear / nearDist );
	const double uAngW	= angW * 2.0 / (double)_nTilesX;
	const double uAngH	= angH * 2.0 / (double)_nTilesY;
	
	double rotAngW		= -angW + uAngW*(double)_Tile_i + uAngW*0.5;
	double rotAngH		= -angH + uAngH*(double)_Tile_j + uAngH*0.5;

	printf( "%lf %lf\n", rotAngW, rotAngH);

	m_Orientation_back = m_Orientation;
	m_Orientation.rotate( rotAngW , 0, 1, 0 );
	m_Orientation.rotate( rotAngH , 1, 0, 0 );

	m_FieldOfView_back = m_FieldOfView;
	m_FieldOfView	= uAngW;

	m_Width_back	= m_Width;
	m_Height_back	= m_Height;

	m_Width		= _tileResolX;
	m_Height	= _tileResolY;

	m_hasSavedViewProperty	= true;
*/
}

bool View::SetNextTile( int *i, int *j )
{
	if( m_flagTiledViewMode )
	{
		m_curTileX++;
		if( m_curTileX >= m_nTilesX) {
			m_curTileX = 0;
			m_curTileY++;
			if( m_curTileY >= m_nTilesY )
				return false;		
		}
		*i	= m_curTileX;
		*j	= m_curTileY;
		return true;
	}
	else
		return false;
}

void View::SetTiledViewOFF()
{
	m_flagTiledViewMode = false;
}

