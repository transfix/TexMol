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
// VolumeData.cpp: implementation of the VolumeData class.

#include <qcheckbox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <ColorTable2/ColorTable.h>

#include "ui_VolumeRenderingSetBox.h"
#include <DataManager/VolumeDataManager/VolumeData.h>


#include <VolumeFileTypes/VolumeLoader.h>
#include <DataManager/DataManager.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <Contouring/MultiContour.h>
#include <GeometryFileTypes/GeometryLoader.h>
#include <Geometry/Geometry.h>
#include <Contour/contour.h>
#include <C2C/c2c_codec.h>
#include <OpenGL_Viewer/Plane.h>
#include <OpenGL_Viewer/OpenGL_Viewer.h>
#include <stdio.h>
#include <string>

VolumeData::VolumeData(DataManager* dataManager)
	: AbstractData(dataManager)
{
	m_CurrentIsocontourSelected = -1; // needed to see if a new bar was selected.
	m_SimpleVolumeData = 0;
	m_Loaded = false;
	m_VolumeRenderingProperties = 0;
	m_MultiContour = 0;
	data1 = 0;
	data2 = 0;
	data3 = 0;
	data4 = 0;
	// related to contour spectrum
	array_size = 0;
	isoval = 0;
	area = 0;
	min_vol = 0;
	max_vol = 0;
	gradient = 0;

	if(dataManager)
	{
		m_VolumeRenderingProperties = new VolumeRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_VolumeRenderingProperties->hide();
		
		clipValueChangedSlot(m_VolumeRenderingProperties->_ui->m_ClipSlider->value());
		// It seems that splatting renderer doesn't work and we don't need that option. So pull it out for now.
		//m_VolumeRenderingProperties->_ui->m_RadioButton_VolRender->setChecked(true);

		connectSlots();
	}
}

bool VolumeData::connectSlots()
{
	if(!m_VolumeRenderingProperties)
	{
		return false;
	}

	//ColorTable Slot
  	connect(m_VolumeRenderingProperties->m_ColorTable,SIGNAL(changed()),SLOT(functionChangedSlot()));
	connect((const class QObject*)(m_VolumeRenderingProperties->_ui->m_CheckBox_Rendering), SIGNAL(toggled(bool)), this, SLOT(renderEnabledSlot(bool)));
	m_Alpha = 1.0f;
	connect((const class QObject*)(m_VolumeRenderingProperties->_ui->m_ClipSlider), SIGNAL(valueChanged(int)), this, SLOT(clipValueChangedSlot(int)));
	connect((const class QObject*)(m_VolumeRenderingProperties->_ui->m_TransparencySlider), SIGNAL(valueChanged(int)), this, SLOT(transparencyValueChangedSlot(int)));
	connect((const class QObject*)(m_VolumeRenderingProperties->_ui->m_SaveIsocontoursButton), SIGNAL(clicked()), this, SLOT(isocontourNodeSavedSlot()));

	return true;
}

void VolumeData::associateDataManager(DataManager* dataManager)
{
	if(dataManager)
	{
		AbstractData::associateDataManager(dataManager);
		m_VolumeRenderingProperties = new VolumeRenderingProperties(dataManager->m_ParentPropertiesWidget);
		m_VolumeRenderingProperties->hide();
		connectSlots();
	}
}

VolumeData::~VolumeData()
{
	delete []isoval;
	isoval = 0;
	delete []area;
	area = 0;
	delete []min_vol;
	min_vol = 0;
	delete []max_vol;
	max_vol = 0;
	delete []gradient;
	gradient = 0;
	delete m_SimpleVolumeData;
	//delete m_VolumeRenderingProperties; m_VolumeRenderingProperties = 0; this causes a crash when we try to hide this widget from the main window! Why ?
	delete m_MultiContour;
	m_MultiContour = 0;
	delete []data1;
	delete []data2;
	delete []data3;
	delete []data4;
}

bool VolumeData::updateProperties()
{
	if(!m_VolumeRenderingProperties)
	{
		return false;
	}

	setValExtents();

	if(!m_VolumeRenderingProperties->m_ColorTable)
	{
		return false;
	}

	// reset the color table for rawv files.
	if(m_SimpleVolumeData)
	{
		if(m_SimpleVolumeData->getNumberOfVariables() > 3)
		{
			m_VolumeRenderingProperties->m_ColorTable->reset();
		}
	}

	return true;
}

bool VolumeData::read(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	const char* fileName = (fileNames[0]).toLatin1().constData();

	delete m_SimpleVolumeData;

	m_SimpleVolumeData = 0;

	VolumeLoader* volumeLoader = new VolumeLoader();

	m_SimpleVolumeData = volumeLoader->loadFile(fileName);

	delete volumeLoader;

	if(!m_SimpleVolumeData)
	{
		return false;
	}

	updateProperties();
	return true;
}

bool VolumeData::setSimpleData(SimpleVolumeData* sData)
{
	if(!sData)
	{
		return false;
	}

	if(m_SimpleVolumeData)
	{
		delete m_SimpleVolumeData;
	}

	m_SimpleVolumeData = sData;
	updateProperties();
	return true;
}

void VolumeData::loadColorMap()
{
	static int counter = 0;

	if(m_DataManager)
	{
		int colorMapSize = 256; // new rover supports only 256  - be careful SKVINAY !
		double dColorMap[256*4];
		int idx;		
		unsigned char *_uColorMap = m_VolumeRenderingProperties->m_ColorTable->getCharTable(colorMapSize);
		double r_256 = 1.0/256.0;
		for(int i=0; i<256; i++) {
			idx = i*4;
			dColorMap[idx]	 = _uColorMap[idx]   *r_256;
			dColorMap[idx+1] = _uColorMap[idx+1] *r_256;
			dColorMap[idx+2] = _uColorMap[idx+2] *r_256;
			dColorMap[idx+3] = _uColorMap[idx+3] *r_256;
		}

		m_DataManager->uploadColorMap(_uColorMap, colorMapSize);
		m_DataManager->uploadColorMap(dColorMap, colorMapSize);
		delete _uColorMap;
	}
}

void VolumeData::functionChangedSlot()
{
	if(m_DataManager)
	{
		isocontourChangedSlot();

		loadColorMap();
		m_DataManager->update();
	}
}

void VolumeData::isocontourChangedSlot()
{
	if(!m_SimpleVolumeData) {
		return;		}

	
	CVCColorTable::isocontour_nodes _currentNodes = m_VolumeRenderingProperties->m_ColorTable->info().isocontourNodes();

	bool needToUpdate = false;
	if(_currentNodes.size() != isoValueList.size()) {
		// New isocontour node added
		needToUpdate = true;
	}
	else {
		// Check changes of isovalues
		CVCColorTable::isocontour_nodes::iterator it; int i;
		for(i=0, it=_currentNodes.begin(); it != _currentNodes.end(); it++, i++) {
			if(it->position != isoValueList[i]) {
				needToUpdate = true; break;
			}
		}
	}
		
	if(needToUpdate) {
		if(m_SimpleVolumeData->getNumberOfVariables() < 1)
			return;

		//check contour allocation
		if(m_MultiContour == 0)
			m_MultiContour = new CCVContouring::MultiContour();

		if(m_MultiContour == 0)
			return;    // something went wrong !

		// remove all contours
		m_MultiContour->removeAll();

		//insert new ones
		const double defaultColor[] = {0.132, 0.987, 0.987};
		isoValueList.clear();
		CVCColorTable::isocontour_nodes::iterator it;
		int i;
		for(i=0, it=_currentNodes.begin(); it != _currentNodes.end(); it++, i++) {
			isocontourNodeAddedSlot(i+1, it->position, defaultColor[0], defaultColor[1], defaultColor[2]);
			isoValueList.push_back(it->position);
		}
	}
}

void VolumeData::isocontourNodeAddedSlot(int index, double isovalue, double R, double G, double B)
{
	if(m_SimpleVolumeData->getNumberOfVariables() < 1)
	{
		return;
	}

	if(m_SimpleVolumeData->getNumberOfVariables() > 3)
	{
		if(!data1 || !data2 || !data3 || !data4)
		{
			int width = m_SimpleVolumeData->getWidth();
			int height = m_SimpleVolumeData->getHeight();
			int depth = m_SimpleVolumeData->getDepth();
			delete []data1;
			delete []data2;
			delete []data3;
			delete []data4;
			data1 = new unsigned char[width*height*depth];
			data2 = new unsigned char[width*height*depth];
			data3 = new unsigned char[width*height*depth];
			data4 = new unsigned char[width*height*depth];
			SimpleVolumeData::getUnsignedCharData(m_SimpleVolumeData->getData(0), m_SimpleVolumeData->getType(0), data1, width, height, depth);
			SimpleVolumeData::getUnsignedCharData(m_SimpleVolumeData->getData(1), m_SimpleVolumeData->getType(1), data2, width, height, depth);
			SimpleVolumeData::getUnsignedCharData(m_SimpleVolumeData->getData(2), m_SimpleVolumeData->getType(2), data3, width, height, depth);
			SimpleVolumeData::getUnsignedCharData(m_SimpleVolumeData->getData(3), m_SimpleVolumeData->getType(3), data4, width, height, depth);
			m_MultiContour->setData(
				data4,
				data1,
				data2,
				data3,
				m_SimpleVolumeData->getWidth(), m_SimpleVolumeData->getHeight(), m_SimpleVolumeData->getDepth(),
				m_SimpleVolumeData->getMaxX()-m_SimpleVolumeData->getMinX(),
				m_SimpleVolumeData->getMaxY()-m_SimpleVolumeData->getMinY(),
				m_SimpleVolumeData->getMaxZ()-m_SimpleVolumeData->getMinZ(),
				0,0,0,
				1,1,1,
				m_SimpleVolumeData->getMinX(), m_SimpleVolumeData->getMinY(), m_SimpleVolumeData->getMinZ(),
				m_SimpleVolumeData->getMaxX(), m_SimpleVolumeData->getMaxY(), m_SimpleVolumeData->getMaxZ());
		}
	}
	else
	{
		if(!data1)
		{
			int width = m_SimpleVolumeData->getWidth();
			int height = m_SimpleVolumeData->getHeight();
			int depth = m_SimpleVolumeData->getDepth();
			delete []data1;
			data1 = new unsigned char[width*height*depth];
			//SimpleVolumeData::getUnsignedCharData( m_SimpleVolumeData->getData(0),
			//	m_SimpleVolumeData->getType(0), data1, width, height, depth );
			SimpleVolumeData::getNormalizedUnsignedCharData(m_SimpleVolumeData->getData(0),
					m_SimpleVolumeData->getType(0), data1, width, height, depth);
			m_MultiContour->setData(
				data1,
				m_SimpleVolumeData->getWidth(), m_SimpleVolumeData->getHeight(), m_SimpleVolumeData->getDepth(),
				m_SimpleVolumeData->getMaxX()-m_SimpleVolumeData->getMinX(),
				m_SimpleVolumeData->getMaxY()-m_SimpleVolumeData->getMinY(),
				m_SimpleVolumeData->getMaxZ()-m_SimpleVolumeData->getMinZ(),
				0,0,0,
				1,1,1,
				m_SimpleVolumeData->getMinX(), m_SimpleVolumeData->getMinY(), m_SimpleVolumeData->getMinZ(),
				m_SimpleVolumeData->getMaxX(), m_SimpleVolumeData->getMaxY(), m_SimpleVolumeData->getMaxZ());
		}
	}

	m_MultiContour->addContour(index, isovalue*255, R, G, B);
}

void VolumeData::isocontourNodeDeletedSlot(int index)
{
	printf("Deleted isocontour with index = %d\n", index);

	// delete it internally within volume data itself
	if(!m_MultiContour)
	{
		return;    // error
	}

	m_MultiContour->removeContour(index);

	if(m_DataManager)
	{
		m_DataManager->update();
	}

	if(m_CurrentIsocontourSelected == index)
	{
		if(m_VolumeRenderingProperties)
		{
			m_VolumeRenderingProperties->_ui->m_LineEdit_AreaEdit->clear();
			m_VolumeRenderingProperties->_ui->m_LineEdit_VolumeEdit->clear();
		}
	}
}

void VolumeData::isocontourNodeChangedSlot(int index, double isovalue, double R, double G, double B)
{
	printf("Changed isocontour with index = %d, value = %lf\n", index, isovalue);

	if(!m_MultiContour)
	{
		return;    // error
	}

	m_MultiContour->setIsovalue(index, isovalue*255.0);
	m_DataManager->update();
}

void VolumeData::isocontourNodeColorChangedSlot(int node, double R, double G, double B)
{
	if(!m_MultiContour)
	{
		return;    // error
	}

	m_MultiContour->setColor(node, R,G,B);
	m_DataManager->update();
}

void VolumeData::renderEnabledSlot(bool enabled)
{
	if(m_DataManager)
	{
		loadColorMap();
		AbstractData::renderEnabledSlot(enabled);
	}
}

bool VolumeData::isLoaded()
{
	return m_Loaded;
}

void VolumeData::setLoaded(bool loaded)
{
	m_Loaded = loaded;
}

QWidget* VolumeData::getPropertiesWidget()
{
	if(m_DataManager)
	{
		return m_VolumeRenderingProperties;
	}

	return 0;
}

void VolumeData::setVisibilityInPropertiesWidget()
{
	if(m_DataManager)
	{
		m_VolumeRenderingProperties->_ui->m_CheckBox_Rendering->setChecked(renderEnabled[m_DataManager->getCurrentRenderer()]);
	}
}

void VolumeData::setValExtents()
{
	if(!m_SimpleVolumeData)
	{
		return;
	}

	double minVal = 0.0;
	double maxVal = 1.0;

	if(m_SimpleVolumeData ->getNumberOfVariables() < 4)
	{
		if(!m_SimpleVolumeData ->getMinMax(0, &minVal, &maxVal))
		{
			return;
		}
	}
	else
	{
		if(!m_SimpleVolumeData ->getMinMax(3, &minVal, &maxVal))
		{
			return;
		}
	}

	if(m_VolumeRenderingProperties)
	{
		// it seems that these values are fixed in current ColorTable structure
	//	m_VolumeRenderingProperties->m_ColorTable->setValExtents(minVal, maxVal);
	}
}

bool VolumeData::deleteIsocontourBar(int index)
{
	if(! m_VolumeRenderingProperties)
	{
		return false;
	}

	if(! m_VolumeRenderingProperties->m_ColorTable)
	{
		return false;
	}
	
	CVCColorTable::ColorTable::isocontour_nodes _contour_nodes = m_VolumeRenderingProperties->m_ColorTable->info().isocontourNodes();
	CVCColorTable::ColorTable::isocontour_nodes::iterator it = _contour_nodes.begin();
	for(int i=0; i<index; i++) it++;
        _contour_nodes.erase(it);

	return true;
}

bool VolumeData::setHardCutOff(float cutOffValue)
{
	return false;
}

void VolumeData::renderContours()
{
	Plane plane = getClippingPlane();

	if(m_MultiContour == 0)
	{
		return;
	}

	m_MultiContour->render(plane.a(),plane.b(),plane.c(),plane.d());
}

void VolumeData::isocontourNodeSavedSlot()
{
	GeometryLoader loader;
	QString filter;
	string s = loader.getSaveFilterString() + ";;C2C (*.c2c)";
	QString fileExtensionsSupported = (QString)(s.c_str());
	QString filename = QFileDialog::getSaveFileName(nullptr, "Save As", QString(), fileExtensionsSupported, &filter);
	string str_filename = (string)(filename.toLatin1().constData());
	string str_filter = (string)(filter.toLatin1().constData());

	if(!filename.isNull())
	{
		//////// if c2c , save using c2c code ///////
		if(filter.contains("c2c"))
		{
			if(!m_SimpleVolumeData)
			{
				return;
			}

			if(m_SimpleVolumeData->getNumberOfVariables() < 1)
			{
				return;
			}

			float span[3];
			span[0] = m_SimpleVolumeData->getSpanX();
			span[1] = m_SimpleVolumeData->getSpanY();
			span[2] = m_SimpleVolumeData->getSpanZ();
			float orig[3];
			orig[0] = m_SimpleVolumeData->getMinX();
			orig[1] = m_SimpleVolumeData->getMinY();
			orig[2] = m_SimpleVolumeData->getMinZ();
			int dim[3];
			dim[0] = m_SimpleVolumeData->getWidth();
			dim[1] = m_SimpleVolumeData->getHeight();
			dim[2] = m_SimpleVolumeData->getDepth();
			void* data = m_SimpleVolumeData->getData(0);
			unsigned char* red = 0;
			unsigned char* green = 0;
			unsigned char* blue = 0;

			if(m_SimpleVolumeData->getNumberOfVariables() >= 4)
			{
				red   = (unsigned char*)m_SimpleVolumeData->getData(1);
				green = (unsigned char*)m_SimpleVolumeData->getData(2);
				blue  = (unsigned char*)m_SimpleVolumeData->getData(3);
			}

			unsigned char type = 2;
			float isoval = 0.5;
			writeC2CFile(data, red, green,
						 blue, type, filename.toLatin1().constData(),
						 isoval, dim, orig, span);
			return;
		}

		// Try to save using general GeometryLoaded class
		Geometry* geo = m_MultiContour->getGeometry();

		if(!loader.saveFile(str_filename, str_filter, geo))
		{
			printf("Error saving file\n");
		}
	}
}

QString VolumeData::getSelection(CCVOpenGLMath::Ray targetVector)
{
	return 0;
}


void getContourSpectrum(unsigned char* uchar_data, int type, int* dim, int array_size, float* isoval , float* area, float* min_vol, float* max_vol, float* gradient, float* span);
void compute_Area_Volume(int array_size, float* isovalue, float* area, float* min_vol, float* max_vol, float* gradient, float isoval, float& return_area,float& return_minvol, float& return_maxvol, float& return_grad);
void getVolume(float* vol,int dim[3], double orig[3], double span[3], double isovalue,double* Area, double* Volume);

void VolumeData::initSpectrumSlot()
{
	if(!m_SimpleVolumeData)
	{
		return;
	}

	if(m_SimpleVolumeData ->getNumberOfVariables() < 0)
	{
		return;
	}

	if(!allocateSpectrumData(256))
	{
		return;
	}

	int type;
	void* data;

	if(m_SimpleVolumeData ->getNumberOfVariables() < 4)
	{
		// rawiv
		data = m_SimpleVolumeData->getData(0);
		type = m_SimpleVolumeData->getType(0);
		//if( !m_SimpleVolumeData ->getMinMax( 0, &minVal, &maxVal ) ) return;
	}
	else
	{
		// rawv
		data = m_SimpleVolumeData->getData(3);
		type = m_SimpleVolumeData->getType(3);
		//if( !m_SimpleVolumeData ->getMinMax( 3, &minVal, &maxVal ) ) return;
	}

	int dim[3];
	dim[0] = m_SimpleVolumeData->getWidth();
	dim[1] = m_SimpleVolumeData->getHeight();
	dim[2] = m_SimpleVolumeData->getDepth();
	float span[3];
	span[0] = m_SimpleVolumeData->getSpanX();
	span[1] = m_SimpleVolumeData->getSpanY();
	span[2] = m_SimpleVolumeData->getSpanZ();
	array_size = 256;

	switch(type)
	{
		case SimpleVolumeData::UCHAR:
			getContourSpectrum((unsigned char*)data, CONTOUR_UCHAR, dim, array_size, isoval , area, min_vol, max_vol, gradient, span);
			break;
		case SimpleVolumeData::USHORT:
			getContourSpectrum((unsigned char*)data, CONTOUR_USHORT, dim, array_size, isoval , area, min_vol, max_vol, gradient, span);
			break;
		case SimpleVolumeData::ULONG:
			return;
		case SimpleVolumeData::FLOAT:
			getContourSpectrum((unsigned char*)data, CONTOUR_FLOAT, dim, array_size, isoval , area, min_vol, max_vol, gradient, span);
			break;
		case SimpleVolumeData::DOUBLE:
			return;
		default:
			return;
	}
}

void VolumeData::selectedIsocontourNodeSlot(int index, double isovalue)
{
	/*
	if( m_VolumeRenderingProperties->m_InitSpectrumPushButton->isEnabled() ) return;
	//if( index == m_CurrentIsocontourSelected ) return;
	*/
	if(!m_SimpleVolumeData)
	{
		return;
	}

	double minVal = 0.0, maxVal = 1.0;

	if(m_SimpleVolumeData ->getNumberOfVariables() < 4)
	{
		if(!m_SimpleVolumeData ->getMinMax(0, &minVal, &maxVal))
		{
			return;
		}
	}
	else
	{
		if(!m_SimpleVolumeData ->getMinMax(3, &minVal, &maxVal))
		{
			return;
		}
	}

	//float actual_isovalue = minVal + isovalue * (maxVal - minVal);
	float actual_isovalue = isovalue;
	float eval_area,eval_minvol,eval_maxvol,eval_grad;
	compute_Area_Volume(array_size, isoval, area, min_vol, max_vol, gradient, actual_isovalue, eval_area, eval_minvol, eval_maxvol, eval_grad);

	if(eval_area < 0)
	{
		eval_area = 0;
	}

	if(eval_maxvol < 0)
	{
		eval_maxvol = 0;
	}

	m_VolumeRenderingProperties->_ui->m_LineEdit_AreaEdit->setText(QString::number(eval_area));
	m_VolumeRenderingProperties->_ui->m_LineEdit_VolumeEdit->setText(QString::number(eval_maxvol));
	m_CurrentIsocontourSelected = index;
	return;
}

bool VolumeData::allocateSpectrumData(int size)
{
	if(size < 1)
	{
		return false;
	}

	isoval	= new float[size];
	area	= new float[size];
	min_vol = new float[size];
	max_vol = new float[size];
	gradient= new float[size];
	return true;
}

void VolumeData::acquireContourSpectrumSlot()
{
	// when we return, we should preferably check whether it passed or failed and reset the menu item.
	if(!m_VolumeRenderingProperties)
	{
		return;
	}

	if(!m_VolumeRenderingProperties->m_ColorTable)
	{
		return;
	}

	if(!isoval || !area || !min_vol || !max_vol || !gradient)
	{
		initSpectrumSlot(); // this will also update the button etc.
	/*Q3Err:I
		m_VolumeRenderingProperties->m_ColorTable->setSpectrumFunctions(isoval, area, min_vol, max_vol, gradient);
	*/
	}
}

int VolumeData::getRenderingType()
{
	//Currently, we only use volume rendering option.
	return VOLUME_RENDERING;
	/*
	if(!m_VolumeRenderingProperties)
	{
		return VOLUME_RENDERING;
	}

	if(m_VolumeRenderingProperties->_ui->m_RadioButton_VolRender->isChecked())
	{
		return VOLUME_RENDERING;
	}
	else
	{
		return SPLATTING;
	}
	*/
}

void VolumeData::clipValueChangedSlot(int value)
{
	if(!m_VolumeRenderingProperties)
	{
		return;
	}
	int minVal = m_VolumeRenderingProperties->_ui->m_ClipSlider->minimum();
	int maxVal = m_VolumeRenderingProperties->_ui->m_ClipSlider->maximum();
	int clamped = (value>minVal ? value : minVal);
	clamped = (clamped<maxVal ? clamped: maxVal);
	m_ClippingPlane = (double)clamped/((double)(maxVal - minVal));

	if(m_DataManager)
	{
		m_DataManager->update();
	}
}

void VolumeData::transparencyValueChangedSlot(int value)
{
	if(!m_VolumeRenderingProperties)
	{
		return;
	}

	m_Alpha = value / 100.0f;

	if(m_DataManager)
	{
		m_DataManager->update();
	}
}

double VolumeData::getClippingPlaneExtent()
{
	return m_ClippingPlane;
}

Plane VolumeData::getClippingPlane()
{
	Plane plane = OpenGL_Viewer::getViewPlane();
	double maxx = m_SimpleVolumeData->getMaxX();
	double maxy = m_SimpleVolumeData->getMaxY();
	double maxz = m_SimpleVolumeData->getMaxZ();
	double minx = m_SimpleVolumeData->getMinX();
	double miny = m_SimpleVolumeData->getMinY();
	double minz = m_SimpleVolumeData->getMinZ();
	double ratiox = maxx - minx;
	double ratioy = maxy - miny;
	double ratioz = maxz - minz;
	// find the maximum ratio
	double largestSide = (ratiox > ratioy ? ratiox : ratioy);
	largestSide = (largestSide > ratioz ? largestSide : ratioz);
	// normalize so the max ratio is 1.0
	ratiox /= largestSide;
	ratioy /= largestSide;
	ratioz /= largestSide;
	double diagonal = sqrt(ratiox*ratiox + ratioy*ratioy + ratioz*ratioz);
	double nearestdist = (-0.5 * diagonal) + (m_ClippingPlane * diagonal);
	plane[3] = nearestdist;
	// generate a point on the plane.
	double x, y, z;
	{
		double a = plane[0], b = plane[1], c = plane[2], d = plane[3];

		if((a<-0.000000001) || (a>0.000000001))
		{
			y = 1, z = 1;
			x = (d-(b*y+c*z)) / a;
		}
		else if((b<-0.000000001) || (b>0.000000001))
		{
			x = 1, z = 1;
			y = (d-(a*x+c*z)) / b;
		}
		else if((c<-0.000000001) || (c>0.000000001))
		{
			x = 1, y = 1;
			z = (d-(a*x+b*y)) / c;
		}
		else
		{
			return plane;    // wierd case!!
		}
	}
	// move the point to 0-1], scale by largest side, translate to real position of volume.
	x += (maxx-minx)/(largestSide * 2.0);
	y += (maxy-miny)/(largestSide * 2.0);
	z += (maxz-minz)/(largestSide * 2.0);
	x *= largestSide;
	y *= largestSide;
	z *= largestSide;
	x += minx;
	y += miny;
	z += minz;
	// update the dist with new point, keep old normal.
	plane[3] = -1*(plane[0]*x + plane[1]*y + plane[2]*z);
	return plane;
}

bool VolumeData::save(const char* filename)
{
	if(!m_SimpleVolumeData || !filename)
	{
		return false;
	}

	VolumeLoader* volumeLoader = new VolumeLoader();
	bool ret = volumeLoader->saveFile(filename, m_SimpleVolumeData);
	delete volumeLoader;
	return ret;
}

bool VolumeData::supportsFileNames(QStringList fileNames)
{
	if(fileNames.size() < 1)
	{
		return false;
	}

	int i;
	bool ret = true;

	for(i=0; i<fileNames.size(); i++)
	{
		VolumeLoader* volumeLoader = new VolumeLoader();
		ret = ret && (fileNames[i].endsWith("rawiv") ||
					  fileNames[i].endsWith("rawv") ||
					  fileNames[i].endsWith("dx")||
					  fileNames[i].endsWith("mrc"));
		delete volumeLoader;
	}

	return ret;
}

bool VolumeData::getMinMax(float* min, float* max)
{
	if(!m_SimpleVolumeData || !min || !max)
	{
		return false;
	}

	min[0] = m_SimpleVolumeData->getMinX();
	min[1] = m_SimpleVolumeData->getMinY();
	min[2] = m_SimpleVolumeData->getMinZ();
	max[0] = m_SimpleVolumeData->getMaxX();
	max[1] = m_SimpleVolumeData->getMaxY();
	max[2] = m_SimpleVolumeData->getMaxZ();
	return true;
}

bool VolumeData::parseAnimationCommand(QStringList commands, int curCommand)
{
	return false;
}

Geometry* VolumeData::getGeometry()
{
	if(m_MultiContour == 0)
	{
		return 0;
	}
	else
	{
		return m_MultiContour->getGeometry();
	}
}

void VolumeData::forceAlpha(float alpha)
{
	if(m_VolumeRenderingProperties)
	{
		m_VolumeRenderingProperties->_ui->m_TransparencySlider->setValue(alpha*100);
	}
}
