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
#if !defined(AFX_AXIS3DHANDLER_H__5BC573E6_A739_4530_9799_87571210E0E7__INCLUDED_)
#define AFX_AXIS3DHANDLER_H__5BC573E6_A739_4530_9799_87571210E0E7__INCLUDED_

#include <UsefulMath/Vector.h>
//Added by qt3to4:
#include <QMouseEvent>

class Rover3DWidget;
class View;
class QMouseEvent;
using CCVOpenGLMath::Vector;

class Axis3DHandler
{
	public:
		enum Axis {XAxis, YAxis, ZAxis};
		enum Section {NScaleNob, Middle, PScaleNob};
		Axis3DHandler(Rover3DWidget* rover3DWidget, Axis axis);
		virtual ~Axis3DHandler();
		virtual Axis3DHandler* clone() const;
		virtual bool mousePress3DEvent(View*, QMouseEvent* e);
		virtual bool mouseRelease3DEvent(View*, QMouseEvent* e);
		virtual bool mouseDoubleClick3DEvent(View*, QMouseEvent* e);
		virtual bool mouseMove3DEvent(View*, QMouseEvent* e);
		virtual float getNearestClickDistance(View*, QMouseEvent* e);
		Axis getAxis() const;

	protected:
		bool closeToMouse(View* view, QMouseEvent* e, const Vector& vector);
		float nearestTOnAxis(View* view, QMouseEvent* e) const;
		Vector nearestPointOnAxis(View* view, QMouseEvent* e) const;
		void setPScaleNob(const Vector& value) const;
		void setNScaleNob(const Vector& value) const;
		Vector getPNob();
		Vector getNNob();
		Rover3DWidget* const m_Rover3DWidget;
		const Axis m_Axis;
		bool m_MouseDown;
		Section m_Section;
		Vector m_PositionOnAxis;
};

#endif
