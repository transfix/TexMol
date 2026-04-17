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
#include <DataManager/Axis3DHandler.h>
#include <DataManager/Rover3DWidget.h>
#include <math.h>
#include <OpenGL_Viewer/View.h>
#include <qevent.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <UsefulMath/Ray.h>

using CCVOpenGLMath::Vector;

// Construction/Destruction
Axis3DHandler::Axis3DHandler(Rover3DWidget* rover3DWidget, Axis3DHandler::Axis axis)
	: m_Rover3DWidget(rover3DWidget), m_Axis(axis)
{
	m_MouseDown = false;
}

Axis3DHandler::~Axis3DHandler()
{
}

Axis3DHandler* Axis3DHandler::clone() const
{
	return new Axis3DHandler(*this);
}

bool Axis3DHandler::mousePress3DEvent(View* view, QMouseEvent* e)
{
	Vector origin = m_Rover3DWidget->getSubVolume().getOrigin();
	float distance = nearestTOnAxis(view, e);
	Vector point = nearestPointOnAxis(view, e);
	Ray deletepickRay = view->GetPickRay(e->position().x(), e->position().y());
	Vector screenPoint = view->GetScreenPoint(point);
	bool isValid1 = m_Rover3DWidget->getSubVolume().withinCube(point);
	bool isValid2 = closeToMouse(view, e, point);
	bool isValid = m_Rover3DWidget->getSubVolume().withinCube(point) && closeToMouse(view, e, point);
	if (closeToMouse(view, e, getPNob()))
	{
		m_MouseDown = true;
		m_Section = PScaleNob;
		m_PositionOnAxis = point - origin;
		m_Rover3DWidget->roverDown(m_Axis);
		return true;
	}
	else if (closeToMouse(view, e, getNNob()))
	{
		m_MouseDown = true;
		m_Section = NScaleNob;
		m_PositionOnAxis = point - origin;
		m_Rover3DWidget->roverDown(m_Axis);
		return true;
	}
	else if (isValid)
	{
		m_MouseDown = true;
		m_Section = Middle;
		m_PositionOnAxis = point - origin;
		m_Rover3DWidget->roverDown(m_Axis);
		return true;
	}
	else
	{
		m_MouseDown = false;
		return false;
	}
}

bool Axis3DHandler::mouseRelease3DEvent(View* view, QMouseEvent* e)
{
	if (m_MouseDown)
	{
		m_MouseDown = false;
		m_Rover3DWidget->roverReleased(m_Axis);
		return true;
	}
	return false;
}

bool Axis3DHandler::mouseDoubleClick3DEvent(View*, QMouseEvent* e)
{
	// ignore this for now
	return false;
}

bool Axis3DHandler::mouseMove3DEvent(View* view, QMouseEvent* e)
{
	if (m_MouseDown)
	{
		Vector point = nearestPointOnAxis(view, e);
		if (m_Section==PScaleNob)
		{
			setPScaleNob(point);
			m_Rover3DWidget->roverMoving(m_Axis);
			return true;
		}
		else if (m_Section==NScaleNob)
		{
			setNScaleNob(point);
			m_Rover3DWidget->roverMoving(m_Axis);
			return true;
		}
		else   // middle
		{
			m_Rover3DWidget->setOrigin(point-m_PositionOnAxis);
			m_Rover3DWidget->roverMoving(m_Axis);
			return true;
		}
	}
	else
	{
		return false;
	}
}

float Axis3DHandler::getNearestClickDistance(View* view, QMouseEvent* e)
{
	float distance = nearestTOnAxis(view, e);
	Vector point = nearestPointOnAxis(view, e);
	bool isValid = m_Rover3DWidget->getSubVolume().withinCube(point) && closeToMouse(view, e, point);
	if (closeToMouse(view, e, getPNob()))
	{
		return distance;
	}
	else if (closeToMouse(view, e, getNNob()))
	{
		return distance;
	}
	else if (isValid)
	{
		return distance;
	}
	else
	{
		return -1.0;
	}
}

Axis3DHandler::Axis Axis3DHandler::getAxis() const
{
	return m_Axis;
}

bool Axis3DHandler::closeToMouse(View* view, QMouseEvent* e, const Vector& v)
{
	Vector screenPoint = view->GetScreenPoint(v);
	screenPoint[1] = view->m_Height - screenPoint[1] - 1; // SKVINAY
	double d1 = fabs((float)(screenPoint[0]-(float)e->position().x()));
	double d2 = fabs((float)(screenPoint[1]-(float)e->position().y()));
	return  d1 < 5.0 &&
			d2 < 5.0;
}

float Axis3DHandler::nearestTOnAxis(View* view, QMouseEvent* e) const
{
	Vector origin = m_Rover3DWidget->getSubVolume().getOrigin();
	Ray pickRay = view->GetPickRay(e->position().x(), e->position().y());
	if (m_Axis==XAxis)
	{
		float xDistance = pickRay.nearestTOnXAxis(origin);
		return xDistance;
	}
	else if (m_Axis==YAxis)
	{
		float yDistance = pickRay.nearestTOnYAxis(origin);
		return yDistance;
	}
	else
	{
		float zDistance = pickRay.nearestTOnZAxis(origin);
		return zDistance;
	}
}

Vector Axis3DHandler::nearestPointOnAxis(View* view, QMouseEvent* e) const
{
	Vector origin = m_Rover3DWidget->getSubVolume().getOrigin();
	Ray pickRay = view->GetPickRay(e->position().x(), e->position().y());
	if (m_Axis==XAxis)
	{
		Vector xPoint = pickRay.nearestPointOnXAxis(origin);
		return xPoint;
	}
	else if (m_Axis==YAxis)
	{
		Vector yPoint = pickRay.nearestPointOnYAxis(origin);
		return yPoint;
	}
	else
	{
		Vector zPoint = pickRay.nearestPointOnZAxis(origin);
		return zPoint;
	}
}

void Axis3DHandler::setPScaleNob(const Vector& value) const
{
	if (m_Axis==XAxis)
	{
		m_Rover3DWidget->setXScaleNob(value);
	}
	else if (m_Axis==YAxis)
	{
		m_Rover3DWidget->setYScaleNob(value);
	}
	else
	{
		m_Rover3DWidget->setZScaleNob(value);
	}
}

void Axis3DHandler::setNScaleNob(const Vector& value) const
{
	if (m_Axis==XAxis)
	{
		m_Rover3DWidget->setXScaleNob(value);
	}
	else if (m_Axis==YAxis)
	{
		m_Rover3DWidget->setYScaleNob(value);
	}
	else
	{
		m_Rover3DWidget->setZScaleNob(value);
	}
}

Vector Axis3DHandler::getPNob()
{
	if (m_Axis==XAxis)
	{
		return Vector(
				   (float)m_Rover3DWidget->getSubVolume().getXMax(),
				   (float)((m_Rover3DWidget->getSubVolume().getYMin() + m_Rover3DWidget->getSubVolume().getYMax())/2.0),
				   (float)((m_Rover3DWidget->getSubVolume().getZMin() + m_Rover3DWidget->getSubVolume().getZMax())/2.0),
				   1.0f
			   );
	}
	else if (m_Axis==YAxis)
	{
		return Vector(
				   (float)((m_Rover3DWidget->getSubVolume().getXMin() + m_Rover3DWidget->getSubVolume().getXMax())/2.0),
				   (float)m_Rover3DWidget->getSubVolume().getYMax(),
				   (float)((m_Rover3DWidget->getSubVolume().getZMin() + m_Rover3DWidget->getSubVolume().getZMax())/2.0),
				   1.0f
			   );
	}
	else
	{
		return Vector(
				   (float)((m_Rover3DWidget->getSubVolume().getXMin() + m_Rover3DWidget->getSubVolume().getXMax())/2.0),
				   (float)((m_Rover3DWidget->getSubVolume().getYMin() + m_Rover3DWidget->getSubVolume().getYMax())/2.0),
				   (float)m_Rover3DWidget->getSubVolume().getZMax(),
				   1.0f
			   );
	}
}

Vector Axis3DHandler::getNNob()
{
	if (m_Axis==XAxis)
	{
		return Vector(
				   (float)m_Rover3DWidget->getSubVolume().getXMin(),
				   (float)((m_Rover3DWidget->getSubVolume().getYMin() + m_Rover3DWidget->getSubVolume().getYMax())/2.0),
				   (float)((m_Rover3DWidget->getSubVolume().getZMin() + m_Rover3DWidget->getSubVolume().getZMax())/2.0),
				   1.0f
			   );
	}
	else if (m_Axis==YAxis)
	{
		return Vector(
				   (float)((m_Rover3DWidget->getSubVolume().getXMin() + m_Rover3DWidget->getSubVolume().getXMax())/2.0),
				   (float)m_Rover3DWidget->getSubVolume().getYMin(),
				   (float)((m_Rover3DWidget->getSubVolume().getZMin() + m_Rover3DWidget->getSubVolume().getZMax())/2.0),
				   1.0f
			   );
	}
	else
	{
		return Vector(
				   (float)((m_Rover3DWidget->getSubVolume().getXMin() + m_Rover3DWidget->getSubVolume().getXMax())/2.0),
				   (float)((m_Rover3DWidget->getSubVolume().getYMin() + m_Rover3DWidget->getSubVolume().getYMax())/2.0),
				   (float)m_Rover3DWidget->getSubVolume().getZMin(),
				   1.0f
			   );
	}
}
