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
#ifndef CCV_TEXMOL_RENDERER_SET_H
#define CCV_TEXMOL_RENDERER_SET_H

#include <TexMol/Dialogs/RenderAreaWidget.h>
#include <UsefulMath/Vector.h>
#include <UsefulMath/Quaternion.h>
#include <qcolor.h>
//Added by qt3to4:
#include <QLabel>
#include <Q3PtrList>
#include <QGridLayout>

class DataManager;
class MouseHandler;
class QSplitter;
class QLabel;
class LightSet;

class RendererSet
{
	public:
		RendererSet(bool showGrid, QLabel* viewInformationLabel, QLabel* mouseInformationLabel, QLabel* selectionInformationLabel, bool select);
		virtual ~RendererSet();
		bool addNewRenderer(QWidget *parent, QSplitter* viewSplitter, DataManager* dataManager, bool syncView, bool current, MouseHandler* mouseHandler, LightSet* lightSet);
		void syncView(bool sync);
		void splitView(int renderer, QWidget *parent, QSplitter* viewSplitter, DataManager* dataManager, bool syncView, MouseHandler* mouseHandler, LightSet* lightSet);
		void defaultTransformation(int dx, int dy);
		void rotate(int dx, int dy);
		void zoom(int dx, int dy);
		void pan(int dx, int dy);
		void setNewMousePos(int x, int y);
		void resetCurrent();
		void updateGL();
		void showGrids(bool displayGrid);
		void setViewType(int viewType, bool sync);
		void setShiftPressed(bool shiftPressed);
		void setControlPressed(bool controlPressed);
		bool saveImages(QString imageFileName, QString formatSelected, bool saveAll);
		bool saveTiledImages(QString imageFileName, QString formatSelected, int tileResol, int nTiles, bool saveAll, bool mergeToSingelImage);
		bool startRecordingViews(const char* animFileName);
		bool recordCurrentView();
		bool stopRecordingViews();
		bool playbackAnimation(const char* animFileName);
		bool recordAnimation(const char*  animationFileName, QString imageBaseFileName, QString fileType);
		bool recordAxisRotatedAnimation(QString imageBaseFileName, QString fileType, float degree, float x, float y, float z, int seconds);
		void displayMousePosition(int x, int y);
		void displayViewParameters(CCVOpenGLMath::Vector target, CCVOpenGLMath::Quaternion orientation,
								   float windowSize);
		void selectObjectsSlot(bool select);
		void setSelection(QString selection);
		void getBackgroundColor(QColor* color);
		void setBackgroundColor(QColor color);
		bool setViewingParameters(double* translationParams, double* rotationParams, double windowSize);
		bool setViewingParameters(int rendererId, double* translationParams, double* rotationParams, double windowSize);
		bool getViewingParameters(CCVOpenGLMath::Quaternion* orientation, CCVOpenGLMath::Vector* translation, float* windowSize);

		int getNumberOfRenderers();
		bool makeFullScreen(bool maximise, QWidget* qwidget);
		bool makeStereo(bool useStereoMode);
		bool enableFirstLight();
		RenderAreaWidget* getCurrentRenderer();
		int getCurrentRendererID();
		void renderType(bool useRayTracer);
		// not implemented
		virtual bool playbackMovie(QString animationDirName, QString imageBaseFileName, int numberOfImages, int fps);
		bool m_ControlPressed;
		bool m_ShiftPressed;

	protected:
		Q3PtrList<RenderAreaWidget> m_RendererList;
		bool m_ShowGrid;
		bool m_Select;
		QLabel* m_ViewInformationLabel;
		QLabel* m_MouseInformationLabel;
		QLabel* m_SelectionInformationLabel;

		QGridLayout* m_splitViewLayout;
};

#endif
