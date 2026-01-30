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
// SecondaryStructureData.cpp: implementation of the SecondaryStructureData class.

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qpushbutton.h>

#ifdef CVC_USE_QSA
#include <qsproject.h>
#include <qsinterpreter.h>
#endif //CVC_USE_QSA

#include <qimage.h>
#include <qgl.h>
#include <QColorDialog>	//by cha

#include "ui_SecondaryStructureRenderingPropertiesBase.h"

#include <DataManager/SecondaryStructureDataManager/SecondaryStructureData.h>
#include <DataManager/SecondaryStructureDataManager/SecondaryStructureRenderingProperties.h>
#include <DataManager/DataManager.h>
#include <Geometry/Geometry.h>
#include <GeometryFileTypes/GeometryLoader.h>
#include <GeometryRenderer/GeometryRenderer.h>
#include <C2C/c2c_codec.h>
#include <C2C/ContourGeom.h>
#include <UsefulMath/AreaVolumes.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <Curvatures/Curvature.h>
#include <TexMol/Dialogs/ScriptsEditorDialog.h>
#include <UsefulMath/Quaternion.h>
#include <UsefulMath/Vector.h>
#include <stdio.h>

#if defined(__APPLE__)
#undef Q_WS_MAC
#endif

#ifdef WITH_CGAL   
#ifdef WITH_SECSTRUCT
#include <SecondaryStructures/skel.h>
#endif
#endif

#include <Histogram/histogram.h>

SecondaryStructureData::SecondaryStructureData(DataManager* dataManager, Geometry* inputGeom)
	: AbstractData(dataManager)
{
	m_FileName = "";
	m_ClippingPlane = 0;
	m_SecondaryStructureRenderingProperties = 0;
	m_RenderCurvatures = false;
	m_GeometryRenderer = new CCVTexMolGeometryRenderer::GeometryRenderer();
	m_SheetGeometry = 0;
	m_HelixGeometry = 0;
	m_Skeleton = 0;

	if(dataManager)
	{
		m_DataManager = dataManager;
		m_SecondaryStructureRenderingProperties = new SecondaryStructureRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_SecondaryStructureRenderingProperties->hide();
		
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_RenderCheckBox), SIGNAL(toggled(bool)), this, SLOT(renderEnabledSlot(bool)));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox), SIGNAL(clicked()), this, SLOT(renderTrianglesSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox), SIGNAL(clicked()), this, SLOT(renderLinesSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorCheckBox), SIGNAL(clicked()), this, SLOT(secondaryStructureColorTypeSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_LineColorCheckBox), SIGNAL(clicked()), this, SLOT(lineColorTypeSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorPushButton), SIGNAL(clicked()), this, SLOT(secondaryStructureColorSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_LineColorPushButton), SIGNAL(clicked()), this, SLOT(lineColorSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_LineThicknessSpinBox), SIGNAL(valueChanged(int)), this, SLOT(lineThicknessChangedSlot(int)));
		// initialize alpha to 1.0f
		m_SecondaryStructureAlpha = 1.0f;
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_TransparencySlider), SIGNAL(valueChanged(int)), this, SLOT(transparencySliderSlot(int)));
		// initialize spin boxes
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox), SIGNAL(valueChanged(int)), this, SLOT(alphaSpinBoxSlot(int)));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox), SIGNAL(valueChanged(int)), this, SLOT(betaSpinBoxSlot(int)));
		// initialize check boxes
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_RenderAlphaCheckBox), SIGNAL(clicked()), this, SLOT(renderAlphaCheckBoxSlot()));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->_ui->m_RenderBetaCheckBox), SIGNAL(clicked()), this, SLOT(renderBetaCheckBoxSlot()));
		// initialize histogram
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->m_AlphaHistogram), SIGNAL(valueChanged(float, float)), this, SLOT(alphaHistogramChangedSlot(float,float)));
		connect((const class QObject*)(m_SecondaryStructureRenderingProperties->m_BetaHistogram), SIGNAL(valueChanged(float, float)), this, SLOT(betaHistogramChangedSlot(float,float)));
	}

#ifdef WITH_CGAL   
#ifdef WITH_SECSTRUCT
	// build the skel
	m_Skeleton = new Skel();
	m_Skeleton->compute_secondary_structures(inputGeom);
	HistogramData alphaData = m_Skeleton->get_alpha_histogram_data();
	m_SecondaryStructureRenderingProperties->m_AlphaHistogram->setData(alphaData);
	HistogramData betaData = m_Skeleton->get_beta_histogram_data();
	m_SecondaryStructureRenderingProperties->m_BetaHistogram->setData(betaData);
	// set the default alpha/beta values
	m_SpinBoxesInitialized = false;
	m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox->setValue(m_Skeleton->getDefaultAlphaCount());
	m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox->setValue(m_Skeleton->getDefaultBetaCount());
	m_SpinBoxesInitialized = true;
	// need to compute helices/sheets the first time
	rebuildGeometry(true, true);
#endif
#endif
}

SecondaryStructureData::~SecondaryStructureData()
{
	if(m_HelixGeometry)
	{
		delete m_HelixGeometry;
	}

	if(m_SheetGeometry)
	{
		delete m_SheetGeometry;
	}

	delete m_GeometryRenderer;
	m_GeometryRenderer = 0;
}

QWidget* SecondaryStructureData::getPropertiesWidget()
{
	return m_SecondaryStructureRenderingProperties;
}

void SecondaryStructureData::setVisibilityInPropertiesWidget()
{
	if(m_SecondaryStructureRenderingProperties)
	{
		m_SecondaryStructureRenderingProperties->_ui->m_RenderCheckBox->setChecked(renderEnabled[m_DataManager->getCurrentRenderer()]);
	}
}

QString SecondaryStructureData::getSelection(CCVOpenGLMath::Ray targetVector)
{
	return 0;
}

bool SecondaryStructureData::read(QStringList fileNames)
{
	return false;
}

float* SecondaryStructureData::getSecondaryStructureColors()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return 0;    // should still render.
	}

	if(m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorCheckBox->isChecked())
	{
		QColor q = getUserSelectedSecondaryStructureColor();
		m_SecondaryStructureColors[0] = q.red() / 255.0;
		m_SecondaryStructureColors[1] = q.green() / 255.0;
		m_SecondaryStructureColors[2] = q.blue() / 255.0;
		return m_SecondaryStructureColors;
	}

	return 0;
}

void SecondaryStructureData::setSecondaryStructureMode(bool  render)
{
	m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox->setChecked(render);
}

bool SecondaryStructureData::save(const char* filename)
{
	return false;
}

void SecondaryStructureData::secondaryStructureColorSlot()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return;
	}

	if(!m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorCheckBox->isChecked())
	{
		return;
	}

	QColor color = QColorDialog::getColor(getUserSelectedSecondaryStructureColor());

	if(color.isValid())
	{
		setUserSelectedSecondaryStructureColor(color);
	}
}

void SecondaryStructureData::lineColorSlot()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return;
	}

	QColor color = QColorDialog::getColor(getUserSelectedLineColor());

	if(color.isValid())
	{
		m_SecondaryStructureRenderingProperties->_ui->m_LineColorPushButton->setPaletteBackgroundColor(color);
		m_SheetGeometry->m_UniqueLineColors[0] = color.red()/255.0;
		m_SheetGeometry->m_UniqueLineColors[1] = color.green()/255.0;
		m_SheetGeometry->m_UniqueLineColors[2] = color.blue()/255.0;
	}
}

const QColor& SecondaryStructureData::getUserSelectedSecondaryStructureColor()
{
	return m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorPushButton->paletteBackgroundColor();
}

const QColor& SecondaryStructureData::getUserSelectedLineColor()
{
	return m_SecondaryStructureRenderingProperties->_ui->m_LineColorPushButton->paletteBackgroundColor();
}

void SecondaryStructureData::setUserSelectedSecondaryStructureColor(const QColor& color)
{
	m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorPushButton->setPaletteBackgroundColor(color);
}

bool SecondaryStructureData::supportsFileNames(QStringList fileNames)
{
	return false;
}

bool SecondaryStructureData::getMinMax(float* min, float* max)
{
	return false;
}

void SecondaryStructureData::renderOnce()
{
	bool showHelices = m_SecondaryStructureRenderingProperties->_ui->m_RenderAlphaCheckBox->isChecked();
	bool showSheets = m_SecondaryStructureRenderingProperties->_ui->m_RenderBetaCheckBox->isChecked();

	if(showHelices)
	{
		m_GeometryRenderer->render(m_HelixGeometry,getSecondaryStructureColors(),"SMOOTH",Plane(),false, m_SecondaryStructureAlpha);
	}

	if(showSheets)
	{
		m_GeometryRenderer->render(m_SheetGeometry,getSecondaryStructureColors(),"SMOOTH",Plane(),false, m_SecondaryStructureAlpha);
	}
}

void SecondaryStructureData::render()
{
	if(!m_HelixGeometry && !m_SheetGeometry)
	{
		return;
	}

	// transparent render, build display list and register with GeometryRenderer
	// bool transparentRender = (m_SecondaryStructureAlpha < 1.0f);
	// GLuint secondaryStructureList;
	// if(transparentRender) {
	//         secondaryStructureList = glGenLists(1);
	//         glNewList(secondaryStructureList, GL_COMPILE);
	// }
	if(m_ReplicationMatrices.size() > 0)
	{
		int i;

		for(i=0; i<m_ReplicationMatrices.size(); i++)
		{
			CCVOpenGLMath::Matrix* transformation = m_ReplicationMatrices[i];

			if(!transformation)
			{
				continue;
			}

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glMultMatrixf(transformation->getMatrix());
			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		renderOnce();
	}

	// if(transparentRender) {
	//         glEndList();
	//         CCVTexMolGeometryRenderer::GeometryRenderer::addTransparentDisplayList(secondaryStructureList);
	// }
}

void SecondaryStructureData::enableScript()
{
#ifdef CVC_USE_QSA

	if(!m_DataManager)
	{
		return;
	}

	if(!m_DataManager->getQSProject())
	{
		return;
	}

	this->setName("secondaryStructure");
	m_DataManager->getQSProject()->addObject(this);
	QString n = this->name();
	ScriptEditor* edit = new ScriptEditor(m_DataManager->getQSProject());

	if(edit->exec() == QDialog::Accepted)
	{
		QString script = edit->code();

		if(script.length())
		{
			m_DataManager->getQSProject()->interpreter()->evaluate(script);
		}
	}

	delete edit;
#endif // CVC_USE_QSA
}

bool SecondaryStructureData::parseAnimationCommand(QStringList commands, int curCommand)
{
	return false;
}

void SecondaryStructureData::renderTrianglesSlot()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return;
	}

	if(m_SheetGeometry)
	{
		m_SheetGeometry->m_RenderTriangles = m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox->isChecked();
	}

	if(m_HelixGeometry)
	{
		m_HelixGeometry->m_RenderTriangles = m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox->isChecked();
	}

	updateGL();
}


void SecondaryStructureData::renderLinesSlot()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return;
	}

	if(m_SheetGeometry)
	{
		m_SheetGeometry->m_RenderLines = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
		m_SheetGeometry->m_RenderPoints = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
	}

	if(m_HelixGeometry)
	{
		m_HelixGeometry->m_RenderLines = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
		m_HelixGeometry->m_RenderPoints = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
	}

	updateGL();
}

void SecondaryStructureData::secondaryStructureColorTypeSlot()
{
	if(!m_SecondaryStructureRenderingProperties)
	{
		return;
	}

	if(m_HelixGeometry)
	{
		m_HelixGeometry->m_UseTriangleColors = m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorCheckBox->isChecked();
	}

	if(m_SheetGeometry)
	{
		m_SheetGeometry->m_UseTriangleColors = m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureColorCheckBox->isChecked();
	}

	updateGL();
}

void SecondaryStructureData::lineColorTypeSlot()
{
	if(m_HelixGeometry)
	{
		m_HelixGeometry->m_UseLineColors = m_SecondaryStructureRenderingProperties->_ui->m_LineColorCheckBox->isChecked();
	}

	if(m_SheetGeometry)
	{
		m_SheetGeometry->m_UseLineColors = m_SecondaryStructureRenderingProperties->_ui->m_LineColorCheckBox->isChecked();
	}

	updateGL();
}

void SecondaryStructureData::lineThicknessChangedSlot(int thickness)
{
	if(m_HelixGeometry)
	{
		m_HelixGeometry->SetLineWidth(thickness);
		m_HelixGeometry->m_PointSize = thickness;
	}

	updateGL();
}

void SecondaryStructureData::transparencySliderSlot(int value)
{
	// set the alpha component of SecondaryStructureColor
	m_SecondaryStructureAlpha = value/100.0f;
	// updateGL so user can track how the alpha changes look
	updateGL();
}

void SecondaryStructureData::rebuildGeometry(bool alphaHistogramChanged, bool betaHistogramChanged)
{
#ifdef WITH_CGAL
#ifdef WITH_SECSTRUCT
	// don't recurse
	m_SpinBoxesInitialized = false;
	{
		if(!m_Skeleton)
		{
			return;
		}

		// when we change the helix histogram, set the cylinders widget to max so we can see the updates
		if(alphaHistogramChanged)
		{
			int max =  m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox->maxValue();
			m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox->setValue(max);
		}

		// when we change the sheet histogram, set the sheet widget to max so we can see the updates
		if(betaHistogramChanged)
		{
			int max =  m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox->maxValue();
			m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox->setValue(max);
		}

		// don't worry about leak, the DataManagers will clean it up
		Geometry* helixGeom = new Geometry();
		Geometry* sheetGeom = new Geometry();
		const int alphaValue = m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox->value();
		const int betaValue = m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox->value();
		const float alphaMinWidth =  m_SecondaryStructureRenderingProperties->m_AlphaHistogram->getMinWidth();
		const float alphaMaxWidth =  m_SecondaryStructureRenderingProperties->m_AlphaHistogram->getMaxWidth();
		const float betaMinWidth =  m_SecondaryStructureRenderingProperties->m_BetaHistogram->getMinWidth();
		const float betaMaxWidth =  m_SecondaryStructureRenderingProperties->m_BetaHistogram->getMaxWidth();
		// here we ask the SS code to rebuild the helix and sheet geometry
		m_Skeleton->update_display(helixGeom, sheetGeom, alphaValue, betaValue, alphaMinWidth, alphaMaxWidth, betaMinWidth, betaMaxWidth, alphaHistogramChanged);
		m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureAlphaSpinBox->setValue(m_Skeleton->getAlphaCount());
		m_SecondaryStructureRenderingProperties->_ui->m_SecondaryStructureBetaSpinBox->setValue(m_Skeleton->getBetaCount());

		if(m_HelixGeometry)
		{
			delete m_HelixGeometry;
		}

		m_HelixGeometry = helixGeom;

		if(m_SheetGeometry)
		{
			delete m_SheetGeometry;
		}

		m_SheetGeometry = sheetGeom;

		if(!m_SecondaryStructureRenderingProperties)
		{
			return;
		}

		if(m_HelixGeometry)
		{
			m_HelixGeometry->m_RenderTriangles = m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox->isChecked();
			m_HelixGeometry->m_RenderLines = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
			m_HelixGeometry->m_RenderPoints = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
		}

		if(m_SheetGeometry)
		{
			m_SheetGeometry->m_RenderTriangles = m_SecondaryStructureRenderingProperties->_ui->m_RenderSecondaryStructureCheckBox->isChecked();
			m_SheetGeometry->m_RenderLines = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
			m_SheetGeometry->m_RenderPoints = m_SecondaryStructureRenderingProperties->_ui->m_RenderLinesCheckBox->isChecked();
		}
	}
	m_SpinBoxesInitialized = true;
	updateGL();
#endif
#endif
}

void SecondaryStructureData::alphaSpinBoxSlot(int value)
{
	if(m_SpinBoxesInitialized)
	{
		rebuildGeometry(false,false);
	}
}

void SecondaryStructureData::betaSpinBoxSlot(int value)
{
	if(m_SpinBoxesInitialized)
	{
		rebuildGeometry(false,false);
	}
}

void SecondaryStructureData::renderAlphaCheckBoxSlot()
{
	rebuildGeometry(false,false);
}

void SecondaryStructureData::renderBetaCheckBoxSlot()
{
	rebuildGeometry(false,false);
}

void SecondaryStructureData::alphaHistogramChangedSlot(float minValue, float maxValue)
{
	rebuildGeometry(true,false);
}

void SecondaryStructureData::betaHistogramChangedSlot(float minValue, float maxValue)
{
	rebuildGeometry(false,true);
}

Geometry* SecondaryStructureData::getSkeletonGeometry()
{
#ifdef WITH_CGAL
#ifdef WITH_SECSTRUCT
	return m_Skeleton->getSkeletonGeometry();
#else
	return NULL;
#endif
#else
	return NULL;
#endif
}
