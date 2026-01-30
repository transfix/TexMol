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
#include <TexMol/Dialogs/ConstructVolumesDialog.h>
//#include <q3filedialog.h>
#include <QFileDialog>
#include <qlineedit.h>
#include <qradiobutton.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <q3buttongroup.h>
#include <qpushbutton.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
#include <DataManager/DataManager.h>
#include <PDBParser/GroupOfAtoms.h>
#include <PDBParser/CollectionData.h>
#include <Blurmaps/BlurMapsDataManager.h>

ConstructVolumes::ConstructVolumes(QWidget* parent,  const char* name, bool modal, Qt::WFlags fl)
	: ConstructVolumesDialogBase(parent, name, modal, fl)
{
}

ConstructVolumes::~ConstructVolumes()
{
}

bool ConstructVolumes::constructVolume(DataManager* dataManager, PDBParser::GroupOfAtoms* groupOfAtoms, VolumeData* vData)
{
	if(!vData)
	{
		return false;
	}

	if(groupOfAtoms == 0)
	{
		return false;
	}

	// get input values for blurring //
	int dim1 = m_Dim1SpinBox->value();
	int dim2 = m_Dim2SpinBox->value();
	int dim3 = m_Dim3SpinBox->value();
	PDBParser::GroupOfAtoms::FUNCTIONS densityType = PDBParser::GroupOfAtoms::ELECTRON_DENSITY;

	if(electronDensityRadioButton->isChecked())
	{
		densityType = PDBParser::GroupOfAtoms::ELECTRON_DENSITY;
		cout << "densityType = PDBParser::GroupOfAtoms::ELECTRON_DENSITY" << endl;
	}
	else if(hydrophobicityRadioButton->isChecked())
	{
		densityType = PDBParser::GroupOfAtoms::PER_ATOM_HYDROPHOBICITY;
		cout << "densityType = PDBParser::GroupOfAtoms::PER_ATOM_HYDROPHOBICITY" << endl;
	}
	else if(m_ElectrostaticPotentialRadioButton->isChecked())
	{
		densityType = PDBParser::GroupOfAtoms::ELECTROSTATIC_POTENTIAL;
		cout << "densityType = PDBParser::GroupOfAtoms::ELECTROSTATIC_POTENTIAL" << endl;
	}
	else
	{
		return false;    // should not happen, atleast one should be seleceted (?)
	}

	bool ok;
	double blobby = (m_BlobbyLineEdit->text()).toDouble(&ok);



	if(!ok)
	{
		return false;
	}

	bool rawV = m_RawVCheckBox->isChecked();
	PDBParser::GroupOfAtoms::GOA_TYPE colorType = PDBParser::GroupOfAtoms::ATOM;

	if(rawV)
	{
		if(m_AtomRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::ATOM;
		}

		if(m_ResidueRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::RESIDUE;
		}

		if(m_SSRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::SECONDARY_STRUCTURE;
		}

		if(m_ChainRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::CHAIN;
		}

		if(m_ProteinRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::PROTEIN;
		}

		if(m_CollectionRadioButton->isChecked())
		{
			colorType = PDBParser::GroupOfAtoms::COLLECTION;
		}
	/*Q3Err:C
		if(m_ColormapFileLineEdit->text() == 0)
	*/
		if((m_ColormapFileLineEdit->text()).isNull())
		{
			return false;
		}
	}

	PDBParser::GroupOfAtoms::RADIUS_TYPE radiusType;

	if(m_AtomRadiusRadioButton->isChecked())
	{
		radiusType = PDBParser::GroupOfAtoms::VDW_RADIUS;
	}
	else
	{
		radiusType = PDBParser::GroupOfAtoms::ROUX_RADIUS;
	}

	// create volume
	SimpleVolumeData* sData = 0;
	{
		sData = BlurMapsDataManager::getVolume(groupOfAtoms, 0, dim1, dim2, dim3, densityType, rawV, blobby, colorType, m_ColormapFileLineEdit->text().latin1(), 0, radiusType, PDBParser::ATOM_TYPE);

		if(sData == 0)
		{
			return false;
		}

		vData->setSimpleData(sData);

		if(rawV)
		{
			vData->setFileName("InternalFile_" + QString::number(dataManager->m_DataSetsAdded) + ".rawv");
		}
		else
		{
			vData->setFileName("InternalFile_" + QString::number(dataManager->m_DataSetsAdded) + ".rawiv");
		}
	}
	return true;
}

void ConstructVolumes::colorMapFileSlot()
{
  //QString cMapFile = Q3FileDialog::getOpenFileName("", "Color Map (*.cmap);;All files (*.*)");
	QString cMapFile = QFileDialog::getOpenFileName("", "Color Map (*.cmap);;All files (*.*)");
/*Q3Err:C
	if(cMapFile != 0)
*/
	if(!cMapFile.isNull())
	{
		m_ColormapFileLineEdit->setText(cMapFile);
	}
}

void ConstructVolumes::constructRawVSlot(bool enabled)
{
	m_ColoringTypeButtonGroup->setEnabled(enabled);
	m_ColormapFileLineEdit->setEnabled(enabled);
	m_ColorMapFilePushButton->setEnabled(enabled);
}

void ConstructVolumes::functionTypeChangedSlot(int buttonSelected)
{
	switch(buttonSelected) {
	case 0:
		electronDensityRadioButton->setChecked(true);
		hydrophobicityRadioButton->setChecked(false);
		m_ElectrostaticPotentialRadioButton->setChecked(false);
		break;
	case 1:
		electronDensityRadioButton->setChecked(false);
		hydrophobicityRadioButton->setChecked(true);
		m_ElectrostaticPotentialRadioButton->setChecked(false);
		break;
	case 2:
		electronDensityRadioButton->setChecked(false);
		hydrophobicityRadioButton->setChecked(false);
		m_ElectrostaticPotentialRadioButton->setChecked(true);
		break;
	}

	if(electronDensityRadioButton->isChecked())
		m_RawVGroupBox->setEnabled(true);
	else
		m_RawVGroupBox->setEnabled(false);
}
