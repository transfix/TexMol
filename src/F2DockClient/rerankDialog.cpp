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
#include <F2DockClient/rerankDialog.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <QDoubleValidator>
#include <QIntValidator>

#include <iostream>
#include <string>

#include "ui_rerankUI.h"

using namespace std;

RerankDialog::RerankDialog(QWidget *parent,Qt::WindowFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::rerankUI;
  _ui->setupUi(this);

  init();
  makeConnections();
}


RerankDialog::~RerankDialog()
{ delete _ui;}



void RerankDialog::makeConnections()
{
	connect( _ui->receptorPDBButton, SIGNAL( clicked() ), this, SLOT( receptorPDBFileNameSlot() ) );
	connect( _ui->ligandPDBButton, SIGNAL( clicked() ), this, SLOT( ligandPDBFileNameSlot() ) );
	connect( _ui->dockingOutputButton, SIGNAL( clicked() ), this, SLOT( dockingOutputFileNameSlot() ) );
	connect( _ui->rerankingInputButton, SIGNAL( clicked() ), this, SLOT( inputFileNameSlot() ) );

	connect( _ui->resetButton, SIGNAL( clicked() ), this, SLOT( resetSlot() ) );

	connect( _ui->submitButton, SIGNAL( clicked() ), this, SLOT( submitSlot() ) );

	connect( _ui->receptorPQRButton, SIGNAL( clicked() ), this, SLOT( receptorPQRFileNameSlot() ) );
	connect( _ui->ligandPQRButton, SIGNAL( clicked() ), this, SLOT( ligandPQRFileNameSlot() ) );
	connect( _ui->receptorRAWNButton, SIGNAL( clicked() ), this, SLOT( receptorRAWNFileNameSlot() ) );
	connect( _ui->ligandRAWNButton, SIGNAL( clicked() ), this, SLOT( ligandRAWNFileNameSlot() ) );
	connect( _ui->receptorQuadButton, SIGNAL( clicked() ), this, SLOT( receptorQuadFileNameSlot() ) );
	connect( _ui->ligandQuadButton, SIGNAL( clicked() ), this, SLOT( ligandQuadFileNameSlot() ) );

}


void RerankDialog::init()
{
	_ui->rerankingTabs->setCurrentIndex(0);

	QIntValidator *numSolValidator = new QIntValidator(1, 54000, this);
	_ui->numSolTextBox->setValidator(numSolValidator);

	QDoubleValidator *weightValidator = new QDoubleValidator(-100.00, 100.00, 3,this);
	_ui->polarWeightTextBox->setValidator(weightValidator);
	_ui->nonPolarWeightTextBox->setValidator(weightValidator);
	_ui->dockWeightTextBox->setValidator(weightValidator);

	QDoubleValidator *errorValidator = new QDoubleValidator(0.00, 1.00, 3,this);
	_ui->bornErrorTextBox->setValidator(errorValidator);
	_ui->gpolErrorTextBox->setValidator(errorValidator);
}


void RerankDialog::updateDialog()
{
	_ui->numSolTextBox->setText(rerankCont->getNumsol().c_str());
	_ui->polarWeightTextBox->setText(rerankCont->getGpolWeight().c_str());
	_ui->nonPolarWeightTextBox->setText(rerankCont->getNonpolWeight().c_str());
	_ui->dockWeightTextBox->setText(rerankCont->getF2dockWeight().c_str());
	_ui->bornErrorTextBox->setText(rerankCont->getEpsilonBR().c_str());
	_ui->gpolErrorTextBox->setText(rerankCont->getEpsilonGpol().c_str());
}


void RerankDialog::inputFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load Input File", QString(), "INP (*.inp)");

	if(!filename.isNull()) 
	{
		_ui->rerankingInputTextBox->setText(filename);

		if(!rerankCont->setParamsFromFile(_ui->rerankingInputTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid input", "Please select a valid input");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}


void RerankDialog::receptorPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PDB File", QString(), "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->receptorPDBTextBox->setText(filename);

		if(!rerankCont->setReceptorPDBName(_ui->receptorPDBTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::ligandPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PDB File", QString(), "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->ligandPDBTextBox->setText(filename);

		if(!rerankCont->setLigandPDBName(_ui->ligandPDBTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::receptorPQRFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PQR File", QString(), "PQR (*.pqr)");

	if(!filename.isNull()) 
	{
		_ui->receptorPQRTextBox->setText(filename);

		if(!rerankCont->setReceptorPQRName(_ui->receptorPQRTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PQR", "Please select a valid receptor PQR");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::ligandPQRFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PQR File", QString(), "PQR (*.pqr)");

	if(!filename.isNull()) 
	{
		_ui->ligandPQRTextBox->setText(filename);

		if(!rerankCont->setLigandPQRName(_ui->ligandPQRTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PQR", "Please select a valid ligand PQR");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::receptorRAWNFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor RAWN File", QString(), "RAWN (*.rawn)");

	if(!filename.isNull()) 
	{
		_ui->receptorRAWNTextBox->setText(filename);

		if(!rerankCont->setReceptorRAWNName(_ui->receptorRAWNTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor RAWN", "Please select a valid receptor RAWN");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::ligandRAWNFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand RAWN File", QString(), "RAWN (*.rawn)");

	if(!filename.isNull()) 
	{
		_ui->ligandRAWNTextBox->setText(filename);

		if(!rerankCont->setLigandRAWNName(_ui->ligandRAWNTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand RAWN", "Please select a valid ligand RAWN");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::receptorQuadFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor Quad File", QString(), "Quad (*.quad)");

	if(!filename.isNull()) 
	{
		_ui->receptorQuadTextBox->setText(filename);

		if(!rerankCont->setReceptorQuadName(_ui->receptorQuadTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor Quad", "Please select a valid receptor Quad");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::ligandQuadFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand Quad File", QString(), "Quad (*.quad)");

	if(!filename.isNull()) 
	{
		_ui->ligandQuadTextBox->setText(filename);

		if(!rerankCont->setLigandQuadName(_ui->ligandQuadTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand Quad", "Please select a valid ligand Quad");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::dockingOutputFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load Docking Output File", QString(), "Out (*.out)");

	if(!filename.isNull())
	{
   		_ui->dockingOutputTextBox->setText(filename);

		if(!rerankCont->setDockingOutputName(_ui->dockingOutputTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid docking output", "Please select a valid docking output");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void RerankDialog::resetSlot()
{
	if(!rerankCont->reset())
		QMessageBox::information(this, "Failed to reset", "Failed to reset");
	else
		updateDialog();
}


void RerankDialog::submit()
{
	if(rerankCont->handleJob())
	{
		cout<<"done"<<endl;
		this->done(QDialog::Accepted);
	}
	else
	{
		QMessageBox::information(this, "Submission error. Missing required inputs.", "Submission error. Missing required inputs.");
		return;
	}
}


bool RerankDialog::prepareInputFromFile(string rerankInput)
{
	if(!rerankCont->setParamsFromFile(rerankInput))
	{
		QMessageBox::information(this, "Please select a valid docking input file", "Please select a valid docking input file");
		return false;
	}	
	else
	{
		return true;
	}
}


bool RerankDialog::prepareInput()
{
	QString ligandPDB = _ui->ligandPDBTextBox->text();
	QString receptorPDB = _ui->receptorPDBTextBox->text();

	QString ligandPQR = _ui->ligandPQRTextBox->text();
	QString receptorPQR = _ui->receptorPQRTextBox->text();

	QString ligandQuad = _ui->ligandQuadTextBox->text();
	QString receptorQuad = _ui->receptorQuadTextBox->text();

	QString ligandRAWN = _ui->ligandRAWNTextBox->text();
	QString receptorRAWN = _ui->receptorRAWNTextBox->text();

	QString dockingOutput = _ui->dockingOutputTextBox->text();

	QString n = _ui->numSolTextBox->text();
	QString w1 = _ui->polarWeightTextBox->text();
	QString w2 = _ui->nonPolarWeightTextBox->text();
	QString w3 = _ui->dockWeightTextBox->text();
	QString e1 = _ui->bornErrorTextBox->text();
	QString e2 = _ui->gpolErrorTextBox->text();

	if(dockingOutput.length()>0)
	{
		if(!rerankCont->setDockingOutputName(dockingOutput.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid docking Output", "Please select a valid docking Output");
			return false;
		}			
	}


	if(ligandPDB.length()>0)
	{
		if(!rerankCont->setLigandPDBName(ligandPDB.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
			return false;
		}			
	}

	if(receptorPDB.length()>0)
	{
		if(!rerankCont->setReceptorPDBName(receptorPDB.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
			return false;
		}			
	}

	if(ligandPQR.length()>0)
	{
		if(!rerankCont->setLigandPQRName(ligandPQR.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand PQR", "Please select a valid ligand PQR");
			return false;
		}			
	}

	if(receptorPQR.length()>0)
	{
		if(!rerankCont->setReceptorPQRName(receptorPQR.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor PQR", "Please select a valid receptor PQR");
			return false;
		}			
	}

	if(ligandQuad.length()>0)
	{
		if(!rerankCont->setLigandQuadName(ligandQuad.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand Quad", "Please select a valid ligand Quad");
			return false;
		}			
	}

	if(receptorQuad.length()>0)
	{
		if(!rerankCont->setReceptorQuadName(receptorQuad.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor Quad", "Please select a valid receptor Quad");
			return false;
		}			
	}

	if(ligandRAWN.length()>0)
	{
		if(!rerankCont->setLigandRAWNName(ligandRAWN.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand RAWN", "Please select a valid ligand RAWN");
			return false;
		}			
	}

	if(receptorRAWN.length()>0)
	{
		if(!rerankCont->setReceptorRAWNName(receptorRAWN.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor RAWN", "Please select a valid receptor RAWN");
			return false;
		}			
	}
	
	if(n.length()>0)
	{
		if(!rerankCont->setNumsol(n.toInt()))
		{
			QMessageBox::information(this, "Please select a valid number of solutions", "Please select a valid number of solutions");
			return false;
		}			

	}

	if(w1.length()>0)
	{
		if(!rerankCont->setGpolWeight(w1.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid gpol weight", "Please select a valid gpol weight");
			return false;
		}			

	}

	if(w2.length()>0)
	{
		if(!rerankCont->setNonpolWeight(w2.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid nongpol weight", "Please select a valid nongpol weight");
			return false;
		}			

	}

	if(w3.length()>0)
	{
		if(!rerankCont->setF2dockWeight(w3.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid docking weight", "Please select a valid docking weight");
			return false;
		}			

	}
	
	if(e1.length()>0)
	{
		if(!rerankCont->setEpsilonBR(e1.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid born radii error", "Please select a valid born radii error");
			return false;
		}			

	}

	if(e2.length()>0)
	{
		if(!rerankCont->setEpsilonGpol(e2.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid gpol error", "Please select a valid gpol error");
			return false;
		}			

	}

	if(_ui->receptorPQRCheckBox->isChecked()) rerankCont->setStoreReceptorPQR();
	if(_ui->ligandPQRCheckBox->isChecked()) rerankCont->setStoreLigandPQR();
	
	if(_ui->receptorRAWNCheckBox->isChecked()) rerankCont->setStoreReceptorRAWN();
	if(_ui->ligandRAWNCheckBox->isChecked()) rerankCont->setStoreLigandRAWN();

	if(_ui->receptorQuadCheckBox->isChecked()) rerankCont->setStoreReceptorQuad();
	if(_ui->ligandQuadCheckBox->isChecked()) rerankCont->setStoreLigandQuad();

	return true;
}


void RerankDialog::submitSlot()
{
	QString title = _ui->titleTextBox->text();
	QString rerankInput = _ui->rerankingInputTextBox->text();

	bool processFromInput = false;


	if(title.length()>0)
	{
		if(!rerankCont->setJobTitle(title.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid title", "Please select a valid title");
			return;
		}			
	}

	if(rerankInput.length()>0)
	{
		if(prepareInputFromFile(rerankInput.toStdString()))
		{
			processFromInput = true;
			submit();
		}
	}

	if(!processFromInput)
	{
		if(prepareInput())
			submit();
	}


	
}
