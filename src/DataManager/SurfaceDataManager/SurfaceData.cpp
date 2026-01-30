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
// SurfaceData.cpp: implementation of the SurfaceData class.

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
//Added by qt3to4:
#include <Q3StrList>
#include <QColorDialog>	//by cha
#include <Q3FileDialog>
#include <QImageWriter>
#include <QPalette>
#include <QStyleFactory>

//#include "../ui_SurfaceRenderingPropertiesBase.h"
#include "ui_SurfaceRenderingPropertiesBase.h"

#include <DataManager/SurfaceDataManager/SurfaceData.h>
#include <DataManager/SurfaceDataManager/SurfaceRenderingProperties.h>
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
#include <SignDistanceFunction_v2/mtxlib.h>
#include <stdio.h>
#if defined(__APPLE__)
#undef Q_WS_MAC
#endif

#define SurfRenderType_Color 0
#define SurfRenderType_Normal 1
#define SurfRenderType_Mean 2
#define SurfRenderType_Gaussian 3
#define SurfRenderType_Texture 4

SurfaceData::SurfaceData(DataManager* dataManager)
	: AbstractData(dataManager)
{
	m_FileName = "";
	m_ClippingPlane = 0;
	m_SurfaceRenderingProperties = 0;
	m_RenderCurvatures = false;
	m_GeometryRenderer = new CCVTexMolGeometryRenderer::GeometryRenderer();
	hasTextureImage = false;

	if(dataManager)
	{
		m_DataManager = dataManager;
		m_SurfaceRenderingProperties = new SurfaceRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_SurfaceRenderingProperties->hide();

		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_CheckBox_Render), SIGNAL(toggled(bool)), this, SLOT(renderEnabledSlot(bool)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderSurface), SIGNAL(clicked()), this, SLOT(renderTrianglesSlot()));
		m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderSurface->setChecked( true );
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderWireframe), SIGNAL(clicked()), this, SLOT(renderWireframesSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderLines), SIGNAL(clicked()), this, SLOT(renderLinesSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderPoints), SIGNAL(clicked()), this, SLOT(renderPointsSlot()));

		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceTexture), SIGNAL(clicked()), this, SLOT(surfaceTexturePushButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedLineColor), SIGNAL(clicked()), this, SLOT(lineUniqueColorRadioButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_DefaultLineColor), SIGNAL(clicked()), this, SLOT(lineDefaultColorRadioButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedPointColor), SIGNAL(clicked()), this, SLOT(pointUniqueColorRadioButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_DefaultPointColor), SIGNAL(clicked()), this, SLOT(pointDefaultColorRadioButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedWireframeColor), SIGNAL(clicked()), this, SLOT(wireframeUniqueColorRadioButtonSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_RadioButton_DefaultWireframeColor), SIGNAL(clicked()), this, SLOT(wireframeDefaultColorRadioButtonSlot()));

		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor), SIGNAL(clicked()), this, SLOT(wireframeColorSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor), SIGNAL(clicked()), this, SLOT(surfaceColorSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor), SIGNAL(clicked()), this, SLOT(lineColorSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor), SIGNAL(clicked()), this, SLOT(pointColorSlot()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_SpinBox_Thickness), SIGNAL(valueChanged(int)), this, SLOT(wireframeThicknessChangedSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_SpinBox_LineThickness), SIGNAL(valueChanged(int)), this, SLOT(lineThicknessChangedSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_SpinBox_PointThickness), SIGNAL(valueChanged(int)), this, SLOT(pointThicknessChangedSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_ComboBox_SurfaceRenderingTypes), SIGNAL(activated(int)), this, SLOT(renderingTypeSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_Slider_Clip), SIGNAL(valueChanged(int)), this, SLOT(clipValueChangedSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_QSA), SIGNAL(clicked()), this, SLOT(enableScript()));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_Slider_Frame), SIGNAL(valueChanged(int)), this, SLOT(frameValueChangedSlot(int)));
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_PushButton_Replicate), SIGNAL(clicked()), this, SLOT(replicateSlot()));

                //initialize color set buttons
		m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->setStyle( QStyleFactory::create("plastique") );
		QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->palette();
//		palette.setColor( QPalette::Normal, QPalette::Button, QColor(230, 255, 230) );
		palette.setColor( QPalette::Normal, QPalette::Button, QColor(128, 98, 98) );
		m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->setPalette( palette );

		m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->setStyle( QStyleFactory::create("plastique") );
		palette = m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->palette();
		palette.setColor( QPalette::Normal, QPalette::Button, QColor(210, 210, 210) );
		m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->setPalette( palette );

		m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->setStyle( QStyleFactory::create("plastique") );
		palette = m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->palette();
		palette.setColor( QPalette::Normal, QPalette::Button, QColor(128, 98, 98) );
		m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->setPalette( palette );

		m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->setStyle( QStyleFactory::create("plastique") );
		palette = m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->palette();
		palette.setColor( QPalette::Normal, QPalette::Button, QColor(128, 98, 98) );
		m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->setPalette( palette );

		// initialize alpha to 1.0f
		m_SurfaceAlpha = 1.0f;
		connect((const class QObject*)(m_SurfaceRenderingProperties->_ui->m_Slider_Transparency), SIGNAL(valueChanged(int)), this, SLOT(transparencySliderSlot(int)));
		/////////////// add the rendering types to the combo box /////////////////
		//GeometryRenderer geometryRenderer;
		//int numberOfRenderingTypes = geometryRenderer.getNumberOfRenderingTypes();
		//int i;
		//for( i=0; i<numberOfRenderingTypes; i++ )
		//	m_SurfaceRenderingProperties->addNewRenderingTypeInOrder( geometryRenderer.getRenderingTypeName(i), i );
		//////////////////////////////////////////////////////////////////////////
		// SKVINAY change this all someday
		m_SurfaceRenderingProperties->addNewRenderingTypeInOrder("Color", SurfRenderType_Color);
		m_SurfaceRenderingProperties->addNewRenderingTypeInOrder("Normal", SurfRenderType_Normal);
		m_SurfaceRenderingProperties->addNewRenderingTypeInOrder("Mean", SurfRenderType_Mean);
		m_SurfaceRenderingProperties->addNewRenderingTypeInOrder("Gaussian", SurfRenderType_Gaussian);
		m_SurfaceRenderingProperties->addNewRenderingTypeInOrder("Texture", SurfRenderType_Texture);
	}
}

void SurfaceData::deleteCurvatures()
{
	{
		std::vector<Geometry*>::iterator iter = m_NormalVectors.begin(), end = m_NormalVectors.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_NormalVectors.clear();
	}
	{
		std::vector<Geometry*>::iterator iter = m_K1CurvatureVectors.begin(), end = m_K1CurvatureVectors.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_K1CurvatureVectors.clear();
	}
	{
		std::vector<Geometry*>::iterator iter = m_K2CurvatureVectors.begin(), end = m_K2CurvatureVectors.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_K2CurvatureVectors.clear();
	}
	{
		std::vector<double*>::iterator iter = m_RadiiOfCurvature.begin(), end = m_RadiiOfCurvature.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_RadiiOfCurvature.clear();
	}
}

void SurfaceData::deleteData()
{
	{
		std::vector<Geometry*>::iterator iter = m_Geometry.begin(), end = m_Geometry.end();

		for(; iter != end; ++iter)
		{
			delete *iter;
		}

		m_Geometry.clear();
	}
	deleteCurvatures();
}

SurfaceData::~SurfaceData()
{
	deleteData();
	delete m_GeometryRenderer;
	m_GeometryRenderer = 0;
}

// Obtain the min max of the geometry and initialize the clipping plane.
// We also search and set normal and curvature vectors here from a file.
void SurfaceData::initNewGeometry()
{
	int numModels = m_Geometry.size();
	int c;
	double eps = 0.000000000001;

	for(c=0; c<numModels; c++)
	{
		double minx = 0, miny = 0, minz = 0;
		double maxx = 0, maxy = 0, maxz = 0;
		Geometry* geometry = m_Geometry[c];

		if(!geometry)
		{
			m_min.push_back(minx);
			m_min.push_back(miny);
			m_min.push_back(minz);
			m_max.push_back(maxx);
			m_max.push_back(maxy);
			m_max.push_back(maxz);
			return;
		}
		else
		{
			geometry->m_RenderTriangles = true;
			geometry->getExtents(&minx, &miny, &minz, &maxx, &maxy, &maxz);
			m_min.push_back(minx);
			m_min.push_back(miny);
			m_min.push_back(minz);
			m_max.push_back(maxx);
			m_max.push_back(maxy);
			m_max.push_back(maxz);
		}

		minx -= eps;
		miny -= eps;
		minz -= eps;
		maxx += eps;
		maxy += eps;
		maxz += eps;
		m_min.push_back(minx);
		m_min.push_back(miny);
		m_min.push_back(minz);
		m_max.push_back(maxx);
		m_max.push_back(maxy);
		m_max.push_back(maxz);
	}

	if(m_SurfaceRenderingProperties)
	{
		clipValueChangedSlot(m_SurfaceRenderingProperties->_ui->m_Slider_Clip->value());
	}
}

void SurfaceData::frameValueChangedSlot(int value)
{
	if(m_Geometry.size() < 1)
	{
		return;
	}

	m_Frame = value;

	if(m_Frame < 0)
	{
		m_Frame = 0;
	}

	if(m_Frame >= m_Geometry.size())
	{
		m_Frame = m_Geometry.size() - 1;
	}

	m_DataManager->updateGL();
}

double dot(double* v1, double* v2, int i)
{
	return v1[i*3+0] * v2[i*3+0] + v1[i*3+1] * v2[i*3+1] + v1[i*3+2] * v2[i*3+2];
}

void SurfaceData::setCurvatureVectors(QStringList curvFileNames)
{
	deleteCurvatures();
	int numModels = curvFileNames.size();

	if(numModels < 1)
	{
		return;
	}

	int i;

	for(i=0; i<numModels; i++)
	{
		if(i>=m_Geometry.size())
		{
			return;
		}

		const char* fileName = (curvFileNames[i]).latin1();

		if(!fileName || !(m_Geometry[i]))
		{
			m_NormalVectors.push_back(0);
			m_K1CurvatureVectors.push_back(0);
			m_K2CurvatureVectors.push_back(0);
			m_RadiiOfCurvature.push_back(0);
			continue;
		}

		// search for a .curv file
		int numberOfValues = 0;
		double* HandK = 0;
		double* normals = 0;
		double* k1Vectors = 0;
		double* k2Vectors = 0;
		double* radiiOfCurvature = 0;
		Curvature::read(fileName, &numberOfValues, &HandK, &normals, &k1Vectors, &k2Vectors);

		if(numberOfValues != m_Geometry[i]->m_NumTriVerts)
		{
			m_NormalVectors.push_back(0);
			m_K1CurvatureVectors.push_back(0);
			m_K2CurvatureVectors.push_back(0);
			m_RadiiOfCurvature.push_back(0);
			continue;
		}

		if(HandK)
		{
			setRadiiOfCurvature(HandK, i);
			delete []HandK;
			HandK = 0;
		}
		else
		{
			m_RadiiOfCurvature.push_back(0);
		}

		double length = 0.01;

		if(normals)
		{
			Geometry* normalVectors = new Geometry();
			setVectorGeometry(normalVectors, numberOfValues, normals, length, i);
		}
		else
		{
			m_NormalVectors.push_back(0);
		}

		if(k1Vectors)
		{
			Geometry* k1CurvatureVectors = new Geometry();
			setVectorGeometry(k1CurvatureVectors, numberOfValues, k1Vectors, length, i);
		}
		else
		{
			m_K1CurvatureVectors.push_back(0);
		}

		if(k2Vectors)
		{
			Geometry* k2CurvatureVectors = new Geometry();
			setVectorGeometry(k2CurvatureVectors, numberOfValues, k2Vectors, length, i);
		}
		else
		{
			m_K2CurvatureVectors.push_back(0);
		}
	}
}

bool SurfaceData::setVectorGeometry(Geometry* geometry, int numberOfVectors, double* vectors, double length, int curGeomIndex)
{
	if(curGeomIndex >= m_Geometry.size() || curGeomIndex <0)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[curGeomIndex];

	if(!curGeometry)
	{
		return false;
	}

	if(!geometry || !vectors)
	{
		return false;
	}

	if(curGeometry->m_NumTriVerts != numberOfVectors)
	{
		return false;
	}

	geometry->AllocateLines(numberOfVectors*2, numberOfVectors);
	int i;

	for(i=0; i<numberOfVectors; i++)
	{
		geometry->m_LineVerts[i*6+0] = curGeometry->m_TriVerts[i*3+0];
		geometry->m_LineVerts[i*6+1] = curGeometry->m_TriVerts[i*3+1];
		geometry->m_LineVerts[i*6+2] = curGeometry->m_TriVerts[i*3+2];
		geometry->m_LineVerts[i*6+3] = curGeometry->m_TriVerts[i*3+0] - length*vectors[i*3+0];
		geometry->m_LineVerts[i*6+4] = curGeometry->m_TriVerts[i*3+1] - length*vectors[i*3+1];
		geometry->m_LineVerts[i*6+5] = curGeometry->m_TriVerts[i*3+2] - length*vectors[i*3+2];
		geometry->m_Lines[i*2+0] = i*2+0;
		geometry->m_Lines[i*2+1] = i*2+1;
	}

	return true;
}

bool SurfaceData::setRadiiOfCurvature(double* HandK, int curGeomIndex)
{
	if(!HandK)
	{
		return false;
	}

	if(curGeomIndex >= m_Geometry.size() || curGeomIndex <0)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[curGeomIndex];

	if(!curGeometry)
	{
		return false;
	}

	double* radiiOfCurvature = new double[curGeometry->m_NumTriVerts*2];
	int i;

	for(i=0; i<curGeometry->m_NumTriVerts; i++)
	{
		double h = HandK[i*2+0];
		double k = HandK[i*2+1];
		double k1, k2;
		double d = h*h-k;

		if(d<0)
		{
			d=0;
		}

		k1 = h-sqrt(d);
		k2 = h+sqrt(d);
		double r1 = -1.0/k1;
		double r2 = -1.0/k2;
		radiiOfCurvature[i*2+0] = r1;
		radiiOfCurvature[i*2+1] = r2;
	}

	m_RadiiOfCurvature.push_back(radiiOfCurvature);
	return true;
}

QWidget* SurfaceData::getPropertiesWidget()
{
	return m_SurfaceRenderingProperties;
}

void SurfaceData::setVisibilityInPropertiesWidget()
{
	if(m_SurfaceRenderingProperties)
	{
		m_SurfaceRenderingProperties->_ui->m_CheckBox_Render->setChecked(renderEnabled[m_DataManager->getCurrentRenderer()]);
	}
}

bool SurfaceData::convertContourGeomToGeometry(ContourGeom* cGeom, Geometry* geometry, bool colorPresent)
{
	if(!cGeom || !geometry)
	{
		return false;
	}

	// need to document this function, very odd function
	geometry->AllocateTris(cGeom->getNVert(), cGeom->getNTri());
	//Copy information to new data structure
	int i;

	for(i=0; i<cGeom->getNTri(); i++)
	{
		geometry->m_Tris[3*i+0] = cGeom->tri[i][0];
		geometry->m_Tris[3*i+1] = cGeom->tri[i][1];
		geometry->m_Tris[3*i+2] = cGeom->tri[i][2];
	}

	for(i=0; i<cGeom->getNVert(); i++)
	{
		geometry->m_TriVerts[3*i+0] = cGeom->vert[i][0];
		geometry->m_TriVerts[3*i+1] = cGeom->vert[i][1];
		geometry->m_TriVerts[3*i+2] = cGeom->vert[i][2];
	}

	if(colorPresent)
	{
		for(i=0; i<cGeom->getNVert(); i++)
		{
			geometry->m_TriVertColorsTransparent[4*i+0] = cGeom->vcol[i][0];
			geometry->m_TriVertColorsTransparent[4*i+1] = cGeom->vcol[i][1];
			geometry->m_TriVertColorsTransparent[4*i+2] = cGeom->vcol[i][2];
		}
	}

	// Done copying
	return true;
}

bool SurfaceData::read(QStringList fileNames)
{
	deleteData();
	int numModels = fileNames.size();

	if(numModels < 1)
	{
		return false;
	}

	QStringList qListCurvFileNames;
	int i;

	for(i=0; i<numModels; i++)
	{
		const char* fileName = (fileNames[i]).latin1();

		if(!fileName)
		{
			m_Geometry.push_back(0);
			continue;
		}

		// Load c2c if filename ends with c2c
		if(strstr(fileName,"c2c") || strstr(fileName, "C2C"))
		{
			bool colorPresent;
			ContourGeom* cGeom = decodeC2CFile(fileName, colorPresent);

			if(!cGeom)
			{
				m_Geometry.push_back(0);
				continue;
			}

			// convert from awkward 'ContourGeom' to newer 'Geometry'
			Geometry* geometry = new Geometry();

			if(!convertContourGeomToGeometry(cGeom, geometry, colorPresent))
			{
				delete cGeom;
				delete geometry;
				m_Geometry.push_back(0);
				continue;
			}

			// Done converting
			delete cGeom;
			m_Geometry.push_back(geometry);
			continue;
		}

		// Load in the new Geometry and set it if possible
		GeometryLoader* geometryLoader = new GeometryLoader();
		Geometry* geometry = geometryLoader->loadFile(fileName);
		delete geometryLoader;

		// Done loading
		if(!geometry)
		{
			m_Geometry.push_back(0);
			continue;
		}

		m_Geometry.push_back(geometry);
		char curvFileName[256];
		strcpy(curvFileName, fileName);
		strcat(curvFileName, ".curv");
		qListCurvFileNames.append(curvFileName);
	}

	setCurvatureVectors(qListCurvFileNames);
	initNewGeometry();

	if(m_SurfaceRenderingProperties)
	{
		if(m_Geometry.size() > 1)
		{
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setMinValue(0);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setMaxValue(m_Geometry.size()-1);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setTickInterval(1);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setEnabled(true);
		}
		else
		{
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setMinValue(0);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setMaxValue(0);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setTickInterval(1);
			m_SurfaceRenderingProperties->_ui->m_Slider_Frame->setEnabled(false);
		}
	}

	m_Frame = 0;

	if(m_Geometry.size() < 1)
	{
		return false;
	}

	return true;
}

void SurfaceData::setGeometry(Geometry* geometry)
{
	deleteData();
	m_Geometry.push_back(geometry);
	initNewGeometry();
}

QString SurfaceData::getSelection(CCVOpenGLMath::Ray targetVector)
{
	return 0;
}

float* SurfaceData::getSurfaceColors()
{
	if(!m_SurfaceRenderingProperties)
	{
		return 0; // Should still render.
	}

	{
		QColor q = getUserSelectedSurfaceColor();
		m_SurfaceColors[0] = q.red() / 255.0;
		m_SurfaceColors[1] = q.green() / 255.0;
		m_SurfaceColors[2] = q.blue() / 255.0;
		return m_SurfaceColors;
	}

	return 0;
}

const char* SurfaceData::getRenderingTypeName()
{
	if(!m_SurfaceRenderingProperties)
	{
		return "Smooth";
	}

	return (m_SurfaceRenderingProperties->_ui->m_ComboBox_SurfaceRenderingTypes->currentText()).latin1();
}

float* SurfaceData::getWireframeColors()
{
	if(!m_SurfaceRenderingProperties)
	{
		return 0; // Should still render.
	}

	if(m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedWireframeColor->isChecked())
	{
		QColor q = getUserSelectedWireframeColor();
		m_WireframeColors[0] = q.red() / 255.0;
		m_WireframeColors[1] = q.green() / 255.0;
		m_WireframeColors[2] = q.blue() / 255.0;
		return m_WireframeColors;
	}

	return 0;
}

int SurfaceData::getWireframeThickness()
{
	if(!m_SurfaceRenderingProperties)
	{
		return 1; // Should still render.
	}

	return m_SurfaceRenderingProperties->_ui->m_SpinBox_Thickness->value();
}

void SurfaceData::setWireFrameMode(bool  render)
{
	m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderWireframe->setChecked(render);
}

void SurfaceData::setSurfaceMode(bool  render)
{
	m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderSurface->setChecked(render);
}

double SurfaceData::getArea()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	double area = 0;
	// Add area of each triangle
	{
		int i;

		for(i=0; i<curGeometry->m_NumTris; i++)
		{
			int v1 = curGeometry->m_Tris[3*i+0];
			int v2 = curGeometry->m_Tris[3*i+1];
			int v3 = curGeometry->m_Tris[3*i+2];
			area += CCVOpenGLMath::AreaVolumes::getTriangleArea(curGeometry->m_TriVerts[3*v1+0],
					curGeometry->m_TriVerts[3*v1+1],
					curGeometry->m_TriVerts[3*v1+2],
					curGeometry->m_TriVerts[3*v2+0],
					curGeometry->m_TriVerts[3*v2+1],
					curGeometry->m_TriVerts[3*v2+2],
					curGeometry->m_TriVerts[3*v3+0],
					curGeometry->m_TriVerts[3*v3+1],
					curGeometry->m_TriVerts[3*v3+2]);
		}
	}
	return area;
}

double SurfaceData::getVolumeN()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	double volume = 0;
	// Add volume of each triangle
	{
		int i;

		for(i=0; i<curGeometry->m_NumTris; i++)
		{
			int v1 = curGeometry->m_Tris[3*i+0];
			int v2 = curGeometry->m_Tris[3*i+1];
			int v3 = curGeometry->m_Tris[3*i+2];
			vector3 V1, V2, V3, V4;
			V1.x =  curGeometry->m_TriVerts[3*v1+0];
			V1.y =  curGeometry->m_TriVerts[3*v1+1];
			V1.z =  curGeometry->m_TriVerts[3*v1+2];
			V2.x =  curGeometry->m_TriVerts[3*v2+0];
			V2.y =  curGeometry->m_TriVerts[3*v2+1];
			V2.z =  curGeometry->m_TriVerts[3*v2+2];
			V3.x =  curGeometry->m_TriVerts[3*v3+0];
			V3.y =  curGeometry->m_TriVerts[3*v3+1];
			V3.z =  curGeometry->m_TriVerts[3*v3+2];
			V4 = CrossProduct(V2, V3);
			volume += DotProduct(V1, V4);
		}
	}
	volume /= 6.0;
	return fabs(volume);
}

double SurfaceData::distVtxToPoint(int vertex, float* point)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	double x = curGeometry->m_TriVerts[3*vertex+0] - point[0];
	double y = curGeometry->m_TriVerts[3*vertex+1] - point[1];
	double z = curGeometry->m_TriVerts[3*vertex+2] - point[2];
	return sqrt(x*x + y*y + z*z);
}

double SurfaceData::getArea(float* point, double radius)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	double area = 0;
	// Add area of each triangle entirely within specified sphere
	{
		int i;

		for(i=0; i<curGeometry->m_NumTris; i++)
		{
			int v1 = curGeometry->m_Tris[3*i+0];
			int v2 = curGeometry->m_Tris[3*i+1];
			int v3 = curGeometry->m_Tris[3*i+2];

			if((distVtxToPoint(v1, point) > radius) ||
					(distVtxToPoint(v2, point) > radius) ||
					(distVtxToPoint(v3, point) > radius))
			{
				continue;
			}

			area += CCVOpenGLMath::AreaVolumes::getTriangleArea(curGeometry->m_TriVerts[3*v1+0],
					curGeometry->m_TriVerts[3*v1+1],
					curGeometry->m_TriVerts[3*v1+2],
					curGeometry->m_TriVerts[3*v2+0],
					curGeometry->m_TriVerts[3*v2+1],
					curGeometry->m_TriVerts[3*v2+2],
					curGeometry->m_TriVerts[3*v3+0],
					curGeometry->m_TriVerts[3*v3+1],
					curGeometry->m_TriVerts[3*v3+2]);
		}
	}
	return area;
}

bool SurfaceData::getAreas(double isovalue, double* functionAtVertices, double* areaBelow, double* areaAbove)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	if(!functionAtVertices || !areaBelow || !areaAbove)
	{
		return false;
	}

	*areaBelow = 0;
	*areaAbove = 0;
	// Add area of each triangle
	{
		int i;

		for(i=0; i<curGeometry->m_NumTris; i++)
		{
			int v1 = curGeometry->m_Tris[3*i+0];
			int v2 = curGeometry->m_Tris[3*i+1];
			int v3 = curGeometry->m_Tris[3*i+2];
			double f1 = functionAtVertices[v1];
			double f2 = functionAtVertices[v2];
			double f3 = functionAtVertices[v3];

			if((f1<isovalue) && (f2<isovalue) && (f3<isovalue))
			{
				*areaBelow += CCVOpenGLMath::AreaVolumes::getTriangleArea(
								  curGeometry->m_TriVerts[3*v1+0],
								  curGeometry->m_TriVerts[3*v1+1],
								  curGeometry->m_TriVerts[3*v1+2],
								  curGeometry->m_TriVerts[3*v2+0],
								  curGeometry->m_TriVerts[3*v2+1],
								  curGeometry->m_TriVerts[3*v2+2],
								  curGeometry->m_TriVerts[3*v3+0],
								  curGeometry->m_TriVerts[3*v3+1],
								  curGeometry->m_TriVerts[3*v3+2]);
			}
			else if((f1>=isovalue) && (f2>=isovalue) && (f3>=isovalue))
			{
				*areaAbove += CCVOpenGLMath::AreaVolumes::getTriangleArea(
								  curGeometry->m_TriVerts[3*v1+0],
								  curGeometry->m_TriVerts[3*v1+1],
								  curGeometry->m_TriVerts[3*v1+2],
								  curGeometry->m_TriVerts[3*v2+0],
								  curGeometry->m_TriVerts[3*v2+1],
								  curGeometry->m_TriVerts[3*v2+2],
								  curGeometry->m_TriVerts[3*v3+0],
								  curGeometry->m_TriVerts[3*v3+1],
								  curGeometry->m_TriVerts[3*v3+2]);
			}
			else
			{
				// Make v1 the different one.
				if(f1<isovalue && f2<isovalue)
				{
					int temp = v1;
					v1 = v3;
					v3 = temp;
				}
				else if(f1<isovalue && f3<isovalue)
				{
					int temp = v1;
					v1 = v2;
					v2 = temp;
				}
				else if(f1>=isovalue && f2>=isovalue)
				{
					int temp = v1;
					v1 = v3;
					v3 = temp;
				}
				else if(f1>=isovalue && f3>=isovalue)
				{
					int temp = v1;
					v1 = v2;
					v2 = temp;
				}

				f1 = functionAtVertices[v1];
				f2 = functionAtVertices[v2];
				f3 = functionAtVertices[v3];
				double v1x = curGeometry->m_TriVerts[3*v1+0];
				double v1y = curGeometry->m_TriVerts[3*v1+1];
				double v1z = curGeometry->m_TriVerts[3*v1+2];
				double v2x = curGeometry->m_TriVerts[3*v2+0];
				double v2y = curGeometry->m_TriVerts[3*v2+1];
				double v2z = curGeometry->m_TriVerts[3*v2+2];
				double v3x = curGeometry->m_TriVerts[3*v3+0];
				double v3y = curGeometry->m_TriVerts[3*v3+1];
				double v3z = curGeometry->m_TriVerts[3*v3+2];
				// Now that v1 is different, calculate isocontour intersection points p1 and p2.
				double p1[3];
				double p2[3];

				if(fabs(f1-f2) < 0.000000000001)
				{
					continue;
				}

				if(fabs(f1-f3) < 0.000000000001)
				{
					continue;
				}

				double t1 = (f1-isovalue) / (f1-f2);
				p1[0] = v1x*(1-t1) + v2x*t1;
				p1[1] = v1y*(1-t1) + v2y*t1;
				p1[2] = v1z*(1-t1) + v2z*t1;
				double t2 = (f1-isovalue) / (f1-f3);
				p2[0] = v1x*(1-t2) + v3x*t2;
				p2[1] = v1y*(1-t2) + v3y*t2;
				p2[2] = v1z*(1-t2) + v3z*t2;
				// Calculate areas of 3 triangles and add them
				// 1. v1 p1 p2
				double area1 = CCVOpenGLMath::AreaVolumes::getTriangleArea(
								   v1x, v1y, v1z,
								   p1[0], p1[1], p1[2],
								   p2[0], p2[1], p2[2]);

				if(f1<isovalue)
				{
					*areaBelow += area1;
				}
				else
				{
					*areaAbove += area1;
				}

				// 2. x2 p1 p2
				double area2 = CCVOpenGLMath::AreaVolumes::getTriangleArea(
								   v2x, v2y, v2z,
								   p1[0], p1[1], p1[2],
								   p2[0], p2[1], p2[2]);

				if(f2<isovalue)
				{
					*areaBelow += area2;
				}
				else
				{
					*areaAbove += area2;
				}

				// 3. x2 x3 p2
				double area3 = CCVOpenGLMath::AreaVolumes::getTriangleArea(
								   v2x, v2y, v2z,
								   v3x, v3y, v3z,
								   p2[0], p2[1], p2[2]);

				if(f3<isovalue)
				{
					*areaBelow += area3;
				}
				else
				{
					*areaAbove += area3;
				}
			}
		}
	}
	return true;
}

void SurfaceData::clipValueChangedSlot(int value)
{
	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	int minVal = m_SurfaceRenderingProperties->_ui->m_Slider_Clip->minValue();
	int maxVal = m_SurfaceRenderingProperties->_ui->m_Slider_Clip->maxValue();
	int clamped = (value>minVal ? value : minVal);
	clamped = (clamped<maxVal ? clamped: maxVal);
	m_ClippingPlane = (double)clamped/((double)(maxVal - minVal));

	if(m_DataManager)
	{
		m_DataManager->updateGL();
	}
}

Plane SurfaceData::getClipPlane()
{
	double mins[3];
	double maxs[3];
	mins[0] = mins[1] = mins[2] = 0;
	maxs[0] = maxs[1] = maxs[2] = 0;

	if(m_Geometry.size() <= m_Frame)
	{
		return OpenGL_Viewer::getClipPlane(mins, maxs, m_ClippingPlane);
	}

	if(m_min.size() < (m_Frame+1)*3)
	{
		return OpenGL_Viewer::getClipPlane(mins, maxs, m_ClippingPlane);
	}

	if(m_max.size() < (m_Frame+1)*3)
	{
		return OpenGL_Viewer::getClipPlane(mins, maxs, m_ClippingPlane);
	}

	mins[0] = m_min[m_Frame*3+0];
	mins[1] = m_min[m_Frame*3+1];
	mins[2] = m_min[m_Frame*3+2];
	maxs[0] = m_max[m_Frame*3+0];
	maxs[1] = m_max[m_Frame*3+1];
	maxs[2] = m_max[m_Frame*3+2];
	return OpenGL_Viewer::getClipPlane(mins, maxs, m_ClippingPlane);
}

bool SurfaceData::save(const char* filename)
{
	if(!filename)
	{
		return false;
	}

	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	Geometry* curGeometry = m_Geometry[m_Frame];

	if(!curGeometry)
	{
		return false;
	}

	GeometryLoader* geometryLoader = new GeometryLoader();
	bool ret = geometryLoader->saveFile(filename, curGeometry);
	delete geometryLoader;
	return ret;
}

void SurfaceData::wireframeColorSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	if(!m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedWireframeColor->isChecked())
	{
		return;
	}

        QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->palette();
        QColor color = QColorDialog::getColor( palette.color(QPalette::Button) );
	if(color.isValid())
	{
		palette.setColor( QPalette::Normal, QPalette::Button, color );
		m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->setPalette( palette );
		m_Geometry[m_Frame]->m_UniqueWireframeColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniqueWireframeColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniqueWireframeColors[2] = color.blue()/255.0;
	}
}

const QColor& SurfaceData::getUserSelectedWireframeColor()
{
	return m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->palette().color( QPalette::Button );
}

void SurfaceData::setUserSelectedWireframeColor(const QColor& color)
{
	QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->palette();
	palette.setColor( QPalette::Normal, QPalette::Button, color );
	m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->setPalette( palette );
}

void SurfaceData::surfaceColorSlot()
{
	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->palette();
        QColor color = QColorDialog::getColor( palette.color(QPalette::Button) );
	if(color.isValid())
	{
		palette.setColor( QPalette::Normal, QPalette::Button, color );
		m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->setPalette( palette );
		setUserSelectedSurfaceColor(color);
		
		m_Geometry[m_Frame]->AllocateTriVertColors();	
		for(int i=0; i<m_Geometry[m_Frame]->m_NumTriVerts; i++)
		{
			m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+0] = color.red()/255.0;
			m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+1] = color.green()/255.0;
			m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+2] = color.blue()/255.0;
		}
	}
}

void SurfaceData::lineColorSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	if(!m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedLineColor->isChecked())
	{
		return;
	}

	QColor color = QColorDialog::getColor(getUserSelectedLineColor());

	if(color.isValid())
	{
		QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->palette();
		palette.setColor( QPalette::Normal, QPalette::Button, color );
		m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->setPalette( palette );
		m_Geometry[m_Frame]->m_UniqueLineColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniqueLineColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniqueLineColors[2] = color.blue()/255.0;
	}
}

void SurfaceData::pointColorSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	if(!m_SurfaceRenderingProperties->_ui->m_RadioButton_UserSelectedPointColor->isChecked())
	{
		return;
	}

	QColor color = QColorDialog::getColor(getUserSelectedPointColor());

	if(color.isValid())
	{
		QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->palette();
		palette.setColor( QPalette::Normal, QPalette::Button, color );
		m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->setPalette( palette );

		m_Geometry[m_Frame]->m_UniquePointColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniquePointColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniquePointColors[2] = color.blue()/255.0;
	}
}

const QColor& SurfaceData::getUserSelectedSurfaceColor()
{
	return m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->palette().color( QPalette::Button );
}

const QColor& SurfaceData::getUserSelectedLineColor()
{
	return m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->palette().color( QPalette::Button );
}

const QColor& SurfaceData::getUserSelectedPointColor()
{
	return m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->palette().color( QPalette::Button );
}

void SurfaceData::setUserSelectedSurfaceColor(const QColor& color)
{
	QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->palette();
	palette.setColor( QPalette::Normal, QPalette::Button, color );
	m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->setPalette( palette );
}

bool SurfaceData::supportsFileNames(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	int i;
	bool ret = true;

	for(i=0; i<fileNames.size(); i++)
	{
		ret = ret && (fileNames[i].endsWith("raw") ||
					  fileNames[i].endsWith("rawn") ||
					  fileNames[i].endsWith("rawc") ||
					  fileNames[i].endsWith("rawnc") ||
					  fileNames[i].endsWith("c2c")||
					  fileNames[i].endsWith("obj"));
	}

	return ret;
}

bool SurfaceData::getMinMax(float* min, float* max)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return false;
	}

	if(!m_Geometry[m_Frame])
	{
		return false;
	}

	// is this done somewhere ?
	//	if( !m_Geometry[m_Frame]->m_bExtentsReady )
	//		m_Geometry[m_Frame]->CalculateExtents();
	if(m_min.size() < (m_Frame+1)*3)
	{
		return false;
	}

	if(m_max.size() < (m_Frame+1)*3)
	{
		return false;
	}

	min[0] = m_min[m_Frame*3+0];
	min[1] = m_min[m_Frame*3+1];
	min[2] = m_min[m_Frame*3+2];
	max[0] = m_max[m_Frame*3+0];
	max[1] = m_max[m_Frame*3+1];
	max[2] = m_max[m_Frame*3+2];
	return true;
}

void SurfaceData::renderOnce()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(m_Geometry[m_Frame])
	{
		bool useClippingPlane = false;
		m_GeometryRenderer->render(m_Geometry[m_Frame],
								   getSurfaceColors(),
								   getRenderingTypeName(),
								   getClipPlane(),
								   useClippingPlane,
								   m_SurfaceAlpha);
	}

	if(m_RenderCurvatures)
	{
		m_NormalVectors[m_Frame]->m_UniqueWireframeColors[0] = 1.0;
		m_NormalVectors[m_Frame]->m_UniqueWireframeColors[1] = 0;
		m_NormalVectors[m_Frame]->m_UniqueWireframeColors[2] = 0;

		if(m_NormalVectors.size() > m_Frame)
		{
			if(m_NormalVectors[m_Frame])
			{
				m_GeometryRenderer->drawLines(m_NormalVectors[m_Frame]);
			}
		}

		m_K1CurvatureVectors[m_Frame]->m_UniqueWireframeColors[0] = 0;
		m_K1CurvatureVectors[m_Frame]->m_UniqueWireframeColors[1] = 1.0;
		m_K1CurvatureVectors[m_Frame]->m_UniqueWireframeColors[2] = 0;

		if(m_K1CurvatureVectors.size() > m_Frame)
		{
			if(m_K1CurvatureVectors[m_Frame])
			{
				m_GeometryRenderer->drawLines(m_K1CurvatureVectors[m_Frame]);
			}
		}

		m_K2CurvatureVectors[m_Frame]->m_UniqueWireframeColors[0] = 0;
		m_K2CurvatureVectors[m_Frame]->m_UniqueWireframeColors[1] = 0;
		m_K2CurvatureVectors[m_Frame]->m_UniqueWireframeColors[2] = 1.0;

		if(m_K2CurvatureVectors.size() > m_Frame)
		{
			if(m_K2CurvatureVectors[m_Frame])
			{
				m_GeometryRenderer->drawLines(m_K2CurvatureVectors[m_Frame]);
			}
		}
	}
}

void SurfaceData::render()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

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
			//if( distanceSqFromObject() < cutoffSq ) // need to make this a multires call
			renderOnce();
			glPopMatrix();
		}
	}
	else
	{
		renderOnce();
	}
}

void SurfaceData::enableScript()
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

	this->setName("surface");
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

void SurfaceData::setVertexColors(float* colors, int numberOfColors)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!colors || (numberOfColors<1))
	{
		return;
	}

	if(((int)(numberOfColors/3))*3 !=numberOfColors)
	{
		return;
	}

	m_Geometry[m_Frame]->AllocateTriVertColors(); // Warning, this will black out everything! Original colors will be lost.
	int i;

	for(i=0; i<m_Geometry[m_Frame]->m_NumTriVerts; i++)
	{
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+0] = colors[i*3+0];
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+1] = colors[i*3+1];
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+2] = colors[i*3+2];

		if((i+1)*3==numberOfColors)
		{
			break;
		}
	}
}

void SurfaceData::setVertexColor(float r, float g, float b, int index)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(index<0)
	{
		return;
	}

	if(index >= m_Geometry[m_Frame]->m_NumTriVerts)
	{
		return;
	}

	if(!m_Geometry[m_Frame]->m_TriVertColorsTransparent)
	{
		return;
	}

	m_Geometry[m_Frame]->m_TriVertColorsTransparent[index*4+0] = r;
	m_Geometry[m_Frame]->m_TriVertColorsTransparent[index*4+1] = g;
	m_Geometry[m_Frame]->m_TriVertColorsTransparent[index*4+2] = b;
}

void SurfaceData::setRadiusCutOffForRendering(float radius)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(m_RadiiOfCurvature.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_RadiiOfCurvature[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->AllocateTriVertColors(); // Warning, this will black out everything! Original colors will be lost.
	int i;

	for(i=0; i<m_Geometry[m_Frame]->m_NumTriVerts; i++)
	{
		double r=0, g=0, b=0;
		double r1 = (m_RadiiOfCurvature[m_Frame])[i*2+0];
		double r2 = (m_RadiiOfCurvature[m_Frame])[i*2+1];
		double avgR = (fabs(r1) + fabs(r2)) / 2.0;

		if(avgR < radius)
		{
			r = 1.0;
		}
		else
		{
			g = 1.0;
		}

		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+0] = r;
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+1] = g;
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[i*4+2] = b;
	}
}

void SurfaceData::renderCurvatures(bool render)
{
	m_RenderCurvatures = render;
}

Geometry* SurfaceData::getGeometry()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return 0;
	}

	return m_Geometry[m_Frame];
}

Geometry* SurfaceData::getGeometry(int index)
{
	if(m_Geometry.size() <= index)
	{
		return 0;
	}

	return m_Geometry[index];
}

void SurfaceData::replicateSlot()
{
	QString transFile = Q3FileDialog::getOpenFileName("", "Transformations (*.txt);;All files (*.*)");

	if(transFile.length() < 1)
	{
		return;
	}

	readTransformations(transFile.latin1());
}

bool SurfaceData::parseAnimationCommand(QStringList commands, int curCommand)
{
	return false;
}

void SurfaceData::renderTrianglesSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	m_Geometry[m_Frame]->m_RenderTriangles = m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderSurface->isChecked();
	updateGL();
}

void SurfaceData::renderWireframesSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	m_Geometry[m_Frame]->m_RenderWireframe = m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderWireframe->isChecked();
	updateGL();
}

void SurfaceData::renderLinesSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	if(!m_SurfaceRenderingProperties)
	{
		return;
	}

	m_Geometry[m_Frame]->m_RenderLines = m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderLines->isChecked();
	updateGL();
}

void SurfaceData::renderPointsSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_RenderPoints = m_SurfaceRenderingProperties->_ui->m_CheckBox_RenderPoints->isChecked();
	//copy the points from tri-verts
	m_Geometry[m_Frame]->AllocatePoints(m_Geometry[m_Frame]->m_NumTriVerts);

	for(unsigned int i = 0; i < m_Geometry[m_Frame]->m_NumPoints*3; i++)
	{
		m_Geometry[m_Frame]->m_Points[i] = m_Geometry[m_Frame]->m_TriVerts[i];
	}
	updateGL();
}


void SurfaceData::wireframeUniqueColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UseWireframeColors = true;
	QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_WireframeColor->palette();
        QColor color = palette.color(QPalette::Button);
	if(color.isValid())
	{
		m_Geometry[m_Frame]->m_UniqueWireframeColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniqueWireframeColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniqueWireframeColors[2] = color.blue()/255.0;
	}
	updateGL();
}

void SurfaceData::wireframeDefaultColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UseWireframeColors = false;

	m_Geometry[m_Frame]->m_UniqueWireframeColors[0] = 0.5f;
	m_Geometry[m_Frame]->m_UniqueWireframeColors[1] = 0.5f;
	m_Geometry[m_Frame]->m_UniqueWireframeColors[2] = 0.5f;

	updateGL();
}

void SurfaceData::lineUniqueColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UseLineColors = true;

        QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_LineColor->palette();
        QColor color = palette.color(QPalette::Button);
	if(color.isValid())
	{
		m_Geometry[m_Frame]->m_UniqueLineColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniqueLineColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniqueLineColors[2] = color.blue()/255.0;
	}

	updateGL();
}

void SurfaceData::lineDefaultColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UseLineColors = false;

	m_Geometry[m_Frame]->m_UniqueLineColors[0] = 0.5f;
	m_Geometry[m_Frame]->m_UniqueLineColors[1] = 0.5f;
	m_Geometry[m_Frame]->m_UniqueLineColors[2] = 0.5f;

	updateGL();
}

void SurfaceData::pointUniqueColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UsePointColors = true;

        QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_PointColor->palette();
        QColor color = palette.color(QPalette::Button);
	if(color.isValid())
	{
		m_Geometry[m_Frame]->m_UniquePointColors[0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_UniquePointColors[1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_UniquePointColors[2] = color.blue()/255.0;
	}

	updateGL();
}

void SurfaceData::pointDefaultColorRadioButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->m_UsePointColors = false;

	m_Geometry[m_Frame]->m_UniquePointColors[0] = 0.5f;
	m_Geometry[m_Frame]->m_UniquePointColors[1] = 0.5f;
	m_Geometry[m_Frame]->m_UniquePointColors[2] = 0.5f;

	updateGL();
}

void SurfaceData::wireframeThicknessChangedSlot(int thickness)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->SetWireframeWidth(thickness);
	updateGL();
}

void SurfaceData::pointThicknessChangedSlot(int thickness)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->setPointSize(thickness);
	updateGL();
}

void SurfaceData::lineThicknessChangedSlot(int thickness)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	m_Geometry[m_Frame]->SetLineWidth(thickness);
	updateGL();
}

void SurfaceData::renderingTypeSlot(int renderingMode)
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}

	switch( renderingMode ) {
	case SurfRenderType_Color:
	   {
	   QPalette palette = m_SurfaceRenderingProperties->_ui->m_PushButton_SurfaceColor->palette();
           QColor color = palette.color(QPalette::Button);

	   m_Geometry[m_Frame]->AllocateTriVertColors();	
	   for(int i=0; i<m_Geometry[m_Frame]->m_NumTriVerts; i++)
	   {
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+0] = color.red()/255.0;
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+1] = color.green()/255.0;
		m_Geometry[m_Frame]->m_TriVertColorsTransparent[4*i+2] = color.blue()/255.0;
	   }
 
	   m_Geometry[m_Frame]->enable2DTriangleTexture( false );
	   }
	   break;
	case SurfRenderType_Normal:
	   m_Geometry[m_Frame]->setColors(renderingMode);
	   m_Geometry[m_Frame]->enable2DTriangleTexture( false );
	   break;
	case SurfRenderType_Mean:
	   m_Geometry[m_Frame]->setColors(renderingMode);
	   m_Geometry[m_Frame]->enable2DTriangleTexture( false );
	   break;
	case SurfRenderType_Gaussian:
	   m_Geometry[m_Frame]->setColors(renderingMode);
	   m_Geometry[m_Frame]->enable2DTriangleTexture( false );
	   break;
	case SurfRenderType_Texture:
	   if( !hasTextureImage ) {
               if( readSurfaceTexture() )
	          m_Geometry[m_Frame]->set2DTexture(m_opengl_qimage.width(), m_opengl_qimage.height(), m_opengl_qimage.bits(), m_opengl_qimage.numBytes()/(m_opengl_qimage.width()*m_opengl_qimage.height()));
	   }
	   m_Geometry[m_Frame]->enable2DTriangleTexture( true );
	   break;
        }
	updateGL();
}


bool SurfaceData::readSurfaceTexture()
{
	Q3StrList imgList = QImageWriter::supportedImageFormats();
	QString filter = QString("Images (");
	int i;

	for(i=0; i<imgList.count(); i++)
	{
		if(i==0)
		{
			filter.append("*.");
		}
		else
		{
			filter.append(" *.");
		}

		filter.append(QString(imgList.at(i)));
	}

	filter.append(")");
	int n = imgList.count();
	QString filename = Q3FileDialog::getOpenFileName("../Dataset",
					   filter,
					   m_DataManager->m_ParentPropertiesWidget,
					   "open file dialog",
					   "Choose texture map");
	QImage qimage;

	if(!qimage.load(filename.latin1()))
	{
		return false;
	}

	m_opengl_qimage = QGLWidget::convertToGLFormat(qimage);
	hasTextureImage = true;
	return true;
}

void SurfaceData::surfaceTexturePushButtonSlot()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return;
	}

	if(!m_Geometry[m_Frame])
	{
		return;
	}
	
	if( readSurfaceTexture() )
	   m_Geometry[m_Frame]->set2DTexture(m_opengl_qimage.width(), m_opengl_qimage.height(), m_opengl_qimage.bits(), m_opengl_qimage.numBytes()/(m_opengl_qimage.width()*m_opengl_qimage.height()));
	updateGL();
}


void SurfaceData::transparencySliderSlot(int value)
{
	// set the alpha component of SurfaceColor
	m_SurfaceAlpha = value/99.0f;
	// updateGL so user can track how the alpha changes look
	updateGL();
}
//  This functions needs to be improved a lot to copy all members of the original triangle SKVINAY
Geometry* SurfaceData::createDuplicates()
{
	if(m_Geometry.size() <= m_Frame)
	{
		return 0;
	}

	if(!m_Geometry[m_Frame])
	{
		return 0;
	}

	Geometry* geom = new Geometry();
	int n = m_ReplicationMatrices.size();

	if(n == 0)
	{
		geom = m_Geometry[m_Frame];
		return geom;
	}

	int numTri = m_Geometry[m_Frame]->m_NumTris;
	int numVert = m_Geometry[m_Frame]->m_NumTriVerts;
	geom->AllocateTris(numVert*n, numTri*n);

	if(m_Geometry[m_Frame]->m_TriVertColorsTransparent)
	{
		geom->AllocateTriVertColors();
	}

	int i;

	for(i=0; i<n; i++)
	{
		int j;

		for(j=0; j<numTri*3; j++)
		{
			int v = m_Geometry[m_Frame]->m_Tris[j];
			geom->m_Tris[i* numTri*3+j] = i*numVert + v;
		}

		for(j=0; j<numVert; j++)
		{
			float x = m_Geometry[m_Frame]->m_TriVerts[j*3+0];
			float y = m_Geometry[m_Frame]->m_TriVerts[j*3+1];
			float z = m_Geometry[m_Frame]->m_TriVerts[j*3+2];
			CCVOpenGLMath::Vector v = CCVOpenGLMath::Vector(x, y, z, 1);
			CCVOpenGLMath::Vector trans_v = (*m_ReplicationMatrices.at(i))*v;
			trans_v.normalize();
			x = trans_v[0];
			y = trans_v[1];
			z = trans_v[2];
			geom->m_TriVerts[i* numVert*3 + j*3+0] = x;
			geom->m_TriVerts[i* numVert*3 + j*3+1] = y;
			geom->m_TriVerts[i* numVert*3 + j*3+2] = z;

			if(m_Geometry[m_Frame]->m_TriVertColorsTransparent)
			{
				float r = m_Geometry[m_Frame]->m_TriVertColorsTransparent[j*4+0];
				float g = m_Geometry[m_Frame]->m_TriVertColorsTransparent[j*4+1];
				float b = m_Geometry[m_Frame]->m_TriVertColorsTransparent[j*4+2];
				geom->m_TriVertColorsTransparent[i* numVert*3 + j*4+0] = r;
				geom->m_TriVertColorsTransparent[i* numVert*3 + j*4+1] = g;
				geom->m_TriVertColorsTransparent[i* numVert*3 + j*4+2] = b;
			}
		}

		CCVOpenGLMath::Matrix* m = m_ReplicationMatrices.at(i);
	}

	return geom;
}

void SurfaceData::setAlpha(float alpha)
{
	m_SurfaceAlpha = alpha;
	m_SurfaceRenderingProperties->_ui->m_Slider_Transparency->setValue((int)(alpha*100));
}
