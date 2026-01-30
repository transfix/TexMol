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
#define GL_GLEXT_PROTOTYPES

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
//#include <GL/gl.h>
#include <TexMol/Dialogs/glext.h>

#include <DataManager/DataManager.h>
#include <math.h>
#include <OpenGL_Viewer/OrthographicView.h>
#include <OpenGL_Viewer/PerspectiveView.h>
#include <OpenGL_Viewer/MyExtensions.h>
#include <qdatetime.h>
#include <qfile.h>
#include <qimage.h>
#include <qimage.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <RayTracer/RayTracer.h>
#include <TexMol/Animator.h>
#include <TexMol/LightSet.h>
#include <TexMol/MouseHandlerHelper.h>
#include <TexMol/Dialogs/RenderAreaWidget.h>
#include <TexMol/RendererSet.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Vector.h>

using CCVRayTracer::RayTracer;

RenderAreaWidget::RenderAreaWidget(QWidget* parent, const char* name, RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet)
	: RenderAreaWidgetBase(parent, name)
{
	initParams(rendererSet, syncView, current, uniqueId, showGrid, mouseHandler, lightSet);
}

RenderAreaWidget::RenderAreaWidget(QGLContext* prevcontext, QWidget* parent, const char* name, const QGLWidget* shareWidget, Qt::WFlags f, RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet)
	: RenderAreaWidgetBase(prevcontext, parent, name, shareWidget, f)
{
	initParams(rendererSet, syncView, current, uniqueId, showGrid, mouseHandler, lightSet);
}


void RenderAreaWidget::initParams(RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet)
{
	m_DataManager = 0;
	m_RendererSet = rendererSet;
	m_SyncView = syncView;
	m_Current = current;
	m_UniqueId = uniqueId;
	m_OpenGL_Viewer.m_bShowGrid = showGrid;
	m_StereoMode = false;
	m_MouseHandler = mouseHandler;
	m_UseRayTracer = false;
	m_RayTracer = new RayTracer(m_OpenGL_Viewer.m_View, lightSet, m_DataManager);
}

RenderAreaWidget::~RenderAreaWidget()
{
	delete m_RayTracer;
}

void RenderAreaWidget::initializeGL()
{
	RenderAreaWidgetBase::initializeGL();
	RenderAreaWidgetBase::protectedSetBackground(1.0f, 1.0f, 1.0f);
	m_DataManager->init();
	//m_GLImposter.init();
}

void RenderAreaWidget::setDataManager(DataManager* dataManager)
{
	m_DataManager = dataManager;
	m_DataManager->setCurrentRendererId(m_UniqueId);
}

void RenderAreaWidget::paintGL()
{
	{
		static int initDone = false;

		if(!initDone && m_StereoMode)
		{
			/*	QGLFormat fmt; // Double buffered by default
				fmt.setRgba (TRUE);
				fmt.setDepth (TRUE);
				fmt.setAlpha (TRUE);
				fmt.setStereo (TRUE);
				fmt.setDoubleBuffer (TRUE);

				this->setFormat (fmt);*/
			initDone = true;
		}
	}
	// clear previous errors
	{
		int c =0;

		while(glGetError()!=GL_NO_ERROR && c<10)
		{
			c++;
		}
	}

	if(m_StereoMode)
	{
		//glDrawBuffer(GL_BACK_LEFT);
		// push current matrix
		/*glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(0,1,0);
		glEnd();*/
		// add rotation
		RenderAreaWidgetBase::paintGL();
		m_DataManager->render(m_UniqueId);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glRotatef(10.5, 0.0, 1.0, 0.0);
		//glDrawBuffer(GL_BACK_RIGHT);
		RenderAreaWidgetBase::paintGL();
		m_DataManager->render(m_UniqueId);
		glPopMatrix();
		// pop back old matrix
		/*glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
		glVertex3f(1,1,0);
		glVertex3f(0,1,0);
		glEnd();*/
	}
	else
	{
		if(m_UseRayTracer)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glRasterPos2d(-1, -1);
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			RenderAreaWidgetBase::paintGL();
			float* image = m_RayTracer->trace();
			glDrawPixels(m_OpenGL_Viewer.m_View->m_Width,
						 m_OpenGL_Viewer.m_View->m_Height,
						 GL_RGBA,
						 GL_FLOAT,
						 (GLvoid*)image
						);
		}
		else
		{
			RenderAreaWidgetBase::paintGL();	//OpenGL_viewer->renderMe()
			m_DataManager->render(m_UniqueId);
		}
	}

	// test for error
	{
		GLenum error;
		error = glGetError();

		if(error == GL_NO_ERROR)
		{
			//printf("No error\n");
		}
		else
		{
			printf("GL Error: %s\n", (char*)gluErrorString(error));
		}
	}
}

float interpolateFloat(float a, float b, float t)
{
	return a*(1-t) + b*t;
}

CCVOpenGLMath::Quaternion interpolateQuaternion(const CCVOpenGLMath::Quaternion& a, const CCVOpenGLMath::Quaternion& b, float t)
{
	CCVOpenGLMath::Quaternion q = (a.inverse()*b);
	return a*q.power(t);
}

CCVOpenGLMath::Vector interpolateVector(CCVOpenGLMath::Vector a, CCVOpenGLMath::Vector b, float t)
{
	return a*(1-t) + b*t;
}

// We try to return the set of values update view info
// for the next frame ( we do interpolate between frames )
// If we instead find some information not related to
// viewing, we pass it on to the appropriate class to use.
bool RenderAreaWidget::getNext(FILE* fp, int& t, CCVOpenGLMath::Quaternion& q, CCVOpenGLMath::Vector& v, float& w)
{
	char line[256];

	// get next view information, if any commands in between, pass them along
	while(true)
	{
		if(!fgets(line, 255, fp))
		{
			return false;
		}

		// break up the line into a set of strings. Someday convert this to C++ from QT.
		QStringList commands = QStringList::split(" ", QString(line));

		if(strstr(commands[0].latin1(), "DATAMANAGER"))
		{
			// got a command for the DATAMANAGER
			if(m_DataManager)
			{
				m_DataManager->parseAnimationCommand(commands, 1);    // bad luck if it doesnt work, carry on with life
			}
		}
		else
		{
			// assume command is for RenderAreaWidget
			break;
		}
	}

	int n = sscanf(line, "%d %f %f %f %f %f %f %f %f %f\n", &t, &(q[0]), &(q[1]), &(q[2]), &(q[3]), &(v[0]), &(v[1]), &(v[2]), &(v[3]), &w);

	if(n != 10)
	{
		return false;
	}

	return true;
}

// Saves an image from the buffer and increments a counter
void RenderAreaWidget::captureNextImage(QString imageBaseFileName, QString fileType)
{
	static int imageNo = 0;
	char path[256];
	sprintf(path, "%s%05d.%s", imageBaseFileName.latin1(), imageNo, fileType.latin1());
	QImage qimage = grabFrameBuffer();
	bool ret = qimage.save(path, fileType.upper());
	//bool ret = qimage.save( path, "BMP" );
	imageNo++;
}


bool RenderAreaWidget::recordOrPlaybackAnimation(FILE* fp, QString imageBaseFileName, QString fileType)
{
	if(!m_DataManager)
	{
		return false;
	}

	if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
	{
		return false;
	}

	CCVOpenGLMath::Quaternion q1, q2, q;
	CCVOpenGLMath::Vector v1, v2, v;
	int t1, t2, t;
	float w1, w2, w;

	// read two frames to interpolate
	if(!getNext(fp, t1, q1, v1, w1))
	{
		return true;
	}

	if(!getNext(fp, t2, q2, v2, w2))
	{
		return true;
	}

	t = t1;
	int tstep = 30;

	//int tstep = (int) (sqrt((q2[0]-q1[0])*(q2[0]-q1[0]) + (q2[1]-q1[1])*(q2[1]-q1[1]) +
	//						(q2[2]-q1[2])*(q2[2]-q1[2]) + (q2[3]-q1[3])*(q2[3]-q1[3]))/0.001f*7);
	bool mergeBufferAlloc = false;
	int imageNumber = 0;
	unsigned char *mergeBuffer = NULL;

	const float makeLeftEye		= -5.0f/180.0f;
	const float makeRightEye	= 10.0f/180.0f;

	while(1)
	{
		// interpolate values linearly, change to some spline
		double alpha = (double)(t-t1)/(double)(t2-t1);
		q = interpolateQuaternion(q1, q2, alpha);
		v = interpolateVector(v1, v2, alpha);
		w = interpolateFloat(w1, w2, alpha);
		// renderInterpolatedFrame
		m_OpenGL_Viewer.m_View->setOrientation(q);
		m_OpenGL_Viewer.m_View->setTarget(v);
		m_OpenGL_Viewer.m_View->SetWindowSize(w);

		if(m_DataManager->m_RenderingMode == Animator::ANIMATION_STEREO_IMAGES_RECORDING_MODE)
		{
			//================================
			//set left eye view
			q.rotate( makeLeftEye, 0, 1, 0 );
			m_OpenGL_Viewer.m_View->setOrientation(q);

			//rendering
			if(m_SyncView)
				m_RendererSet->updateGL();
			else
				updateGL();

			QImage lqimage = grabFrameBuffer();
			unsigned char *limg = lqimage.bits();

			//================================
			//set right eye view
			q.rotate( makeRightEye, 0, 1, 0 );
			m_OpenGL_Viewer.m_View->setOrientation(q);

			//rendering
			if(m_SyncView)
				m_RendererSet->updateGL();
			else
				updateGL();

			QImage rqimage = grabFrameBuffer();
			unsigned char *rimg = rqimage.bits();

			//================================
			// merge images
			int _h = lqimage.height();
			int _w = rqimage.width();
			QImage::Format _format = lqimage.format();
			int _bytesPerLine = lqimage.bytesPerLine();

			if( mergeBufferAlloc == false )
			{
				mergeBuffer = (unsigned char*) malloc( _h * _bytesPerLine * 2 );
				mergeBufferAlloc = true;
			}
	
			int orig_nItemsPerLine = _bytesPerLine;
			int merge_nItemsPerLine = orig_nItemsPerLine * 2;

			for( int i = 0; i<_h; i++ )
			{
				memcpy( mergeBuffer + i*merge_nItemsPerLine,			  limg + i*orig_nItemsPerLine, _bytesPerLine );
				memcpy( mergeBuffer + i*merge_nItemsPerLine + orig_nItemsPerLine, rimg + i*orig_nItemsPerLine, _bytesPerLine );
			}

			QImage mqimage(mergeBuffer, _w*2, _h, _format);
			char path[512];
			sprintf(path, "%s%05d.%s", imageBaseFileName.latin1(), imageNumber, fileType.latin1());
			if( !mqimage.save(path, fileType.upper()) )
			{
				fprintf( stderr, "Stereo image dump failed\n" ); return false;
			}
			imageNumber++;
		}
		else
		{			
			if(m_SyncView)
				m_RendererSet->updateGL();
			else
				updateGL();

			// either save images or just show them on screen
			if(m_DataManager->m_RenderingMode == Animator::ANIMATION_IMAGES_RECORDING_MODE)
				captureNextImage(imageBaseFileName, fileType);
		}
		// incrementtime in small steps to get smooth animation
		t += tstep;

		while(t >= t2)
		{
			t1 = t2;
			q1 = q2;
			v1 = v2;
			w1 = w2;

			if(!getNext(fp, t2, q2, v2, w2))
			{
				return true;
			}
		}
	}
	return true;
}

bool RenderAreaWidget::recordAxisRotatedAnimation(QString imageBaseFileName, QString fileType, float degree, float x, float y, float z, int seconds)
{
	if(!m_DataManager)
	{
		return false;
	}

	bool mergeBufferAlloc = false;
	unsigned char *mergeBuffer = NULL;

	const int framesPerSecond = 30;
	const int totalNumberOfFrame = framesPerSecond * seconds;
	const int loop = totalNumberOfFrame / 4;
	const float halfDegree = degree * 0.5f;
	// we need four half degree rotation
	const float unitRadian = (halfDegree/180.0f) * 4 / (float) totalNumberOfFrame;

	// renderInterpolatedFrame
	
	const float makeLeftEye		= -5.0f/180.0f;
	const float makeRightEye	= 10.0f/180.0f;
	const float makeCenterView	= -5.0f/180.0f;

	int imageNumber = 0;
	float sign[] = {-1.0f, 1.0f, 1.0f, -1.0f};
 
	for( int j=0; j<4; j++ ) {
	for( int i=0; i<loop; i++ ) {
		//set regular rotation
		CCVOpenGLMath::Quaternion q	= m_OpenGL_Viewer.m_View->getOrientation();
		q.rotate( unitRadian * sign[j], x, y, z );

		//================================
		//set left eye view
		q.postMultiply(CCVOpenGLMath::Quaternion::rotation( makeLeftEye, 0, 1, 0 ));
		m_OpenGL_Viewer.m_View->setOrientation(q);

		//rendering
		if(m_SyncView)
			m_RendererSet->updateGL();
		else
			updateGL();

		QImage lqimage = grabFrameBuffer();
		unsigned char *limg = lqimage.bits();

		//================================
		//set right eye view
		q.postMultiply(CCVOpenGLMath::Quaternion::rotation( makeRightEye, 0, 1, 0 ));
		m_OpenGL_Viewer.m_View->setOrientation(q);

		//rendering
		if(m_SyncView)
			m_RendererSet->updateGL();
		else
			updateGL();

		QImage rqimage = grabFrameBuffer();
		unsigned char *rimg = rqimage.bits();

		q.postMultiply(CCVOpenGLMath::Quaternion::rotation( makeCenterView, 0, 1, 0 ));
		m_OpenGL_Viewer.m_View->setOrientation(q);

		//================================
		// merge images
		int _h = lqimage.height();
		int _w = rqimage.width();
		QImage::Format _format = lqimage.format();
		int _bytesPerLine = lqimage.bytesPerLine();

		if( mergeBufferAlloc == false )
		{
			mergeBuffer = (unsigned char*) malloc( _h * _bytesPerLine * 2 );
			mergeBufferAlloc = true;
		}
	
		int orig_nItemsPerLine = _bytesPerLine;
		int merge_nItemsPerLine = orig_nItemsPerLine * 2;

		for( int i = 0; i<_h; i++ )
		{
			memcpy( mergeBuffer + i*merge_nItemsPerLine,			  limg + i*orig_nItemsPerLine, _bytesPerLine );
			memcpy( mergeBuffer + i*merge_nItemsPerLine + orig_nItemsPerLine, rimg + i*orig_nItemsPerLine, _bytesPerLine );
		}

		QImage mqimage(mergeBuffer, _w*2, _h, _format);
		char path[512];
		sprintf(path, "%s%05d.%s", imageBaseFileName.latin1(), imageNumber, fileType.latin1());
		if( !mqimage.save(path, fileType.upper()) )
		{
			fprintf( stderr, "Stereo image dump failed\n" ); return false;
		}
		imageNumber++;
	}}
}

bool RenderAreaWidget::recordAnimation(QString animationFileName, QString imageBaseFileName, QString fileType)
{
	FILE* fp = fopen(animationFileName, "r");

	if(fp == 0)
	{
		return false;
	}

	bool ret = recordOrPlaybackAnimation(fp, imageBaseFileName, fileType);
	fclose(fp);
	return ret;
}

bool RenderAreaWidget::playbackAnimation(const char* animationFileName)
{
	if(!m_DataManager)
	{
		return false;
	}

	if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
	{
		return false;
	}

	FILE* fp = fopen(animationFileName, "r");

	if(fp == 0)
	{
		return false;
	}

	bool ret = recordOrPlaybackAnimation(fp, NULL, NULL);
	fclose(fp);
	return ret;
}

bool RenderAreaWidget::startRecordingViews(const char* animationFileName)
{
	strcpy(m_AnimationFileName, animationFileName);
	return true;
}

bool RenderAreaWidget::recordCurrentView()
{
	recordViewInformation();
}

bool RenderAreaWidget::stopRecordingViews()
{
	return true;
}

// not yet implemented
bool RenderAreaWidget::playbackMovie(QString animationDirName, QString imageBaseFileName, int numberOfImages, int fps)
{
	if(!m_DataManager)
	{
		return false;
	}

	if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
	{
		return false;
	}

	for(int i=0; i<numberOfImages; i++)
	{
		QString filename = animationDirName + imageBaseFileName + QString("%1").arg(i,5);
	}

	return false;
}

void RenderAreaWidget::setSync(bool syncView)
{
	m_SyncView = syncView;
}

void RenderAreaWidget::defaultTransformation(int dx, int dy)
{
	m_OpenGL_Viewer.m_View->defaultTransformation(dx+m_OldMouseXPosition, dy+m_OldMouseYPosition);

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}
}

void RenderAreaWidget::rotate(int dx, int dy)
{
	m_OpenGL_Viewer.m_View->rotate(dx+m_OldMouseXPosition, dy+m_OldMouseYPosition);

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}
}

void RenderAreaWidget::zoom(int dx, int dy)
{
	m_OpenGL_Viewer.m_View->zoom(dx+m_OldMouseXPosition, dy+m_OldMouseYPosition);

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}
}

void RenderAreaWidget::pan(int dx, int dy)
{
	m_OpenGL_Viewer.m_View->pan(dx+m_OldMouseXPosition, dy+m_OldMouseYPosition);

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}
}

void RenderAreaWidget::setNewMousePos(int x, int y)
{
	m_OldMouseXPosition = x;
	m_OldMouseYPosition = y;
}

// If we are pressing the control key, and a button was pressed down before,
// we transform the selected object. Else we transform the view
void RenderAreaWidget::mouseMoveEvent(QMouseEvent* q)
{
	if(m_DataManager)
	{
		if(m_DataManager->mouseMove3DEvent(m_OpenGL_Viewer.m_View, q))
		{
			return;
		}

		m_RendererSet->updateGL();
	}

	if(m_MyButtonFlag)
	{
		int xChange, yChange;
		xChange = q->x() - m_OldMouseXPosition;
		yChange = q->y() - m_OldMouseYPosition;
		MouseHandler::USER_SELECTED_TRANSFORMATION transformation = m_MouseHandler->getUserSelectedTransformation(
					q,
					m_RendererSet->m_ControlPressed,
					m_RendererSet->m_ShiftPressed,
					m_RendererSet->m_ShiftPressed && m_RendererSet->m_ControlPressed
				);

		switch(transformation)
		{
			case MouseHandler::VIEW_TRANSLATE:
				m_SyncView ? m_RendererSet->pan(xChange, yChange) : pan(xChange, yChange);
				break;
			case MouseHandler::VIEW_ZOOM:
				m_SyncView ? m_RendererSet->zoom(xChange, yChange) : zoom(xChange, yChange);
				break;
			case MouseHandler::VIEW_ROTATE:
				m_SyncView ? m_RendererSet->rotate(xChange, yChange) : rotate(xChange, yChange);
				break;
			case MouseHandler::OBJECT_TRANSLATE:
				m_DataManager->pan(m_OpenGL_Viewer.m_View->getPan(xChange, yChange));
				m_RendererSet->updateGL();
				break;
			case MouseHandler::OBJECT_ZOOM:
				m_DataManager->scale(m_OpenGL_Viewer.m_View->getScale(xChange, yChange));
				m_RendererSet->updateGL();
				break;
			case MouseHandler::OBJECT_ROTATE:
				m_DataManager->rotate(m_OpenGL_Viewer.m_View->getRotation(xChange, yChange));	// rotate based on orientation
		//		m_DataManager->rotate(m_OpenGL_Viewer.m_View->getRotationTC(xChange, yChange));	// rotate based on object center
				m_RendererSet->updateGL();
				break;
			case MouseHandler::OBJECT_SELECT:
				m_SyncView ? m_RendererSet->pan(xChange, yChange) : pan(xChange, yChange);
				m_RendererSet->updateGL();
				break;
			case MouseHandler::NO_TRANSFORM:
				break;
			default:
				break;
		}

		if(m_SyncView)
		{
			m_RendererSet->setNewMousePos(q->x(), q->y());
		}
		else
		{
			setNewMousePos(q->x(), q->y());
		}
	}

	if(m_DataManager)
	{
		if(m_DataManager->m_RenderingMode == Animator::FLY_THROUGH_RECORDING_MODE)
		{
		/*Q3Err:C
			if((q->state() & RightButton) ||
					(q->state() & MidButton) ||
					(q->state() & LeftButton))
		*/
			if((q->state() & Qt::RightButton) ||
					(q->state() & Qt::MidButton) ||
					(q->state() & Qt::LeftButton))
			{
				recordViewInformation();
			}
		}
	}

	if(m_Current)
	{
		m_RendererSet->displayMousePosition(q->x(), q->y());
	}

	if(m_Current)
	{
		m_RendererSet->displayViewParameters(m_OpenGL_Viewer.m_View->getTarget(),
											 m_OpenGL_Viewer.m_View->getOrientation(),
											 m_OpenGL_Viewer.m_View->GetWindowSize());
	}
}

void RenderAreaWidget::mouseReleaseEvent(QMouseEvent* q)
{
	if(m_DataManager)
	{
		if(m_DataManager->mouseRelease3DEvent(m_OpenGL_Viewer.m_View, q))
		{
			return;
		}

		m_RendererSet->updateGL();
	}
}

void RenderAreaWidget::mousePressEvent(QMouseEvent* q)
{
	if(m_DataManager)
	{
		if(m_DataManager->mousePress3DEvent(m_OpenGL_Viewer.m_View, q))
		{
			return;
		}

		m_RendererSet->updateGL();
	}

	// request current status on shift and control presses.
	if(!q)
	{
		return;
	}

	if(m_SyncView)
	{
		m_RendererSet->setNewMousePos(q->x(), q->y());
	}
	else
	{
		setNewMousePos(q->x(), q->y());
	}

	m_OpenGL_Viewer.m_View->startDrag(q->x(), q->y());
	m_MyButtonFlag = 1;

	if(!m_Current)
	{
		m_RendererSet->resetCurrent();
		m_Current = true;

		if(m_DataManager)
		{
			m_DataManager->setCurrentRendererId(m_UniqueId);
			m_DataManager->setVisibilityInPropertiesWidget();
		}
	}

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}

	if(m_DataManager)
	{
		m_RendererSet->setSelection(m_DataManager->getSelection(m_OpenGL_Viewer.m_View->GetPickRay(q->x(), q->y())));
	}
}

void RenderAreaWidget::resetCurrent()
{
	m_Current = false;
}

void RenderAreaWidget::showGrids(bool displayGrid)
{
	m_OpenGL_Viewer.m_bShowGrid = displayGrid;
	updateGL();
}

void RenderAreaWidget::cloneView()
{
	m_OpenGL_Viewer.m_View = m_OpenGL_Viewer.m_View->clone();
}

bool RenderAreaWidget::isCurrent()
{
	return m_Current;
}

bool RenderAreaWidget::saveImages(QString imageFileName, bool saveAll, QString imgFormat)
{
	QImage qimage = grabFrameBuffer();
	qimage.save(imageFileName, imgFormat);
	qimage.save(imageFileName, imgFormat);
	return true;
}

bool RenderAreaWidget::saveTiledImages(QString imageFileName, bool saveAll, QString imgFormat, const int _tileWidth, const int _nTilesW, bool mergeToSingleImage )
{
    m_OpenGL_Viewer.m_View->saveCurrentViewport();
    glPushMatrix();

    // arand, 8-23-2011: you need the following line to build in windows...
#ifdef _WIN32
#define USE_MY_EXTENSION
#endif

#ifdef USE_MY_EXTENSION
    //==============================================================================
    MyExtensions *myExtensions = NULL;
    myExtensions = new MyExtensions();
    if( !myExtensions )
    {
	fprintf( stderr, "my extension allocation fail\n");
	return false;
    }
    if ( !myExtensions->initExtensions("GL_EXT_framebuffer_object ") )
	{
		printf("init extensions fail\n");
		return false;
	}
    //==============================================================================
#else

    std::vector <std::string> extensions;
    char *str = (char*)glGetString(GL_EXTENSIONS);
    char *tok;
    // split extensions
    if(str)
    {
        tok = strtok((char*)str, " ");
        while(tok)
        {
            extensions.push_back(tok);    // put a extension into struct
            tok = strtok(0, " ");         // next token
        }
    }
    const std::string& ext("GL_EXT_framebuffer_object");
    std::vector<std::string>::const_iterator iter = extensions.begin();
    std::vector<std::string>::const_iterator endIter = extensions.end();

    while(iter != endIter)
    {
        if(ext == *iter) {
            cout << "GL_EXT_framebuffer_object is supported" << endl; break; }
        else
            ++iter;
    }
#endif
    //==============================================================================

    printf("tileResol = [%d %d], nTileWidth = %d\n", _tileWidth, _tileWidth, _nTilesW);

    GLuint fboId, color_rb, depth_rb;
#ifdef USE_MY_EXTENSION
    myExtensions->glGenFramebuffersEXT(1, &fboId);
    myExtensions->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

    myExtensions->glGenRenderbuffersEXT(1, &color_rb);
    myExtensions->glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, color_rb);
    //The storage format is RGBA8
    myExtensions->glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, _tileWidth, _tileWidth);
    //Attach color buffer to FBO
    myExtensions->glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, color_rb);

    myExtensions->glGenRenderbuffersEXT(1, &depth_rb);
    myExtensions->glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
    myExtensions->glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _tileWidth, _tileWidth);
    // attach a renderbuffer to depth attachment point
    myExtensions->glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

    myExtensions->glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    //==============================================================================

    myExtensions->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
#else
    glGenFramebuffersEXT(1, &fboId);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);

    glGenRenderbuffersEXT(1, &color_rb);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, color_rb);
    //The storage format is RGBA8
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, _tileWidth, _tileWidth);
    //Attach color buffer to FBO
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, color_rb);

    glGenRenderbuffersEXT(1, &depth_rb);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _tileWidth, _tileWidth);
    // attach a renderbuffer to depth attachment point
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    //==============================================================================

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboId);
#endif

    glDrawBuffer(GL_FRONT_LEFT);
    glReadBuffer(GL_FRONT_LEFT);

    m_OpenGL_Viewer.initializeGL();

    glViewport(0, 0, _tileWidth, _tileWidth);

    unsigned char *buf = new unsigned char[_tileWidth * _tileWidth * 4];
    unsigned char *tbuf = new unsigned char[_tileWidth * _tileWidth * 4];

    int ti = 0, tj = 0;    
    int _nTilesH = m_OpenGL_Viewer.m_View->SetTiledViewON( _tileWidth, _nTilesW );

    unsigned char *fullImage = NULL;

    if( mergeToSingleImage )
       fullImage =  new unsigned char[ _nTilesW * _nTilesH * _tileWidth * _tileWidth * 4 ];

    do {
       updateGL();

       glReadPixels(0, 0, _tileWidth, _tileWidth, GL_RGBA, GL_UNSIGNED_BYTE, buf );

       for( int j = 0; j < _tileWidth; j++ ) {
       for( int i = 0; i < _tileWidth; i++ ) {
          int idx_in = (j * _tileWidth + i) * 4;
          int idx_out = ( (_tileWidth - j - 1) * _tileWidth + i) * 4;

          tbuf[ idx_out ]   = buf[ idx_in + 2];	//B
          tbuf[ idx_out +1] = buf[ idx_in + 1];	//G
          tbuf[ idx_out +2] = buf[ idx_in + 0];	//R
          tbuf[ idx_out +3] = buf[ idx_in + 3];	//A

       }}

       if( mergeToSingleImage )
       {
          for( int j = 0; j < _tileWidth; j++ ) {
          for( int i = 0; i < _tileWidth; i++ ) {

             int idx_full_i = ti * _tileWidth + i;
             int idx_full_j = (_nTilesH - tj -1 ) * _tileWidth + j;
             int idx_full = ( idx_full_j * (_nTilesW * _tileWidth) + idx_full_i ) * 4;
	     int idx_out = ( j * _tileWidth + i ) * 4;
             fullImage[ idx_full   ] = tbuf[ idx_out   ];
             fullImage[ idx_full +1] = tbuf[ idx_out +1];
             fullImage[ idx_full +2] = tbuf[ idx_out +2];
             fullImage[ idx_full +3] = tbuf[ idx_out +3];
          }}
       }
       else
       {
          QImage qimage((uchar*)tbuf, _tileWidth, _tileWidth, QImage::Format_ARGB32);
          char path[512];
          sprintf(path, "%s_%d_%d.%s", imageFileName.latin1(), ti, tj, imgFormat.latin1());
          if( !qimage.save(path, imgFormat) )
              fprintf( stderr, "tiled image dump failed\n" );
          else
              fprintf( stderr, "[%d %d] tile image saved\n", ti, tj );
       }
    }while ( m_OpenGL_Viewer.m_View->SetNextTile( &ti, &tj ) );

    if( mergeToSingleImage )
    {
       QImage qimage((uchar*)fullImage, _nTilesW * _tileWidth, _nTilesH * _tileWidth, QImage::Format_ARGB32);
       char path[512];
       sprintf(path, "%s_full_%dx%d.%s", imageFileName.latin1(), _nTilesW*_tileWidth, _nTilesH*_tileWidth, imgFormat.latin1());
       if( !qimage.save(path, imgFormat) )
           fprintf( stderr, "full tiled image save failed\n" );
       else
           fprintf( stderr, "full tiled image saved\n" );
    }

    m_OpenGL_Viewer.m_View->SetTiledViewOFF();

    delete[] buf;
    delete[] tbuf;

    //Delete resources
#ifdef USE_MY_EXTENSION
    myExtensions->glDeleteRenderbuffersEXT(1, &color_rb);
    myExtensions->glDeleteRenderbuffersEXT(1, &depth_rb);
    //Bind 0, which means render to back buffer, as a result, fb is unbound
    myExtensions->glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    myExtensions->glDeleteFramebuffersEXT(1, &fboId);
    // delete extension
    delete myExtensions;
#else
    glDeleteRenderbuffersEXT(1, &color_rb);
    glDeleteRenderbuffersEXT(1, &depth_rb);
    //Bind 0, which means render to back buffer, as a result, fb is unbound
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDeleteFramebuffersEXT(1, &fboId);
#endif
    glPopMatrix();
    m_OpenGL_Viewer.m_View->resetSavedViewport();
    m_OpenGL_Viewer.m_View->SetView( );

    return true;
}


void RenderAreaWidget::recordViewInformation()
{
	if( m_DataManager->m_RenderingMode != Animator::FLY_THROUGH_RECORDING_MODE ||
	    m_DataManager->m_RenderingMode != Animator::PER_FRAME_RECORDING_MODE )
	{
		return;
	}

	static QTime t;
	static bool started = false;
	printf("%s\n", m_AnimationFileName);

	FILE* fp = fopen(m_AnimationFileName, "a");

	if(fp == 0)
	{
		return;
	}

	if(!started)
	{
		t.start();
		started = true;
	}

	CCVOpenGLMath::Quaternion q = m_OpenGL_Viewer.m_View->getOrientation();
	CCVOpenGLMath::Vector v = m_OpenGL_Viewer.m_View->getTarget();
	float w = m_OpenGL_Viewer.m_View->GetWindowSize();
	fprintf(fp, "%d %f %f %f %f %f %f %f %f %f\n", t.elapsed(),
			q[0], q[1], q[2], q[3], v[0], v[1], v[2], v[3], w);
	fclose(fp);
}

void RenderAreaWidget::getBackgroundColor(QColor* color)
{
	float r, g, b;
	RenderAreaWidgetBase::getBackgroundColor(&r, &g, &b);
	color->setRgb((int)(r*255), (int)(g*255), (int)(b*255));
}

void RenderAreaWidget::setBackgroundColor(QColor color)
{
	{
		int r = color.red();
		int g = color.green();
		int b = color.blue();
	}
	setBackground((color.red())/255.0f, (color.green())/255.0f, (color.blue())/255.0f);
}

bool RenderAreaWidget::isRenderer(int rendererId)
{
	if(m_UniqueId == rendererId)
	{
		return true;
	}

	return false;
}

bool RenderAreaWidget::setViewingParameters(double* translationParams, double* rotationParams, double windowSize)
{
	if(!translationParams || !rotationParams)
	{
		return false;
	}

	// set viewing params to View
	CCVOpenGLMath::Quaternion q;
	CCVOpenGLMath::Vector v;
	float w;
	v[0] = translationParams[0];
	v[1] = translationParams[1];
	v[2] = translationParams[2];
	v[3] = translationParams[3];
	q[0] = rotationParams[0];
	q[1] = rotationParams[1];
	q[2] = rotationParams[2];
	q[3] = rotationParams[3];
	w = windowSize;
	m_OpenGL_Viewer.m_View->setOrientation(q);
	m_OpenGL_Viewer.m_View->setTarget(v);
	m_OpenGL_Viewer.m_View->SetWindowSize(w);

	if(m_SyncView)
	{
		m_RendererSet->updateGL();
	}
	else
	{
		updateGL();
	}

	return true;
}

bool RenderAreaWidget::getViewingParameters(CCVOpenGLMath::Quaternion* orientation, CCVOpenGLMath::Vector* translation, float* windowSize)
{
	if(!orientation || !translation || !windowSize)
	{
		return false;
	}

	(*orientation) = m_OpenGL_Viewer.m_View->getOrientation();
	(*translation) = m_OpenGL_Viewer.m_View->getTarget();
	(*windowSize)  = m_OpenGL_Viewer.m_View->GetWindowSize();
	return true;
}

bool RenderAreaWidget::makeStereo(bool useStereoMode)
{
	m_StereoMode = useStereoMode;
	return true;
}

bool RenderAreaWidget::enableFirstLight()
{
	m_OpenGL_Viewer.enableFirstLight();
	return true;
}

void RenderAreaWidget::renderType(bool useRayTracer)
{
	m_UseRayTracer = useRayTracer;
}
