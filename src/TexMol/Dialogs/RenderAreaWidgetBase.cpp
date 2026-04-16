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

#include <GL/glew.h>
#include <qcolor.h>
#include <qgl.h>
#include <qobject.h>
//Added by qt3to4:
#include <QWheelEvent>
#include <QMouseEvent>
#include <TexMol/Dialogs/RenderAreaWidgetBase.h>
#include <Geometry/Geometry.h>
#include <Geometry/GeometryScene.h>
#include <math.h>

//#define TEST_MSG_CHA

const float RenderAreaWidgetBase::defaultBackgroundColor[3] = {0.0f, 0.0f, 0.0f};

RenderAreaWidgetBase::RenderAreaWidgetBase(QWidget* parent, const char* name)
	: QGLWidget(parent)
{
	if (name) setObjectName(QString::fromLatin1(name));
	if(parent)
	{
		// Context sharing handled by Qt6 automatically via QOpenGLWidget
	}

	initParams();
}

RenderAreaWidgetBase::RenderAreaWidgetBase(QGLContext* prevcontext, QWidget* parent, const char* name, const QGLWidget* shareWidget, Qt::WindowFlags f)
	: QGLWidget(parent)
{
	if (name) setObjectName(QString::fromLatin1(name));
	initParams();
}

RenderAreaWidgetBase::~RenderAreaWidgetBase()
{
	//m_GeometryScene->deleteMe();
}

void RenderAreaWidgetBase::initParams()
{
	m_MyButtonFlag = 0;
	protectedSetBackground(0.0,0.0,0.0);
	m_OpenGL_Viewer.initParams();
	setMouseTracking(true);
	m_savedParentWidget = NULL;
}

void RenderAreaWidgetBase::initializeGL()
{
	// Initialize GLEW for GL extension function pointers (replaces
	// hand-rolled StaticExtensionPointers and bundled GLEW 1.7.0)
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(err));
	}

	m_OpenGL_Viewer.initializeGL();
	// setAutoBufferSwap removed in Qt6 — QOpenGLWidget always double-buffers
}

void RenderAreaWidgetBase::resizeGL(int w, int h)
{
	m_OpenGL_Viewer.resizeGL(w, h);
}

void RenderAreaWidgetBase::protectedSetBackground(float r, float g, float b)
{
	m_OpenGL_Viewer.protectedSetBackground(r, g, b);
}

void RenderAreaWidgetBase::setBackground(float r, float g, float b)
{
	protectedSetBackground(r,g,b);
	update();
}

void RenderAreaWidgetBase::setBackgroundColor(const QColor& color)
{
	setBackground(color.red()/255.0f, color.green()/255.0f, color.blue()/255.0f);
}

void RenderAreaWidgetBase::getBackgroundColor(float* r, float* g, float* b)
{
	m_OpenGL_Viewer.getBackgroundColor(r, g, b);
}

void RenderAreaWidgetBase::resetBackgroundColor()
{
	setBackground(defaultBackgroundColor[0],
				  defaultBackgroundColor[1],
				  defaultBackgroundColor[2]);
}

void RenderAreaWidgetBase::setFieldOfView(double fieldOfView)
{
	m_OpenGL_Viewer.setFieldOfView(fieldOfView);
	update();
}

void RenderAreaWidgetBase::resetFieldOfView()
{
	m_OpenGL_Viewer.resetFieldOfView();
	update();
}

void RenderAreaWidgetBase::setViewDirection(OpenGL_Viewer::Views view)
{
	m_OpenGL_Viewer.setViewDirection(view);
	update();
}

void RenderAreaWidgetBase::resetViewDirection()
{
	m_OpenGL_Viewer.resetViewDirection();
	update();
}

void RenderAreaWidgetBase::setVolumeNearPlane(int nearPlane)
{
	int clamped = (nearPlane>0 ? nearPlane : 0);
	clamped = (clamped<99 ? clamped: 99);
	// SKVINAY another place where it is not cleanly separated
	//m_VolumeRenderer.setNearPlane((double)clamped/99.0);
	update();
}

QSizePolicy RenderAreaWidgetBase::sizePolicy() const
{
	return QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
}

bool RenderAreaWidgetBase::bShowAxis() const
{
	return m_OpenGL_Viewer.bShowAxis();
}

bool RenderAreaWidgetBase::bShowGrid() const
{
	return m_OpenGL_Viewer.bShowGrid();
}

void RenderAreaWidgetBase::setViewType(int viewType)
{
	m_OpenGL_Viewer.setViewType(viewType);
}

void RenderAreaWidgetBase::setView(const RenderAreaWidgetBase* viewer)
{
	//delete m_View;
	//m_View = viewer->m_View->clone();
	m_OpenGL_Viewer.setView(&(viewer->m_OpenGL_Viewer));
	update();
}

QSize RenderAreaWidgetBase::sizeHint() const
{
	return QSize(300,300);
}

void RenderAreaWidgetBase::dontShare()
{
	m_OpenGL_Viewer.dontShare();
}

void RenderAreaWidgetBase::shareWith(RenderAreaWidgetBase* RenderAreaWidgetBase)
{
	m_OpenGL_Viewer.shareWith(&(RenderAreaWidgetBase->m_OpenGL_Viewer));
	// Context sharing handled by Qt6 automatically
}

void RenderAreaWidgetBase::redraw()
{
	update();
}

/* View related information */

View* RenderAreaWidgetBase::getView()
{
	return m_OpenGL_Viewer.getView();
}

void RenderAreaWidgetBase::setView(View* view)
{
	m_OpenGL_Viewer.setView(view);
}

void RenderAreaWidgetBase::paintGL()
{
	m_OpenGL_Viewer.renderMe();
}

void RenderAreaWidgetBase::mouseMoveEvent(QMouseEvent* q)
{
}

void RenderAreaWidgetBase::mousePressEvent(QMouseEvent* q)
{
}

void RenderAreaWidgetBase::mouseReleaseEvent(QMouseEvent* q)
{
	m_MyButtonFlag = 0;
}

void RenderAreaWidgetBase::wheelEvent(QWheelEvent* q)
{
	m_OpenGL_Viewer.zoom(-q->angleDelta().y()/3);
	update();
}

void RenderAreaWidgetBase::saveParentWidget( QWidget *widget )
{
	m_savedParentWidget = widget;
}

void RenderAreaWidgetBase::keyPressEvent( QKeyEvent *e )
{
	const Qt::KeyboardModifiers modifiers = e->modifiers();

        if( (e->key() == Qt::Key_Escape) && (modifiers == Qt::NoButton) )
	{
		if( m_savedParentWidget )
		{
			showNormal();
			setParent(m_savedParentWidget);
			move(0, 0);
			show();
			m_savedParentWidget = NULL;
		} 
	}
}
