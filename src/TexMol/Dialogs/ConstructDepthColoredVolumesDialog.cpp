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
#include <TexMol/Dialogs/ConstructDepthColoredVolumesDialog.h>
#include <qlineedit.h>
//#include <q3filedialog.h>
#include <QFileDialog>
#include <qcheckbox.h>
#include <math.h>
#include <ColorTable2/ColorTable.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <DataManager/DataManager.h>
#include <VolumeFileTypes/VolumeLoader.h>

ConstructDepthColoredVolumesDialog::ConstructDepthColoredVolumesDialog(QString inputVolumeFileName, QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: ConstructDepthColoredVolumesDialogBase(parent, name, modal, fl)
{
	m_InputVolumeFileNameLextLabel->setText(inputVolumeFileName);
	m_OutputRawVFileName = m_RawVFileNameLineEdit->text();
}

ConstructDepthColoredVolumesDialog::~ConstructDepthColoredVolumesDialog()
{
}

void ConstructDepthColoredVolumesDialog::outputRawVFileNameSlot()
{
  //m_OutputRawVFileName = Q3FileDialog::getOpenFileName("", "Volume RAWV file (*.rawv);;All files (*.*)");
  m_OutputRawVFileName = QFileDialog::getOpenFileName(this, "Open Volume file", "", "Volume RAWV file (*.rawv);;All files (*.*)");
/*Q3Err:C
	if(m_OutputRawVFileName != 0)
*/
	if(!m_OutputRawVFileName.isNull())
	{
		m_RawVFileNameLineEdit->setText(m_OutputRawVFileName);
	}
}

double ConstructDepthColoredVolumesDialog::getDistanceOfVoxel(int i, int j, int k, int width, int height, int depth)
{
	return (sqrt(((i)-width/2)*((i)-width/2)+((j)-height/2)*((j)-height/2)+((k)-depth/2)*((k)-depth/2)));
}

VolumeData* ConstructDepthColoredVolumesDialog::constructDepthColoredVolumes(DataManager* dataManager, QString volumeFileName)
{
	if(!volumeFileName.endsWith("rawiv") && !volumeFileName.endsWith("rawv"))
	{
		return 0;
	}

	VolumeData* volumeData = new VolumeData(dataManager);
/*Q3Err:CS
	if(!volumeData->read(volumeFileName.toLatin1().constData()))
*/
	if(!volumeData->read(QStringList(QString(volumeFileName.toLatin1()))))
	{
		delete volumeData;
		return 0;
	}

	VolumeData* depthColoredVolume = constructDepthColoredVolumesFromVolume(dataManager, volumeData);
	delete volumeData;
	return depthColoredVolume;
}

VolumeData* ConstructDepthColoredVolumesDialog::constructDepthColoredVolumesFromVolume(DataManager* dataManager, VolumeData* inputVolumeFile)
{
/*Q3Err:C
	if(m_RawVFileNameLineEdit->text() == 0)
*/
	if( (m_RawVFileNameLineEdit->text()).isNull() )
	{
		return 0;
	}

	if(!inputVolumeFile)
	{
		return 0;
	}

	if(!inputVolumeFile->m_SimpleVolumeData)
	{
		return 0;
	}

	// get the color table values
	int colorMapSize = 256; // new rover supports only 256  - be careful SKVINAY !
	double dColorMap[256*4];


	// arand: need to reimplement this function...
	//m_ColorTable->getTableDouble(dColorMap, colorMapSize);

	//  create depth colored volume
	SimpleVolumeData* sData = 0;
	sData = inputVolumeFile->m_SimpleVolumeData->createDepthColoredVolume(dColorMap, colorMapSize-1);

	if(!sData)
	{
		return 0;
	}

	//  create return VolumeData
	VolumeData* vData = new VolumeData(dataManager);
	vData->setSimpleData(sData);
	// write to file if needed
	QString volFileName = m_RawVFileNameLineEdit->text();

	if(volFileName != "")
	{
		if(volFileName.endsWith("rawv"))
		{
			bool ret = false;
			{
				VolumeLoader* volumeLoader = new VolumeLoader();
				ret = volumeLoader->saveFile(volFileName.toLatin1().constData(), sData);
				delete volumeLoader;
			}
			vData->setFileName(m_RawVFileNameLineEdit->text());
			return vData;
		}
	}

	vData->setFileName("InternalFile_" + QString::number(dataManager->m_DataSetsAdded) + "_"+m_InputVolumeFileNameLextLabel->text()+".rawv");
	return vData;
}

bool ConstructDepthColoredVolumesDialog::loadFile()
{
	return m_LoadDataCheckBox->isChecked();
}
