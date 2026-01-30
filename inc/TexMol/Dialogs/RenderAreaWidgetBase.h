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
#ifndef RENDER_AREA_WIDGET_BASE_H
#define RENDER_AREA_WIDGET_BASE_H

#include <qgl.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QMouseEvent>
#include <Geometry/Geometry.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>

class QColor;

class RenderAreaWidgetBase : public QGLWidget
{
		Q_OBJECT

	public:
		RenderAreaWidgetBase(QWidget* parent, const char* name);
		RenderAreaWidgetBase(QGLContext* context, QWidget* parent, const char* name, const QGLWidget* shareWidget, Qt::WFlags f);
		~RenderAreaWidgetBase();
		void setBackground(float r, float g, float b);
		void getBackgroundColor(float* r, float* g, float* b);
		enum Axes { XAxis, YAxis, ZAxis, XScaleNob, YScaleNob, ZScaleNob, NoAxis };

	public slots:
		void setBackgroundColor(const QColor& color);
		void resetBackgroundColor();
		void setFieldOfView(double fieldOfView);
		void resetFieldOfView();
		void setViewDirection(OpenGL_Viewer::Views view);
		void resetViewDirection();
		void setVolumeNearPlane(int nearPlane);

	public:
		virtual QSizePolicy sizePolicy() const;
		bool bShowAxis() const;
		bool bShowGrid() const;
		void setView(const RenderAreaWidgetBase* viewer);
		static const float defaultBackgroundColor[3];
		virtual QSize sizeHint() const;
		void dontShare();
		void shareWith(RenderAreaWidgetBase* RenderAreaWidgetBase);
		void redraw();
		void setViewType(int viewType);
		virtual View* getView();
		virtual void setView(View* view);
		OpenGL_Viewer m_OpenGL_Viewer;
		void saveParentWidget( QWidget *widget );

	signals:
		void explorerMoving();
		void explorerRelease();

	protected:
		void initParams();
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
		void mouseMoveEvent(QMouseEvent* q);
		void mousePressEvent(QMouseEvent* q);
		void mouseReleaseEvent(QMouseEvent* q);
		void wheelEvent(QWheelEvent* q);
		void protectedSetBackground(float r, float g, float b);
		void initGrid();
		float m_OldMouseXPosition;
		float m_OldMouseYPosition;
		int m_MyButtonFlag;
		void keyPressEvent(QKeyEvent *e);
		QWidget *m_savedParentWidget; // for full screen mode
};

#endif
