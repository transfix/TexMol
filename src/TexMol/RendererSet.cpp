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
#include <DataManager/DataManager.h>
#include <QGridLayout>
#include <qlabel.h>
#include <qsplitter.h>
#include <TexMol/MouseHandlerHelper.h>
#include <TexMol/RendererSet.h>

// #define TEST_MSG_CHA

RendererSet::RendererSet(bool showGrid, QLabel* viewInformationLabel, QLabel* mouseInformationLabel, QLabel* selectionInformationLabel, bool select)
{
	m_ShowGrid = showGrid;
	m_RendererList.setAutoDelete(true);
	m_ViewInformationLabel = viewInformationLabel;
	m_MouseInformationLabel = mouseInformationLabel;
	m_SelectionInformationLabel = selectionInformationLabel;
	m_Select = select;
	m_ShiftPressed = false;
	m_ControlPressed = false;
	m_splitViewLayout = NULL;
}

RendererSet::~RendererSet()
{
	m_RendererList.clear();
}

bool RendererSet::addNewRenderer(QWidget *parent, QSplitter* viewSplitter, DataManager* dataManager, bool syncView, bool current, MouseHandler* mouseHandler, LightSet* lightSet)
{

	QWidget *newWindow = new QWidget();
	if( m_splitViewLayout == NULL )
		m_splitViewLayout = new QGridLayout( viewSplitter );

	RenderAreaWidget* renderer = new RenderAreaWidget((QWidget*)newWindow,"Renderer", this, syncView, current, 0, m_ShowGrid, mouseHandler, lightSet);

	m_splitViewLayout->addWidget( renderer );

	renderer->setDataManager(dataManager);
	m_RendererList.append(renderer);
	QColor color;
	getBackgroundColor(&color);
	setBackgroundColor(color);
	return true;
}

void RendererSet::updateGL()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->updateGL();
	}
}

void RendererSet::defaultTransformation(int x, int y)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->defaultTransformation(x, y);
	}
}

void RendererSet::rotate(int x, int y)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->rotate(x, y);
	}
}

void RendererSet::zoom(int x, int y)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->zoom(x, y);
	}
}

void RendererSet::pan(int x, int y)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->pan(x, y);
	}
}

void RendererSet::setNewMousePos(int x, int y)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->setNewMousePos(x, y);
	}
}

// Display the current renderer's mouse position in the status bar.
void RendererSet::displayMousePosition(int x, int y)
{
	char displayString[256];
	sprintf(displayString, "X = %d, Y = %d", x, y);
	m_MouseInformationLabel->setText(displayString);
}

//  Display the current renderer's view parameters in the status bar.
void RendererSet::displayViewParameters(CCVOpenGLMath::Vector target, CCVOpenGLMath::Quaternion orientation, float windowSize)
{
	char displayString[512];
	sprintf(displayString, "View: T=[%8.3f %8.3f %8.3f %8.3f], O=[%5.3f %5.3f %5.3f %5.3f], W=[%7.2f]",
			target[0], target[1], target[2], target[3],
			orientation[0], orientation[1], orientation[2], orientation[3],
			windowSize);
	m_ViewInformationLabel->setText(displayString);
}

void RendererSet::syncView(bool sync)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->setSync(sync);
	}
	if (sync)
	{
		if (m_RendererList.count() < 2)
		{
			return;
		}
		// get view of current renderer
		int c;
		View* view = NULL;
		for (int c=0; c<m_RendererList.count(); c++)
		{
			if ((m_RendererList.at(c))->isCurrent())
			{
				view = (m_RendererList.at(c))->getView();
			}
		}
		if (view == NULL)
		{
			return;
		}
		for (int c=0; c<m_RendererList.count(); c++)
		{
			if (!(m_RendererList.at(c))->isCurrent())
			{
				(m_RendererList.at(c))->setView(view);
			}
		}
	}
	else
	{
		int c;
		for (int c=0; c<m_RendererList.count(); c++)
		{
			(m_RendererList.at(c))->cloneView();
		}
	}
}

void RendererSet::resetCurrent()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->resetCurrent();
	}
}

void RendererSet::splitView(int renderer, QWidget *parent, QSplitter* viewSplitter, DataManager* dataManager, bool syncView, MouseHandler* mouseHandler, LightSet* lightSet)
{
	if (m_RendererList.count() < 1)
	{
		return;
	}

	RenderAreaWidget* r = m_RendererList.at(0);
	QGLContext* prevcontext = (QGLContext*)((RenderAreaWidgetBase*)r)->context();
	
        QWidget *newWindow = new QWidget();

	RenderAreaWidget* renderer2 = new RenderAreaWidget(prevcontext, (QWidget*)r,"Renderer", NULL, 0, this, syncView, false, m_RendererList.count(), m_ShowGrid, mouseHandler, lightSet);
//	RenderAreaWidget* renderer2 = new RenderAreaWidget((QWidget*)newWindow,"Renderer", this, syncView, false, m_RendererList.count(), m_ShowGrid, mouseHandler, lightSet);
//	RenderAreaWidget* renderer2 = new RenderAreaWidget((QWidget*)r, "Renderer", this, syncView, false, m_RendererList.count(), m_ShowGrid, mouseHandler, lightSet);

	m_splitViewLayout->addWidget( renderer2 );

	renderer2->setDataManager(dataManager);
	QColor color;
	getBackgroundColor(&color);
	m_RendererList.append(renderer2);
	renderer2->show();
	setBackgroundColor(color);
}

void RendererSet::showGrids(bool displayGrid)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->showGrids(displayGrid);
	}
	m_ShowGrid = displayGrid;
}

void RendererSet::setViewType(int viewType, bool sync)
{
	// if sync is on, create new view for selected renderer
	//                share it among all, could create for anyone!
	// else create new view for all
	//
	// issue:
	//      How to make orthographic mode and perspective mode to
	//	    visually correspond ?
	if (sync)
	{
		// set view of current renderer
		View* view = NULL;
		for (int c=0; c<m_RendererList.count(); c++)
		{
			if ((m_RendererList.at(c))->isCurrent())
			{
				(m_RendererList.at(c))->m_OpenGL_Viewer.setViewType(viewType);
				(m_RendererList.at(c))->m_OpenGL_Viewer.initView();
				view = (m_RendererList.at(c))->m_OpenGL_Viewer.getView();
			}
		}
		if (view == NULL)
		{
			return;
		}
		for (int c=0; c<m_RendererList.count(); c++)
		{
			if (!(m_RendererList.at(c))->isCurrent())
			{
				(m_RendererList.at(c))->m_OpenGL_Viewer.setView(view);
			}
		}
	}
	else
	{
		for (int c=0; c<m_RendererList.count(); c++)
		{
			(m_RendererList.at(c))->m_OpenGL_Viewer.setViewType(viewType);
			(m_RendererList.at(c))->m_OpenGL_Viewer.initView();
		}
	}
}

void RendererSet::setShiftPressed(bool shiftPressed)
{
	m_ShiftPressed = shiftPressed;
}

void RendererSet::setControlPressed(bool controlPressed)
{
	m_ControlPressed = controlPressed;
}

bool RendererSet::saveImages(QString baseImageFileName, QString formatSelected, bool saveAll)
{
	bool ret = true;
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if (saveAll)
		{
			QString imageFileName = baseImageFileName + QString::number(c) + "." + formatSelected;
			ret &= (m_RendererList.at(c))->saveImages(imageFileName, saveAll, formatSelected);
		}
		else if ((m_RendererList.at(c))->isCurrent())
		{
			QString imageFileName = baseImageFileName + "." + formatSelected;
			ret &= (m_RendererList.at(c))->saveImages(imageFileName, saveAll, formatSelected);
		}
	}
	return ret;
}

bool RendererSet::saveTiledImages(QString baseImageFileName, QString formatSelected, int tileResol, int nTiles, bool saveAll, bool mergeToSingleImage)
{
	bool ret = true;
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if (saveAll)
		{
			ret &= (m_RendererList.at(c))->saveTiledImages(baseImageFileName, saveAll, formatSelected, tileResol, nTiles, mergeToSingleImage);
		}
		else if ((m_RendererList.at(c))->isCurrent())
		{
			ret &= (m_RendererList.at(c))->saveTiledImages(baseImageFileName, saveAll, formatSelected, tileResol, nTiles, mergeToSingleImage);
		}
	}
	return ret;
}


bool RendererSet::startRecordingViews(const char* animFileName)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->startRecordingViews(animFileName);
		}
	}
	return false;
}

bool RendererSet::recordCurrentView()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->recordCurrentView();
		}
	}
	return false;
}

bool RendererSet::stopRecordingViews()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->stopRecordingViews(); // its ok to do this for all views...
	}
	return true;
}

bool RendererSet::playbackAnimation(const char* animFileName)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->playbackAnimation(animFileName);
		}
	}
	return false;
}

bool RendererSet::recordAnimation(const char*  animationFileName, QString imageBaseFileName, QString fileType)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->recordAnimation(animationFileName, imageBaseFileName, fileType);
		}
	}
	return false;
}

bool RendererSet::recordAxisRotatedAnimation(QString imageBaseFileName, QString fileType, float degree, float x, float y, float z, int seconds)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->recordAxisRotatedAnimation(imageBaseFileName, fileType, degree, x, y, z, seconds);

		}
	}
	return false;
}

// not implemented
bool RendererSet::playbackMovie(QString animationDirName, QString imageBaseFileName, int numberOfImages, int fps)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return (m_RendererList.at(c))->playbackMovie(animationDirName, imageBaseFileName, numberOfImages, fps);
		}
	}
	return false;
}

void RendererSet::selectObjectsSlot(bool select)
{
	m_Select = select;
}

void RendererSet::setSelection(QString selection)
{
	if (!m_Select)
	{
		return;
	}
	if (!m_SelectionInformationLabel)
	{
		return;    // nothing to do if I dont know where to put the results.
	}
	m_SelectionInformationLabel->setText(selection);
}

void RendererSet::getBackgroundColor(QColor* color)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			(m_RendererList.at(c))->getBackgroundColor(color);
			return;
		}
	}
	color->setRgb(0, 0, 0);
	return;
}

void RendererSet::setBackgroundColor(QColor color)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->setBackgroundColor(color);
	}
}

//  Set the viewing parameters to current renderer
bool RendererSet::setViewingParameters(double* translationParams, double* rotationParams, double windowSize)
{
	if (!translationParams || !rotationParams)
	{
		return false;
	}
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return(m_RendererList.at(c))->setViewingParameters(translationParams, rotationParams, windowSize);
		}
	}
	return false;
}

//  Set the viewing parameters to the selected renderer
bool RendererSet::setViewingParameters(int rendererId, double* translationParams, double* rotationParams, double windowSize)
{
	if (!translationParams || !rotationParams)
	{
		return false;
	}
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isRenderer(rendererId))
		{
			return (m_RendererList.at(c))->setViewingParameters(translationParams, rotationParams, windowSize);
		}
	}
	return true;
}

// Get the viewing parameters of the current renderer
bool RendererSet::getViewingParameters(CCVOpenGLMath::Quaternion* orientation, CCVOpenGLMath::Vector* translation, float* windowSize)
{
	if (!orientation || !translation || !windowSize)
	{
		return false;
	}
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return(m_RendererList.at(c))->getViewingParameters(orientation, translation, windowSize);
		}
	}
	return false;
}

int RendererSet::getNumberOfRenderers()
{
	return m_RendererList.count();
}

bool RendererSet::makeFullScreen(bool maximise, QWidget* qwidget)
{
	if (getNumberOfRenderers() != 1)
	{
		return false;
	}
	if (maximise)
	{
		(m_RendererList.at(0))->reparent(NULL, Qt::WType_TopLevel, QPoint(0, 0), true);  // You may want other widget flags/positions...
		(m_RendererList.at(0))->saveParentWidget( qwidget );
		(m_RendererList.at(0))->showFullScreen();
	}
	else
	{
		(m_RendererList.at(0))->showNormal();
		(m_RendererList.at(0))->reparent(qwidget, 0, QPoint(0, 0), true);  // You may want other widget flags/positions...
	}
	return true;
}

bool RendererSet::makeStereo(bool useStereoMode)
{
	if (getNumberOfRenderers() != 1)
	{
		return false;
	}
	(m_RendererList.at(0))->makeStereo(useStereoMode);
	return true;
}

bool RendererSet::enableFirstLight()
{
	if (getNumberOfRenderers() < 1)
	{
		return false;
	}
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->enableFirstLight();
	}
	return true;
}

RenderAreaWidget* RendererSet::getCurrentRenderer()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return m_RendererList.at(c);
		}
	}
	return 0;
}


int RendererSet::getCurrentRendererID()
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		if ((m_RendererList.at(c))->isCurrent())
		{
			return c;
		}
	}
	return -1;
}

void RendererSet::renderType(bool useRayTracer)
{
	for (int c=0; c<m_RendererList.count(); c++)
	{
		(m_RendererList.at(c))->renderType(useRayTracer);
	}
}
