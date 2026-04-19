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
// Qt3Support removed
#include <QFileDialog>

#include <TexMol/Dialogs/ConstructCurvaturesDialog.h>
#include <DataManager/SurfaceDataManager/SurfaceData.h>
//#include <q3filedialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qradiobutton.h>
#include <SimpleVolumeData/SimpleVolumeData.h>
#include <Contouring/MultiContour.h>
#include <GeometryFileTypes/GeometryLoader.h>
#include <DataManager/VolumeDataManager/VolumeData.h>
#include <SimpleVolumeData/SimpleVolumeDataIsocontourer.h>
#include <PDBParser/CollectionData.h>
#include <Blurmaps/BlurMapsDataManager.h>
#include <PDBParser/GroupOfAtoms.h>
#include <GOAFileTypes/GOALoader.h>

ConstructCurvaturesDialog::ConstructCurvaturesDialog(QWidget* parent,  const char* name, bool modal, Qt::WindowFlags fl)
	: ConstructCurvaturesDialogBase(parent, name, modal, fl)
{
}

ConstructCurvaturesDialog::~ConstructCurvaturesDialog()
{
}

bool ConstructCurvaturesDialog::constructCurvature()
{
/*Q3Err:C
	if(m_PDBFileNameLineEdit->text() == 0)
*/
	if( (m_PDBFileNameLineEdit->text()).isNull() )
	{
		fprintf( stderr, "PDB in-file name is not setted\n");
		return false;
	}
/*Q3Err:C
	if(m_MeanCurvatureFileLineEdit->text() == 0)
*/
	if( (m_MeanCurvatureFileLineEdit->text()).isNull() )
	{
		fprintf( stderr, "Mean curvature out-file name is not setted\n");
		return false;
	}
/*Q3Err:C
	if(m_GaussianCurvatureFileLineEdit->text() == 0)
*/
	if( (m_GaussianCurvatureFileLineEdit->text()).isNull() )
	{
		fprintf( stderr, "Gaussian curvature out-file name is not setted\n");
		return false;
	}

	QString inputPDBFileName = m_PDBFileNameLineEdit->text();
	QString outputMeanCurvatureSurfaceFileName = m_MeanCurvatureFileLineEdit->text();
	QString outputGaussianCurvatureSurfaceFileName = m_GaussianCurvatureFileLineEdit->text();
	QString outputMeanCurvatureFileName;
	QString outputGaussianCurvatureFileName;

	if(!outputMeanCurvatureSurfaceFileName.endsWith(".rawnc"))
	{
		outputMeanCurvatureSurfaceFileName.append(".rawnc");
		outputMeanCurvatureFileName = outputMeanCurvatureSurfaceFileName + "_mean.txt";
	}
	else
	{
		int len = outputMeanCurvatureSurfaceFileName.length();
		outputMeanCurvatureFileName = outputMeanCurvatureSurfaceFileName.left(len-6) + "_mean.txt";
	}
	if(!outputGaussianCurvatureSurfaceFileName.endsWith(".rawnc"))
	{
		outputGaussianCurvatureSurfaceFileName.append(".rawnc");
		outputGaussianCurvatureFileName = outputGaussianCurvatureSurfaceFileName + "_gaussian.txt";
	}
	else
	{
		int len = outputGaussianCurvatureSurfaceFileName.length();
		outputGaussianCurvatureFileName = outputGaussianCurvatureSurfaceFileName.left(len-6) + "_gaussian.txt";
	}

	int dim1 = m_Dim1SpinBox->value();
	int dim2 = m_Dim2SpinBox->value();
	int dim3 = m_Dim3SpinBox->value();
	bool ok;
	double blobby = (m_BlobbyLineEdit->text()).toDouble(&ok);

	if(!ok)
	{
		return false;
	}

	int numberOfGridDivisions = (m_NumberOfGridDivisionsLineEdit->text()).toInt(&ok);

	if(!ok)
	{
		return false;
	}

	double maxFunctionError = (m_MaximumFunctionErrorLineEdit->text()).toDouble(&ok);

	if(!ok)
	{
		return false;
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

	if(m_IsocontourFileRadioButton->isChecked())
	{
	/*Q3Err:C
		if(m_IsocontourFileLineEdit->text() == 0)
	*/
		if( (m_IsocontourFileLineEdit->text()).isNull() )
		{
			return false;
		}

		QString inputIsocontourFileName = m_IsocontourFileLineEdit->text();
		return BlurMapsDataManager::getCurvaturesFromIsocontourFile(inputPDBFileName.toLatin1().constData(), dim1, dim2, dim3,
				blobby, inputIsocontourFileName.toLatin1().constData(), outputMeanCurvatureSurfaceFileName.toLatin1().constData(),
				outputGaussianCurvatureSurfaceFileName.toLatin1().constData(),
				outputMeanCurvatureFileName.toLatin1().constData(),
				numberOfGridDivisions, maxFunctionError, radiusType, PDBParser::ATOM_TYPE);
	}
	else
	{
		double isovalue = (m_IsovalueLineEdit->text()).toDouble(&ok);

		if(!ok)
		{
			return false;
		}

		// create vol file
		SimpleVolumeData* sData;
		{
			GOALoader* goaLoader = new GOALoader();
			PDBParser::GroupOfAtoms* molecule = goaLoader->loadFile(inputPDBFileName.toLatin1().constData());
			delete goaLoader;

			if(!molecule)
			{
				return false;
			}

			sData = BlurMapsDataManager::getVolume(molecule, "check.rawiv", dim1, dim2, dim3, PDBParser::GroupOfAtoms::ELECTRON_DENSITY, false, blobby, PDBParser::GroupOfAtoms::ATOM, 0, 0, radiusType, PDBParser::ATOM_TYPE);
			delete molecule;

			if(!sData)
			{
				return false;
			}
		}
		Geometry* geometry = SimpleVolumeDataIsocontourer::getIsocontour(sData, isovalue);

		if(geometry == 0)
		{
			return false;
		}

		// get curvatures
		return BlurMapsDataManager::getCurvatures(inputPDBFileName.toLatin1().constData(), dim1, dim2, dim3,
				blobby, geometry, outputMeanCurvatureSurfaceFileName.toLatin1().constData(),
				outputGaussianCurvatureSurfaceFileName.toLatin1().constData(),
				outputMeanCurvatureFileName.toLatin1().constData(),
				numberOfGridDivisions, maxFunctionError, radiusType, PDBParser::ATOM_TYPE);
	}

	return false;
}

void ConstructCurvaturesDialog::inputPDBFileSlot()
{
  //QString inputPDBFileName = Q3FileDialog::getOpenFileName("", "Molecular Structure (*.pdb *.pqr);;PQR (*.pqr);;PDB (*.pdb);;All files (*.*)");
  QString inputPDBFileName = QFileDialog::getOpenFileName(this, "Open PDB/PQR file", "", "Molecular Structure (*.pdb *.pqr);;PQR (*.pqr);;PDB (*.pdb);;All files (*.*)");
/*Q3Err:C
	if(inputPDBFileName != 0)
*/
	if(!inputPDBFileName.isNull())
	{
		m_PDBFileNameLineEdit->setText(inputPDBFileName);
	}
}

void ConstructCurvaturesDialog::inputIsocontourFileSlot()
{
  //QString inputIsocontourFileName = Q3FileDialog::getOpenFileName("", "Surface file (*.raw);;All files (*.*)");
  QString inputIsocontourFileName = QFileDialog::getOpenFileName(this, "Open Isocontour file", "", "Surface file (*.raw);;All files (*.*)");

/*Q3Err:C
	if(inputIsocontourFileName != 0)
*/
	if(!inputIsocontourFileName.isNull())
	{
		m_IsocontourFileLineEdit->setText(inputIsocontourFileName);
	}
}

void ConstructCurvaturesDialog::outputMeanCurvatureFileSlot()
{
  //QString outputMeanCurvatureFileName = Q3FileDialog::getSaveFileName("", "Surface file (*.raw);;All files (*.*)");
  QString outputMeanCurvatureFileName = QFileDialog::getSaveFileName(this, "Save Mean Curvature file", "", "Surface file (*.raw);;All files (*.*)");

/*Q3Err:C
	if(outputMeanCurvatureFileName != 0)
*/
	if(!outputMeanCurvatureFileName.isNull())
	{
		m_MeanCurvatureFileLineEdit->setText(outputMeanCurvatureFileName);
	}
}

void ConstructCurvaturesDialog::outputGaussianCurvatureFileSlot()
{
  //QString outputGaussianCurvatureFileName = Q3FileDialog::getSaveFileName("", "Surface file (*.raw);;All files (*.*)");
  QString outputGaussianCurvatureFileName = QFileDialog::getSaveFileName(this, "Save Gaussian Curvature file", "", "Surface file (*.raw);;All files (*.*)");

/*Q3Err:C
	if(outputGaussianCurvatureFileName != 0)
*/
	if(!outputGaussianCurvatureFileName.isNull())
	{
		m_GaussianCurvatureFileLineEdit->setText(outputGaussianCurvatureFileName);
	}
}

void ConstructCurvaturesDialog::isovalueRadioButtonSelectedSlot(bool enabled)
{
	m_IsocontourFileLineEdit->setEnabled(!enabled);
	m_IsocontourFilePushButton->setEnabled(!enabled);
	m_IsovalueLineEdit->setEnabled(enabled);
	m_IsovalueTextLabel->setEnabled(enabled);
}

void ConstructCurvaturesDialog::isocontourRadioButtonSelectedSlot(bool enabled)
{
	m_IsocontourFileLineEdit->setEnabled(enabled);
	m_IsocontourFilePushButton->setEnabled(enabled);
	m_IsovalueLineEdit->setEnabled(!enabled);
	m_IsovalueTextLabel->setEnabled(!enabled);
}
