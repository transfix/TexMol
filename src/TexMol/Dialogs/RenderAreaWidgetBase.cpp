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
	: QGLWidget(parent, name)
{
	if(parent)
	{
	/*Q3Err:CS
		QObjectList* l = parent->queryList("RenderAreaWidgetBase");
		QObjectListIt It(*l);
		QObject* obj;
		
		//m_OpenGL_Viewer.m_GeometryScene = 0;
		while((obj = it.current()) != 0)
		{
			// for each found object...
			++it;

			if(this!=obj)
			{
				((QGLContext*)context())->reset();
				((QGLContext*)context())->create(((RenderAreaWidgetBase*)obj)->context());
				//m_OpenGL_Viewer.m_GeometryScene = ((RenderAreaWidgetBase*)obj)->m_OpenGL_Viewer.m_GeometryScene;
				//m_OpenGL_Viewer.m_GeometryScene->addMe();
				break;
			}
		}
		delete l; // delete the list, not the objects
	*/

#ifdef TEST_MSG_CHA
        cout << "in  RenderAreaWidgetiBase" << endl;
#endif
		QObjectList l = parent->queryList("RenderAreaWidgetBase");

#ifdef TEST_MSG_CHA
        cout << "after  queryList" << endl;
#endif

		if(!l.isEmpty()) {
			QObject* obj	= l.back();
			l.pop_back();
			
			//m_OpenGL_Viewer.m_GeometryScene = 0;
			while(obj != NULL)
			{
				// for each found object...
				if(this!=obj)
				{
					((QGLContext*)context())->reset();
					((QGLContext*)context())->create(((RenderAreaWidgetBase*)obj)->context());
					//m_OpenGL_Viewer.m_GeometryScene = ((RenderAreaWidgetBase*)obj)->m_OpenGL_Viewer.m_GeometryScene;
					//m_OpenGL_Viewer.m_GeometryScene->addMe();
					break;
				}
				if(!l.isEmpty()) {
					obj	= l.back();
					l.pop_back();
				}
				else
					obj	= NULL;
			}
			l.clear();
		}
#ifdef TEST_MSG_CHA
        cout << "in  RenderAreaWidgetiBase" << endl;
#endif
	}
	else
	{
		//m_OpenGL_Viewer.m_GeometryScene = new GeometryScene();
	}

	initParams();
}

RenderAreaWidgetBase::RenderAreaWidgetBase(QGLContext* prevcontext, QWidget* parent, const char* name, const QGLWidget* shareWidget, Qt::WFlags f)
	: QGLWidget(parent, name, shareWidget, f)
{
	//m_GeometryScene = new GeometryScene();
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
	m_OpenGL_Viewer.initializeGL();
	setAutoBufferSwap(true);
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
	updateGL();
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
	updateGL();
}

void RenderAreaWidgetBase::resetFieldOfView()
{
	m_OpenGL_Viewer.resetFieldOfView();
	updateGL();
}

void RenderAreaWidgetBase::setViewDirection(OpenGL_Viewer::Views view)
{
	m_OpenGL_Viewer.setViewDirection(view);
	updateGL();
}

void RenderAreaWidgetBase::resetViewDirection()
{
	m_OpenGL_Viewer.resetViewDirection();
	updateGL();
}

void RenderAreaWidgetBase::setVolumeNearPlane(int nearPlane)
{
	int clamped = (nearPlane>0 ? nearPlane : 0);
	clamped = (clamped<99 ? clamped: 99);
	// SKVINAY another place where it is not cleanly separated
	//m_VolumeRenderer.setNearPlane((double)clamped/99.0);
	updateGL();
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
	updateGL();
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
	((QGLContext*)context())->reset();
	((QGLContext*)context())->create(RenderAreaWidgetBase->context());
	glInit();
}

void RenderAreaWidgetBase::redraw()
{
	updateGL();
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
	m_OpenGL_Viewer.zoom(-q->delta()/3);
	updateGL();
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
			reparent(m_savedParentWidget, 0, QPoint(0, 0), true);
			m_savedParentWidget = NULL;
		} 
	}
}
