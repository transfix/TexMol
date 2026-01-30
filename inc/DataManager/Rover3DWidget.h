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
#if !defined(AFX_ROVER3DWIDGET_H__0728E187_9100_44E6_B363_3FC056575CCB__INCLUDED_)
#define AFX_ROVER3DWIDGET_H__0728E187_9100_44E6_B363_3FC056575CCB__INCLUDED_

#include <DataManager/Axis3DHandler.h>
#include <qobject.h>
#include <OpenGL_Viewer/Extents.h>
#include <Geometry/Geometry.h>

class Rover3DWidget : public QObject
{
		Q_OBJECT
	public:
		Rover3DWidget();
		virtual ~Rover3DWidget();
		enum Axis {XAxis, YAxis, ZAxis, NoAxis};
		void setBoundary(float* min, float* max);
		const Extents& getBoundary() const;
		const Extents& getSubVolume() const;
		void setOrigin(Vector vector);
		void setXScaleNob(Vector value, bool symetric = true);
		void setYScaleNob(Vector value, bool symetric = true);
		void setZScaleNob(Vector value, bool symetric = true);
		void setPXScaleNob(const Vector& value);
		void setPYScaleNob(const Vector& value);
		void setPZScaleNob(const Vector& value);
		void setNXScaleNob(const Vector& value);
		void setNYScaleNob(const Vector& value);
		void setNZScaleNob(const Vector& value);
		void setAspectRatio(double x, double y, double z);
		void setColor(float r, float g, float b);
		virtual void roverDown(Axis3DHandler::Axis axis);
		virtual void roverMoving(Axis3DHandler::Axis axis);
		virtual void roverReleased(Axis3DHandler::Axis axis);
		Geometry* getWireCubes();
		Geometry* getAxes();

	signals:
		void RoverExploring();
		void RoverReleased();

	protected:
		void setCurrentHighlight(Axis3DHandler::Axis axis);
		void prepareGeometry();
		void prepareAxes();
		void prepareWireCubes();
		void updateSubVolume(double subminx, double subminy, double subminz, double submaxx, double submaxy, double submaxz);
		Extents m_Boundary;
		Extents m_SubVolume;
		Axis m_CurrentHighlight;
		bool m_GeometriesAllocated;
		Geometry m_WireCubeGeometry;
		Geometry m_AxesGeometry;
		float m_R,m_G,m_B;
};

#endif
