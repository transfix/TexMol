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
#ifndef RENDER_AREA_WIDGET_H
#define RENDER_AREA_WIDGET_H

#include <qstring.h>
#include <q3textstream.h>
#include <q3ptrlist.h>
#include <qcolor.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <TexMol/Dialogs/RenderAreaWidgetBase.h>

class DataManager;
class RendererSet;
class MouseHandler;

namespace CCVRayTracer
{
	class RayTracer;
};

class LightSet;

namespace CCVOpenGLMath
{
	class Quaternion;
	class Vector;
};

class RenderAreaWidget : public RenderAreaWidgetBase
{
	public:
		RenderAreaWidget(QWidget* parent, const char* name, RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet);
		RenderAreaWidget(QGLContext* prevcontext, QWidget* parent, const char* name, const QGLWidget* shareWidget, Qt::WFlags f, RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet);
		void initParams(RendererSet* rendererSet, bool syncView, bool current, unsigned int uniqueId, bool showGrid, MouseHandler* mouseHandler, LightSet* lightSet);
		virtual ~RenderAreaWidget();
		virtual void setDataManager(DataManager* dataManager);
		virtual bool startRecordingViews(const char* animFileName);
		virtual bool recordCurrentView();
		virtual bool stopRecordingViews();
		virtual bool playbackAnimation(const char* animFileName);
		virtual bool recordAnimation(QString animationFileName, QString imageBaseFileName, QString fileType);
		virtual bool recordAxisRotatedAnimation(QString imageBaseFileName, QString fileType, float degree, float x, float y, float z, int seconds);
		virtual bool playbackMovie(QString animationDirName, QString imageBaseFileName, int numberOfImages, int fps); // not implemented
		virtual void setSync(bool syncView);
		virtual void defaultTransformation(int dx, int dy);
		virtual void zoom(int dx, int dy);
		virtual void rotate(int dx, int dy);
		virtual void pan(int dx, int dy);
		virtual void setNewMousePos(int x, int y);
		virtual void resetCurrent();
		virtual void showGrids(bool displayGrid);
		virtual void cloneView();
		virtual bool isCurrent();
		virtual bool saveImages(QString imageFileName, bool saveAll, QString imgFormat);
		virtual bool saveTiledImages(QString imageFileName, bool saveAll, QString imgFormat, int tileWidth, int nTilesW, bool mergeToSingleImage );
		virtual void getBackgroundColor(QColor* color);
		virtual void setBackgroundColor(QColor color);
		virtual bool isRenderer(int rendererId);
		virtual bool setViewingParameters(double* translationParams, double* rotationParams, double windowSize);
		virtual bool getViewingParameters(CCVOpenGLMath::Quaternion* orientation, CCVOpenGLMath::Vector* translation, float* windowSize);
		virtual bool makeStereo(bool useStereoMode);
		virtual bool enableFirstLight();
		virtual void renderType(bool useRayTracer);

	protected:
		void initializeGL();
		void paintGL();
		void mouseMoveEvent(QMouseEvent* q);
		void mousePressEvent(QMouseEvent* q);
		void mouseReleaseEvent(QMouseEvent* q);
		void recordViewInformation();
		void captureNextImage(QString imageBaseFileName, QString fileType);
		bool recordOrPlaybackAnimation(FILE* fp, QString imageBaseFileName, QString fileType);
		bool getNext(FILE* fp, int& t, CCVOpenGLMath::Quaternion& q, CCVOpenGLMath::Vector& v, float& w);

		DataManager* m_DataManager;
		RendererSet* m_RendererSet;
		MouseHandler* m_MouseHandler;
		bool m_SyncView;

		// animation related:
		char m_AnimationFileName[256];
		bool m_Current;
		unsigned int m_UniqueId;
		bool m_StereoMode;
		bool m_UseRayTracer;

		CCVRayTracer::RayTracer* m_RayTracer;
};

#endif
