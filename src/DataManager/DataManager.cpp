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

#include <ImposterRenderer/ImposterRenderer.h>
#include <GeometryRenderer/GeometryRenderer.h>
#include <GeometryRenderer/TransparentVolumeRenderer.h>


#include <ColorTable2/ColorTable.h>
#include <DataManager/AbstractData.h>
#include <DataManager/BallAndStickDataManager/BallAndStickData.h>
#include <DataManager/DataManager.h>
#include <DataManager/NURBSDataManager/NURBSData.h>
#include <DataManager/SecondaryStructureDataManager/SecondaryStructureData.h>
#include <DataManager/SurfaceDataManager/SurfaceData.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
#include <DataManager/VolumeDataManager/VolumeRendererExtn.h>
#include <iostream>
#include <PDBParser/Atom.h>
#include <PDBParser/GroupOfAtoms.h>
#include <qapplication.h>
#include <qdatastream.h>
#include <qfile.h>
#include <q3groupbox.h>
#include <q3listbox.h>
#include <qmessagebox.h>
#include <q3textedit.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <TexMol/Animator.h>
#include <TexMol/CallEvent.h>
#include <TexMol/RendererSet.h>
#include <TexMol/Dialogs/MainWindow.h>

#if ! defined(__APPLE__)
#include <DataManager/VolumeDataManager/SplattingRendererExtn.h>
#endif

DataManager::DataManager(QSProject* qSProject, QWidget* parentWidget, Q3ListBox* dataSetsListBox, bool renderGlobalBoundingBox, bool renderDataBoundingBox, RendererSet* rendererSet)
{
	m_QSProject = qSProject;
	m_CurrentDataSet = -1;
	m_ParentPropertiesWidget = parentWidget;
	m_VolumeRendererExtn = NULL;
#if ! defined(__APPLE__)
	m_SplattingRendererExtn = NULL;
#endif
	m_Initialized = false;
	m_DataSetsListBox = dataSetsListBox;
	m_DataSetsAdded = 0;
	m_NextUniqueAvailableId = 0;
	m_BallAndStickDataArray.setAutoDelete(true);
	m_ScalarVolumeArray.setAutoDelete(true);
	m_VectorVolumeArray.setAutoDelete(true);
	m_SurfaceArray.setAutoDelete(true);
	m_NURBSArray.setAutoDelete(true);
	m_SecondaryStructureArray.setAutoDelete(true);
	m_RenderGlobalBoundingBox = renderGlobalBoundingBox;
	m_RenderDataBoundingBox = renderDataBoundingBox;
	m_RendererSet = rendererSet;
	m_RenderingMode = Animator::NORMAL_RENDERING_MODE;
}

DataManager::~DataManager()
{
	if( m_VolumeRendererExtn ) delete m_VolumeRendererExtn;
	m_VolumeRendererExtn = 0;
#if ! defined(__APPLE__)
	if( m_SplattingRendererExtn ) delete m_SplattingRendererExtn;
	m_SplattingRendererExtn=0;
#endif
}

void DataManager::init()
{
	//m_IsocontourRenderer->initRenderer();
	if (!m_Initialized)   //to let other renderers init themselves ?
	{
		m_Initialized = true;
	//	m_VolumeRendererExtn->initRenderer();
#if ! defined(__APPLE__)
	//	m_SplattingRendererExtn->initRenderer();
		//fprintf( stderr, "splatting renderer initialized\n");
#endif
	}
}

QString* DataManager::getDataTypes()
{
	return strDataTypes;
}

int DataManager::getNumberOfDataTypes()
{
	return NUMBER_OF_DATA_TYPES;
}

bool DataManager::checkVolumeRendererAllocation()
{
	m_RendererSet->getCurrentRenderer()->makeCurrent();

	if(m_VolumeRendererExtn == NULL ) {
		m_VolumeRendererExtn = new VolumeRendererExtn();
		m_VolumeRendererExtn->initRenderer();
	}
#if ! defined(__APPLE__)
	if(m_SplattingRendererExtn == NULL ) {
		m_SplattingRendererExtn = new SplattingRendererExtn(4);
		m_SplattingRendererExtn->initRenderer();
	}
#endif
}

bool DataManager::addNewDataSet(QStringList fileNames, bool instantVisibility)
{
	m_RendererSet->getCurrentRenderer()->makeCurrent();

	QString fileName = fileNames[0];
	// add new data set
	if (BallAndStickData::supportsFileNames(fileNames))
	{
		BallAndStickData* ballAndStickData = new BallAndStickData(this);
		if (!ballAndStickData->read(fileNames))
		{
			delete ballAndStickData;
			return false;
		}
		if (!addData(ballAndStickData, fileName))
		{
			delete ballAndStickData;
			return false;
		}
		else
		{
			ballAndStickData->renderEnabledSlot(true);
			return true;
		}
	}
	if (VolumeData::supportsFileNames(fileNames))
	{
		VolumeData* volumeData = new VolumeData(this);
		if (!volumeData->read(fileNames))
		{
			delete volumeData;
			return false;
		}
		if (!addData(volumeData, fileName))
		{
			delete volumeData;
			return false;
		}
		else
		{
			return true;
		}
	}
	if (SurfaceData::supportsFileNames(fileNames))
	{
		SurfaceData* surfaceData = new SurfaceData(this);
		if (!surfaceData->read(fileNames))
		{
			delete surfaceData;
			return false;
		}
		if (!addData(surfaceData, fileName))
		{
			delete surfaceData;
			return false;
		}
		else
		{
			surfaceData->renderEnabledSlot(true);
			return true;
		}
	}
	if (NURBSData::supportsFileNames(fileNames))
	{
		NURBSData* nurbsData = new NURBSData(this);
		if (!nurbsData->read(fileNames))
		{
			delete nurbsData;
			return false;
		}
		if (!addData(nurbsData, fileName))
		{
			delete nurbsData;
			return false;
		}
		else
		{
			return true;
		}
	}
	// unknown file extension. Should not have reached till this point as user
	// should not be allowed to select such files.
	return false;
}

void DataManager::setCurrentRendererId(int currentRendererId)
{
	m_CurrentRendererId = currentRendererId;
}

bool DataManager::render(int rendererId)
{
	CCVTexMolGeometryRenderer::GeometryRenderer::resetTransparentGeometry();
	int tempCurrentDataSet = m_CurrentDataSet;
	int tempCurrentDataType = m_CurrentDataType;
	// first render all the opaque data from all the data sets
	m_GlobalBoundingBox.reset();
	for (int i=0; i<m_BallAndStickDataArray.count(); i++)
	{
		if ((m_BallAndStickDataArray.at(i))->isEnabled(rendererId))
		{
			(m_BallAndStickDataArray.at(i))->beginRender();
			(m_BallAndStickDataArray.at(i))->render();
			(m_BallAndStickDataArray.at(i))->endRender(m_RenderDataBoundingBox);
		}
	}
	for (int i=0; i<m_SurfaceArray.count(); i++)
	{
		if ((m_SurfaceArray.at(i))->isEnabled(rendererId))
		{
			(m_SurfaceArray.at(i))->beginRender();
			(m_SurfaceArray.at(i))->render();
			(m_SurfaceArray.at(i))->endRender(m_RenderDataBoundingBox);
		}
	}
	for (int i=0; i<m_NURBSArray.count(); i++)
	{
		if ((m_NURBSArray.at(i))->isEnabled(rendererId))
		{
			(m_NURBSArray.at(i))->beginRender();
			(m_NURBSArray.at(i))->render();
			(m_NURBSArray.at(i))->endRender(m_RenderDataBoundingBox);
		}
	}
	for (int i=0; i<m_SecondaryStructureArray.count(); i++)
	{
		if ((m_SecondaryStructureArray.at(i))->isEnabled(rendererId))
		{
			(m_SecondaryStructureArray.at(i))->beginRender();
			(m_SecondaryStructureArray.at(i))->render();
			(m_SecondaryStructureArray.at(i))->endRender(m_RenderDataBoundingBox);
		}
	}
	// now combine the volume data from all data sets and render
	// assume that the combine operation takes place each time
	// there is some change in the volume data sets. So now we just render
	// the data sets
	/*	I dont know how to do this :(
		We will currently allow only one data set to be
		rendered at a time. Otherwise we need to do painful
		things like sorting, assume no overlap  etc.
	*/
	bool renderedVolume = false;
	for (int i=0; i<m_ScalarVolumeArray.count(); i++)
	{
		VolumeData* temp = m_ScalarVolumeArray.at(i);
		if (temp->isEnabled(rendererId))
		{
			bool transparentRender = (temp->getAlpha() < 1.0f);
			(temp)->beginRender();
			if (temp->getRenderingType() == VOLUME_RENDERING)
			{
				//volume render
				if (!temp->isLoaded())
				{
					if (!m_VolumeRendererExtn->loadData(temp))
					{
						return false;
					}
				}
				if (transparentRender)
				{
					// transparent render
					TransparentVolumeRenderer::render(temp, m_VolumeRendererExtn);
					m_VolumeRendererExtn->renderVolume(temp->getClippingPlaneExtent());
				}
				else
				{
					// old, non-transparent render
					temp->renderContours();
					m_VolumeRendererExtn->renderVolume(temp->getClippingPlaneExtent());
				}
			}
#if ! defined(__APPLE__)
			else if (temp->getRenderingType() == SPLATTING)
			{
				m_SplattingRendererExtn->renderVolume(temp);
				renderedVolume = true;
			}
#endif
			else
			{
				renderedVolume = true;
			}
			(temp)->endRender(m_RenderDataBoundingBox);
		}
	}
	if (!renderedVolume)
	{
		// now try the vector valued datasets
		for (int i=0; i<m_VectorVolumeArray.count(); i++)
		{
			VolumeData* temp = m_VectorVolumeArray.at(i);
			if (temp->isEnabled(rendererId))
			{
				bool transparentRender = (temp->getAlpha() < 1.0f);
				(temp)->beginRender();
				//volume render
				if (!temp->isLoaded())
				{
					if (!m_VolumeRendererExtn->loadData(temp))
					{
						return false;
					}
				}
				if (transparentRender)
				{
					// transparent render
					TransparentVolumeRenderer::render(temp, m_VolumeRendererExtn);
				}
				else
				{
					// old, non-transparent render
					temp->renderContours();
					m_VolumeRendererExtn->renderVolume(temp->getClippingPlaneExtent());
				}
				renderedVolume = true;
				(temp)->endRender(m_RenderDataBoundingBox);
			}
		}
	}
	CCVTexMolGeometryRenderer::GeometryRenderer::renderTransparentGeometry();
	return true;
}

// Adding different data types
bool DataManager::addData(BallAndStickData* ballAndStickData, QString fileName)
{
	if (ballAndStickData == 0)
	{
		return false;
	}
	ballAndStickData->setFileName(fileName);
	ballAndStickData->m_UniqueDataSetId = m_NextUniqueAvailableId;
	m_NextUniqueAvailableId++;
	m_BallAndStickDataArray.append(ballAndStickData);
	m_DataSetMapList.append(new DataSetMap(ballAndStickData->m_UniqueDataSetId, GOA));
	m_DataSetsAdded++;
	return true;
}

bool DataManager::addData(VolumeData* volumeData, QString fileName)
{
	if (!volumeData)
	{
		return false;
	}
	if (!volumeData->m_SimpleVolumeData)
	{
		return false;
	}
	if (volumeData->m_SimpleVolumeData->getNumberOfVariables() < 1)
	{
		return false;
	}

	//====================================================
	// check whether volume renderer allocated or not, if not allocate
        // CHA: I moved this from constructor to here
        // to prevent opengl extension init fail problem,
	// Also this works for constructed volume from PDB
        checkVolumeRendererAllocation();
	//====================================================

	// remove previous volumes
	// go through the volumes in the data set list, get their indices
	//		call MoleculeVizMainWindow::deleteAndUpdateUI(int selectedIndex);
	// delete the data set, loop till all volumes are removed
	bool removedPrevVolume = true;
	while (removedPrevVolume)
	{
		removedPrevVolume = false;
		// go through the list of loaded files
		int c = m_DataSetMapList.count();

		for (int i=0; i<c; i++)
		{
			int dataSetType = (m_DataSetMapList.at(i))->getDataType();
			if ((dataSetType ==SCALAR_VOLUME) || (dataSetType ==VECTOR_VOLUME))
			{

			//	Q3GroupBox* w = (Q3GroupBox*)m_ParentPropertiesWidget->parentWidget();
				QWidget* w = m_ParentPropertiesWidget->parentWidget();
				if (w == 0)
				{
					return false;
				}

				// arand: this call was not working...
				//CallEvent::call(w->parentWidget(), "deleteAndUpdateUI", std::vector<boost::any>(1,boost::any(i)));
				// this works...
				((MainWindow *)w->parentWidget())->deleteAndUpdateUI(i);

				removedPrevVolume = true;
				break;
			}
		}
	}

	volumeData->setFileName(fileName);
	if (volumeData->m_SimpleVolumeData->getNumberOfVariables() < 4)
	{
		m_ScalarVolumeArray.append(volumeData);
		volumeData->m_UniqueDataSetId = m_NextUniqueAvailableId;
		m_NextUniqueAvailableId++;
		m_DataSetMapList.append(new DataSetMap(volumeData->m_UniqueDataSetId, SCALAR_VOLUME));
		m_DataSetsAdded++;
		return true;
	}
	else
	{
		m_VectorVolumeArray.append(volumeData);
		volumeData->m_UniqueDataSetId = m_NextUniqueAvailableId;
		m_NextUniqueAvailableId++;
		m_DataSetMapList.append(new DataSetMap(volumeData->m_UniqueDataSetId, VECTOR_VOLUME));
		m_DataSetsAdded++;
		return true;
	}
}

bool DataManager::addData(SurfaceData* surfaceData, QString fileName)
{
	if (surfaceData == 0)
	{
		return false;
	}
	surfaceData->setFileName(fileName);
	surfaceData->m_UniqueDataSetId = m_NextUniqueAvailableId;
	m_NextUniqueAvailableId++;
	m_SurfaceArray.append(surfaceData);
	m_DataSetMapList.append(new DataSetMap(surfaceData->m_UniqueDataSetId, SURFACE));
	m_DataSetsAdded++;
	return true;
}

bool DataManager::addData(SecondaryStructureData* secondaryStructureData, QString fileName)
{
	if (secondaryStructureData == 0)
	{
		return false;
	}
	secondaryStructureData->setFileName(fileName);
	secondaryStructureData->m_UniqueDataSetId = m_NextUniqueAvailableId;
	m_NextUniqueAvailableId++;
	m_SecondaryStructureArray.append(secondaryStructureData);
	m_DataSetMapList.append(new DataSetMap(secondaryStructureData->m_UniqueDataSetId, SECONDARYSTRUCTURE));
	m_DataSetsAdded++;
	return true;
}


bool DataManager::addData(NURBSData* nurbsData, QString fileName)
{
	if (nurbsData == 0)
	{
		return false;
	}
	nurbsData->setFileName(fileName);
	nurbsData->m_UniqueDataSetId = m_NextUniqueAvailableId;
	m_NextUniqueAvailableId++;
	m_NURBSArray.append(nurbsData);
	m_DataSetMapList.append(new DataSetMap(nurbsData->m_UniqueDataSetId, NURB));
	m_DataSetsAdded++;
	return true;
}

int DataManager::getNumberOfDataSets()
{
	return m_DataSetMapList.count();
}

QString DataManager::getDataSetName(int index)
{
	AbstractData* selectedData = getSelectedData(index);
	if (!selectedData)
	{
		return 0;
	}
	return selectedData->getFileName();
}

int DataManager::getDataSetIndex(int index)
{
	AbstractData* selectedData = getSelectedData(index);
	if (!selectedData)
	{
		return 0;
	}
	return selectedData->m_UniqueDataSetId;
}

AbstractData* DataManager::getSelectedData(int selectedDataIndex)
{
	AbstractData* selectedData = 0;
	DataSetMap* temp = m_DataSetMapList.at(selectedDataIndex);
	if (!temp)
	{
		return 0;
	}
	int dataSetType = temp->getDataType();
	int dataSetIndex = temp->getIndex();
	switch (dataSetType)
	{
	case GOA:
	{
		int n = m_BallAndStickDataArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_BallAndStickDataArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_BallAndStickDataArray.at(i);
				break;
			}
		}
	}
	break;
	case SCALAR_VOLUME:
	{
		int n = m_ScalarVolumeArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_ScalarVolumeArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_ScalarVolumeArray.at(i);
				break;
			}
		}
	}
	break;
	case VECTOR_VOLUME:
	{
		int n = m_VectorVolumeArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_VectorVolumeArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_VectorVolumeArray.at(i);
				break;
			}
		}
	}
	break;
	case SURFACE:
	{
		int n = m_SurfaceArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_SurfaceArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_SurfaceArray.at(i);
				break;
			}
		}
	}
	break;
	case NURB:
	{
		int n = m_NURBSArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_NURBSArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_NURBSArray.at(i);
				break;
			}
		}
	}
	break;
	case SECONDARYSTRUCTURE:
	{
		int n = m_SecondaryStructureArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_SecondaryStructureArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				selectedData = m_SecondaryStructureArray.at(i);
				break;
			}
		}
	}
	break;
	default:
		break;
	}
	return selectedData;
}

void DataManager::deleteDataSet()
{
	DataSetMap* temp = m_DataSetMapList.at(m_SelectedIndex);
	int dataSetType = temp->getDataType();
	int dataSetIndex = temp->getIndex();
	switch (dataSetType)
	{
	case GOA:
	{
		int n = m_BallAndStickDataArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_BallAndStickDataArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_BallAndStickDataArray.remove(i);
				break;
			}
		}
	}
	break;
	case SCALAR_VOLUME:
	{
		int n = m_ScalarVolumeArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_ScalarVolumeArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_ScalarVolumeArray.remove(i);
				break;
			}
		}
	}
	break;
	case VECTOR_VOLUME:
	{
		int n = m_VectorVolumeArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_VectorVolumeArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_VectorVolumeArray.remove(i);
				break;
			}
		}
	}
	break;
	case SURFACE:
	{
		int n = m_SurfaceArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_SurfaceArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_SurfaceArray.remove(i);
				break;
			}
		}
	}
	break;
	case NURB:
	{
		int n = m_NURBSArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_NURBSArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_NURBSArray.remove(i);
				break;
			}
		}
	}
	break;
	case SECONDARYSTRUCTURE:
	{
		int n = m_SecondaryStructureArray.count();
		for (int i=0; i<n; i++)
		{
			if (m_SecondaryStructureArray.at(i)->m_UniqueDataSetId == dataSetIndex)
			{
				m_SecondaryStructureArray.remove(i);
				break;
			}
		}
	}
	break;
	default:
		break;
	}
	m_DataSetMapList.remove(m_SelectedIndex);
}

QWidget* DataManager::getPropertiesWidget()
{
	// also make sure the render check box is set properly for the new
	// properties widget
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return 0;
	}
	selectedData->setVisibilityInPropertiesWidget();
	return selectedData->getPropertiesWidget();
}

bool DataManager::updateGL()
{
//	Q3GroupBox* w = (Q3GroupBox*)m_ParentPropertiesWidget->parentWidget();
	QWidget* w = (QWidget*)m_ParentPropertiesWidget->parentWidget();
	if (w == 0)
	{
		return false;
	}
	CallEvent::call(w->parentWidget(),"redraw");
	return true;
}

void DataManager::Test_Call_VolumeRender(void)
{
	m_VolumeRendererExtn->renderVolume(0.5);
}

bool DataManager::uploadColorMap(unsigned char* colorMap, int size)
{
	// new rover doesnt use size anymore :(
	if (!m_VolumeRendererExtn)
	{
		return false;
	}
	return m_VolumeRendererExtn->uploadColorMap(colorMap);
}

bool DataManager::uploadColorMap(double* colorMap, int size)
{
	// new rover doesnt use size anymore :(
#if ! defined(__APPLE__)
	if (!m_SplattingRendererExtn)
	{
		return false;
	}
	return m_SplattingRendererExtn->uploadColorMap(colorMap, size);
#else
	return true;
#endif
}

int DataManager::getCurrentRenderer()
{
	if (m_CurrentRendererId == 0)
	{
		return m_CurrentRendererId;
	}
	return m_CurrentRendererId;
}

void DataManager::setVisibilityInPropertiesWidget()
{
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return;
	}
	selectedData->setVisibilityInPropertiesWidget();
	return;
}

bool DataManager::setVisible(bool render)
{
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return false;
	}
	selectedData->renderEnabledSlot(render);
	return true;
}

bool DataManager::setVisible(int index, bool render)
{
	AbstractData* selectedData = getSelectedData(index);
	if (!selectedData)
	{
		return false;
	}
	selectedData->renderEnabledSlot(render);
	return true;
}

void DataManager::setSelectedIndex(int selectedIndex)
{
	m_SelectedIndex = selectedIndex;
}

void DataManager::setDataSetIndex(int dataSetIndex)
{
	for(int i = 0; i < m_DataSetMapList.count(); i++)
	{
		if(m_DataSetMapList.at(i)->getIndex() == dataSetIndex)
		{
			m_SelectedIndex = i;
			return;
		}
	}
	m_SelectedIndex = -1;
}

void DataManager::updateUIWithNewData()
{
//	Q3GroupBox* w = (Q3GroupBox*)m_ParentPropertiesWidget->parentWidget();
	QWidget* w = (QWidget*)m_ParentPropertiesWidget->parentWidget();
	if (w == 0)
	{
		return;
	}
	CallEvent::call(w->parentWidget(),"updateUIWithNewData");
}

int DataManager::getCurrentDataSetType()
{
	DataSetMap* temp = m_DataSetMapList.at(m_SelectedIndex);
	if (!temp)
	{
		return -1;
	}
	return temp->getDataType();
}

int DataManager::getCurrentDataSetIndex()
{
	DataSetMap* temp = m_DataSetMapList.at(m_SelectedIndex);
	if (!temp)
	{
		return -1;
	}
	return temp->getIndex();
}

BallAndStickData* DataManager::getBallAndStickData(int id)
{
	int n = m_BallAndStickDataArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_BallAndStickDataArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_BallAndStickDataArray.at(i);
		}
	}
	return 0;
}

VolumeData* DataManager::getScalarVolumeData(int id)
{
	int n = m_ScalarVolumeArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_ScalarVolumeArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_ScalarVolumeArray.at(i);
		}
	}
	return 0;
}

VolumeData* DataManager::getVectorVolumeData(int id)
{
	int n = m_VectorVolumeArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_VectorVolumeArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_VectorVolumeArray.at(i);
		}
	}
	return 0;
}

SurfaceData* DataManager::getSurfaceData(int id)
{
	int n = m_SurfaceArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_SurfaceArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_SurfaceArray.at(i);
		}
	}
	return 0;
}

NURBSData* DataManager::getNURBSData(int id)
{
	int n = m_NURBSArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_NURBSArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_NURBSArray.at(i);
		}
	}
	return 0;
}

SecondaryStructureData* DataManager::getSecondaryStructureData(int id)
{
	int n = m_SecondaryStructureArray.count();
	for (int i=0; i<n; i++)
	{
		if (m_SecondaryStructureArray.at(i)->m_UniqueDataSetId == id)
		{
			return m_SecondaryStructureArray.at(i);
		}
	}
	return 0;
}

QString DataManager::getSelection(CCVOpenGLMath::Ray targetVector)
{
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return 0;
	}
	return selectedData->getSelection(targetVector);
}

AbstractData* DataManager::getDataFromUniqueID(int uniqueID)
{
	BallAndStickData* ballAndStickData = getBallAndStickData(uniqueID);
	if (ballAndStickData)
	{
		return ballAndStickData;
	}
	SurfaceData* surfaceData = getSurfaceData(uniqueID);
	if (surfaceData)
	{
		return surfaceData;
	}
	NURBSData* nurbsData = getNURBSData(uniqueID);
	if (nurbsData)
	{
		return nurbsData;
	}
	VolumeData* scalarVolumeData = getScalarVolumeData(uniqueID);
	if (scalarVolumeData)
	{
		return scalarVolumeData;
	}
	VolumeData* vectorVolumeData = getVectorVolumeData(uniqueID);
	if (vectorVolumeData)
	{
		return vectorVolumeData;
	}
	SecondaryStructureData* secondaryData = getSecondaryStructureData(uniqueID);
	if (secondaryData)
	{
		return secondaryData;
	}
	return 0;
}

bool DataManager::parseAnimationCommand(QStringList commands, int curCommand)
{
	if (commands.size() < curCommand + 1)
	{
		return false;    // need atleast 1
	}
	// now check each data type, if any, send command to appropriate data.
	if (strstr(commands[curCommand], "BALL_AND_STICK_DATA") ||
			strstr(commands[curCommand], "SURFACE_DATA") ||
			strstr(commands[curCommand], "NURBS_DATA") ||
			strstr(commands[curCommand], "VOLUME_DATA") ||
			strstr(commands[curCommand], "SECONDARYSTRUCTURE_DATA"))
	{
		if (commands.size() < curCommand + 3)
		{
			return false;    // need atleast ID and some command
		}
		AbstractData* abstractData = getDataFromUniqueID(atoi(commands[curCommand+1].latin1()));
		if (!abstractData)
		{
			return false;
		}
		abstractData->parseAnimationCommand(commands, curCommand+2);
	}
	return true;
}

CCVOpenGLMath::Matrix DataManager::getTransformation(int selectedIndex)
{
	CCVOpenGLMath::Matrix m;
	if (selectedIndex == -1)
	{
		return m;
	}
	AbstractData* selectedData = getSelectedData(selectedIndex);
	if (!selectedData)
	{
		return m;
	}
	return selectedData->getTransformation();
}

bool DataManager::setTransformation(int selectedIndex, CCVOpenGLMath::Matrix m)
{
	if (selectedIndex == -1)
	{
		return false;
	}
	AbstractData* selectedData = getSelectedData(selectedIndex);
	if (!selectedData)
	{
		return false;
	}
	selectedData->setTransformation(m);
	return true;
}

bool DataManager::addTransformation(int selectedIndex, CCVOpenGLMath::Matrix m, float scale)
{
	if (selectedIndex == -1)
	{
		return false;
	}
	AbstractData* selectedData = getSelectedData(selectedIndex);
	if (!selectedData)
	{
		return false;
	}
	selectedData->addTransformation(m, scale);
	return true;
}

bool DataManager::save(int selectedIndex, const char* filename)
{
	AbstractData* selectedData = getSelectedData(selectedIndex);
	if (!selectedData)
	{
		return false;
	}
	return selectedData->save(filename);
}

void DataManager::pan(CCVOpenGLMath::Vector v)
{
	if (m_SelectedIndex == -1)
	{
		return;
	}
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return;
	}
	selectedData->addPan(v);
	v.print();
}

void DataManager::rotate(CCVOpenGLMath::Matrix m)
{
 	if (m_SelectedIndex == -1)
	{
		return;
	}
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return;
	}
//	selectedData->addRotation(m);
	// temporal implementation : need a function to get center
	float min[3], max[3];
	selectedData->getDataMinMax( min, max );
	selectedData->addRotation(m, CCVOpenGLMath::Vector( (min[0]+max[0])*0.5f, (min[1]+max[1])*0.5f, (min[2]+max[2])*0.5f, 0.0f ) );
	m.print();
}

void DataManager::scale(float scale)
{
	if (m_SelectedIndex == -1)
	{
		return;
	}
	AbstractData* selectedData = getSelectedData(m_SelectedIndex);
	if (!selectedData)
	{
		return;
	}
	selectedData->addScale(scale);
	printf("scalar is %f\n", scale);
}

void DataManager::globalBoundingBoxSlot(bool render)
{
	m_RenderGlobalBoundingBox = render;
}

void DataManager::dataBoundingBoxSlot(bool render)
{
	m_RenderDataBoundingBox = render;
}

bool DataManager::mousePress3DEvent(View* view, QMouseEvent* e)
{
	int n = m_BallAndStickDataArray.count();
	for (int i=0; i<n; i++)
		if (m_BallAndStickDataArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	n = m_ScalarVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_ScalarVolumeArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	n = m_VectorVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_VectorVolumeArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	n = m_SurfaceArray.count();
	for (int i=0; i<n; i++)
		if (m_SurfaceArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	n = m_NURBSArray.count();
	for (int i=0; i<n; i++)
		if (m_NURBSArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	n = m_SecondaryStructureArray.count();
	for (int i=0; i<n; i++)
		if (m_SecondaryStructureArray.at(i)->mousePress3DEvent(view, e))
		{
			return true;
		}
	return false;
}

bool DataManager::mouseRelease3DEvent(View* view, QMouseEvent* e)
{
	int n;
	n = m_BallAndStickDataArray.count();
	for (int i=0; i<n; i++)
		if (m_BallAndStickDataArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	n = m_ScalarVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_ScalarVolumeArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	n = m_VectorVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_VectorVolumeArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	n = m_SurfaceArray.count();
	for (int i=0; i<n; i++)
		if (m_SurfaceArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	n = m_NURBSArray.count();
	for (int i=0; i<n; i++)
		if (m_NURBSArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	n = m_SecondaryStructureArray.count();
	for (int i=0; i<n; i++)
		if (m_SecondaryStructureArray.at(i)->mouseRelease3DEvent(view, e))
		{
			return true;
		}
	return false;
}

bool DataManager::mouseDoubleClick3DEvent(View* view, QMouseEvent* e)
{
	int n;
	n = m_BallAndStickDataArray.count();
	for (int i=0; i<n; i++)
		if (m_BallAndStickDataArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	n = m_ScalarVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_ScalarVolumeArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	n = m_VectorVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_VectorVolumeArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	n = m_SurfaceArray.count();
	for (int i=0; i<n; i++)
		if (m_SurfaceArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	n = m_NURBSArray.count();
	for (int i=0; i<n; i++)
		if (m_NURBSArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	n = m_SecondaryStructureArray.count();
	for (int i=0; i<n; i++)
		if (m_SecondaryStructureArray.at(i)->mouseDoubleClick3DEvent(view, e))
		{
			return true;
		}
	return false;
}

bool DataManager::mouseMove3DEvent(View* view, QMouseEvent* e)
{
	int n;
	n = m_BallAndStickDataArray.count();
	for (int i=0; i<n; i++)
		if (m_BallAndStickDataArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	n = m_ScalarVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_ScalarVolumeArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	n = m_VectorVolumeArray.count();
	for (int i=0; i<n; i++)
		if (m_VectorVolumeArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	n = m_SurfaceArray.count();
	for (int i=0; i<n; i++)
		if (m_SurfaceArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	n = m_NURBSArray.count();
	for (int i=0; i<n; i++)
		if (m_NURBSArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	n = m_SecondaryStructureArray.count();
	for (int i=0; i<n; i++)
		if (m_SecondaryStructureArray.at(i)->mouseMove3DEvent(view, e))
		{
			return true;
		}
	return false;
}

VolumeData* DataManager::getScalarVolumeData()
{
	return m_ScalarVolumeArray.at(m_ScalarVolumeArray.count()-1);
}

QSProject* DataManager::getQSProject()
{
	return m_QSProject;
}

void DataManager::debugPrint()
{
	for (int i=0; i<m_SurfaceArray.count(); i++)
	{
		SurfaceData* data = m_SurfaceArray.at(i);
		CCVOpenGLMath::Matrix transformation = data->getTransformation();
		// get rotation direction
		CCVOpenGLMath::Vector lookAt(0,0,-1,0);
		CCVOpenGLMath::Vector pos(0,0,0,1);
		lookAt = transformation * lookAt;
		pos = transformation * pos;
		std::cerr << i << ": ROT (" << lookAt[0] << "," << lookAt[1] << ","<< lookAt[2] << ")\n";
		std::cerr << i << ": POS (" << pos[0] << "," << pos[1] << ","<< pos[2] << ")\n";
	}
}
