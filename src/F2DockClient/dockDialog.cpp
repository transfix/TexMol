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
#include <F2DockClient/dockDialog.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <QDoubleValidator>
#include <QIntValidator>

#include <iostream>
#include <string>

#include "ui_dockUI.h"

using namespace std;

DockDialog::DockDialog(QWidget *parent,Qt::WindowFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::dockUI;
  _ui->setupUi(this);

  init();
  makeConnections();
}


DockDialog::~DockDialog()
{ delete _ui;}



void DockDialog::makeConnections()
{
	connect( _ui->receptorPDBButton, SIGNAL( clicked() ), this, SLOT( receptorPDBFileNameSlot() ) );
	connect( _ui->ligandPDBButton, SIGNAL( clicked() ), this, SLOT( ligandPDBFileNameSlot() ) );
	connect( _ui->rmsdButton, SIGNAL( clicked() ), this, SLOT( rmsdFileNameSlot() ) );
	connect( _ui->dockingInputButton, SIGNAL( clicked() ), this, SLOT( inputFileNameSlot() ) );

	connect( _ui->complexTypeCombo, SIGNAL( currentIndexChanged(int) ), this, SLOT( complexTypeSlot(int) ) );
	connect( _ui->rerankCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( rerankSlot() ) );
	connect( _ui->unboundCheckBox, SIGNAL( stateChanged(int) ), this, SLOT( unboundSlot() ) );

	connect( _ui->resetButton, SIGNAL( clicked() ), this, SLOT( resetSlot() ) );
	connect( _ui->submitButton, SIGNAL( clicked() ), this, SLOT( submitSlot() ) );

	connect( _ui->receptorPQRButton, SIGNAL( clicked() ), this, SLOT( receptorPQRFileNameSlot() ) );
	connect( _ui->ligandPQRButton, SIGNAL( clicked() ), this, SLOT( ligandPQRFileNameSlot() ) );
	connect( _ui->receptorF2dButton, SIGNAL( clicked() ), this, SLOT( receptorF2dFileNameSlot() ) );
	connect( _ui->ligandF2dButton, SIGNAL( clicked() ), this, SLOT( ligandF2dFileNameSlot() ) );
	connect( _ui->receptorRAWNButton, SIGNAL( clicked() ), this, SLOT( receptorRAWNFileNameSlot() ) );
	connect( _ui->ligandRAWNButton, SIGNAL( clicked() ), this, SLOT( ligandRAWNFileNameSlot() ) );
	connect( _ui->receptorQuadButton, SIGNAL( clicked() ), this, SLOT( receptorQuadFileNameSlot() ) );
	connect( _ui->ligandQuadButton, SIGNAL( clicked() ), this, SLOT( ligandQuadFileNameSlot() ) );
}


void DockDialog::init()
{
	_ui->dockingTabs->setCurrentIndex(0);

	QStringList labels;
	labels.append("4");
	labels.append("6");
	labels.append("8");
	labels.append("10");
	labels.append("12");
	labels.append("15");
	labels.append("20");
	_ui->rotSeparationCombo->addItems(labels);

	QStringList labels2;
	labels2.append("Antibody-Antigen");
	labels2.append("Enzyme-Inhibitor/Substrate");
	labels2.append("Neither");
	labels2.append("Unknown");
	_ui->complexTypeCombo->addItems(labels2);

	_ui->complexTypeCombo->setCurrentIndex(3);

	QIntValidator *numSolValidator = new QIntValidator(1, 100000, this);
	_ui->numSolTextBox->setValidator(numSolValidator);
	_ui->numRotTextBox->setValidator(numSolValidator);

	QIntValidator *numPeakValidator = new QIntValidator(1, 10, this);
	_ui->peaksPerRotationTextBox->setValidator(numPeakValidator);

	QDoubleValidator *weightValidator = new QDoubleValidator(-100.00, 100.00, 3,this);
	_ui->ssWeightTextBox->setValidator(weightValidator);
	_ui->scWeightTextBox->setValidator(weightValidator);
	_ui->ccWeightTextBox->setValidator(weightValidator);
	_ui->elecWeightTextBox->setValidator(weightValidator);
	_ui->hydrophobicityWeightTextBox->setValidator(weightValidator);
	_ui->hbondWeightTextBox->setValidator(weightValidator);
	_ui->chargeWeightTextBox->setValidator(weightValidator);

	QDoubleValidator *posDouble = new QDoubleValidator(0.01, 100.00, 2,this);
	_ui->gridSpacingTextBox->setValidator(posDouble);


//	_ui->rerankCheckBox->setEnabled(false);
}


void DockDialog::updateDialog()
{
	_ui->rerankCheckBox->setChecked(dockCont->getPerformRerank());
	_ui->unboundCheckBox->setChecked(!dockCont->getBound());

	_ui->clashFilterCheckBox->setChecked(dockCont->getApplyClashFilter());
	_ui->vdwFilterCheckBox->setChecked(dockCont->getApplyVdWFilter());
	_ui->pseudoGsolFilterCheckBox->setChecked(dockCont->getApplyPseudoGsolFilter());
	_ui->dispersionFilterCheckBox->setChecked(dockCont->getApplyDispersionFilter());
	_ui->rerankFilterCheckBox->setChecked(dockCont->getApplyBasicRerank());

	_ui->numSolTextBox->setText(dockCont->getNumSol().c_str());
	_ui->numRotTextBox->setText(dockCont->getNumRot().c_str());
	_ui->gridSpacingTextBox->setText(dockCont->getGridSpacing().c_str());
	_ui->peaksPerRotationTextBox->setText(dockCont->getPeaksPerRotation().c_str());

	_ui->ssWeightTextBox->setText(dockCont->getSSWeight().c_str());
	_ui->scWeightTextBox->setText(dockCont->getSCWeight().c_str());
	_ui->ccWeightTextBox->setText(dockCont->getCCWeight().c_str());
	_ui->elecWeightTextBox->setText(dockCont->getElecWeight().c_str());
	_ui->hydrophobicityWeightTextBox->setText(dockCont->getHydrophobicityWeight().c_str());
	_ui->hbondWeightTextBox->setText(dockCont->getHBondWeight().c_str());
	_ui->chargeWeightTextBox->setText(dockCont->getSimpleChargeWeight().c_str());
}


void DockDialog::inputFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load Input File", QString(), "INP (*.inp)");

	if(!filename.isNull()) 
	{
		_ui->dockingInputTextBox->setText(filename);

		if(!dockCont->setParamsFromFile(_ui->dockingInputTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid input", "Please select a valid input");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}


void DockDialog::rmsdFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load RMSD Atoms File", QString(), "txt (*.txt)");

	if(!filename.isNull()) 
	{
		_ui->rmsdTextBox->setText(filename);

		if(!dockCont->setRMSDName(_ui->rmsdTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid rmsd file", "Please select a valid rmsd file");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}


void DockDialog::receptorPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PDB File", QString(), "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->receptorPDBTextBox->setText(filename);

		if(!dockCont->setReceptorPDBName(_ui->receptorPDBTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::ligandPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PDB File", QString(), "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->ligandPDBTextBox->setText(filename);

		if(!dockCont->setLigandPDBName(_ui->ligandPDBTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::receptorPQRFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PQR File", QString(), "PQR (*.pqr)");

	if(!filename.isNull()) 
	{
		_ui->receptorPQRTextBox->setText(filename);

		if(!dockCont->setReceptorPQRName(_ui->receptorPQRTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PQR", "Please select a valid receptor PQR");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::ligandPQRFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PQR File", QString(), "PQR (*.pqr)");

	if(!filename.isNull()) 
	{
		_ui->ligandPQRTextBox->setText(filename);

		if(!dockCont->setLigandPQRName(_ui->ligandPQRTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PQR", "Please select a valid ligand PQR");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::receptorF2dFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor F2d File", QString(), "F2d (*.f2d)");

	if(!filename.isNull()) 
	{
		_ui->receptorF2dTextBox->setText(filename);

		if(!dockCont->setReceptorF2dName(_ui->receptorF2dTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor F2d", "Please select a valid receptor F2d");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::ligandF2dFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand F2d File", QString(), "F2d (*.f2d)");

	if(!filename.isNull()) 
	{
		_ui->ligandF2dTextBox->setText(filename);

		if(!dockCont->setLigandF2dName(_ui->ligandF2dTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand F2d", "Please select a valid ligand F2d");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::receptorRAWNFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor RAWN File", QString(), "RAWN (*.rawn)");

	if(!filename.isNull()) 
	{
		_ui->receptorRAWNTextBox->setText(filename);

		if(!dockCont->setReceptorRAWNName(_ui->receptorRAWNTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor RAWN", "Please select a valid receptor RAWN");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::ligandRAWNFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand RAWN File", QString(), "RAWN (*.rawn)");

	if(!filename.isNull()) 
	{
		_ui->ligandRAWNTextBox->setText(filename);

		if(!dockCont->setLigandRAWNName(_ui->ligandRAWNTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand RAWN", "Please select a valid ligand RAWN");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::receptorQuadFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor Quad File", QString(), "Quad (*.quad)");

	if(!filename.isNull()) 
	{
		_ui->receptorQuadTextBox->setText(filename);

		if(!dockCont->setReceptorQuadName(_ui->receptorQuadTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor Quad", "Please select a valid receptor Quad");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void DockDialog::ligandQuadFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand Quad File", QString(), "Quad (*.quad)");

	if(!filename.isNull()) 
	{
		_ui->ligandQuadTextBox->setText(filename);

		if(!dockCont->setLigandQuadName(_ui->ligandQuadTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand Quad", "Please select a valid ligand Quad");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}


void DockDialog::complexTypeSlot(int index)
{
//	int index = _ui->complexTypeCombo->currentIndex();
	
	if(!dockCont->setComplexType(index))
	{
		QMessageBox::information(this, "Please select a valid Complex Type", "Please select a valid Complex Type");
	}
	else
	{
		updateDialog();
	}
}

void DockDialog::unboundSlot()
{
	if(!dockCont->setBound(!_ui->unboundCheckBox->isChecked())) 
		QMessageBox::information(this, "Invalid selection", "Invalid selection");
}

void DockDialog::rerankSlot()
{
	if(!dockCont->setPerformRerank(_ui->rerankCheckBox->isChecked()))
		QMessageBox::information(this, "Invalid selection", "Invalid selection");
}

void DockDialog::resetSlot()
{
	if(!dockCont->reset(_ui->complexTypeCombo->currentIndex()))
		QMessageBox::information(this, "Failed to reset", "Failed to reset");
	else
		updateDialog();
}


void DockDialog::submit()
{
	if(dockCont->handleJob())
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


bool DockDialog::prepareInputFromFile(string dockInput)
{
	if(!dockCont->setParamsFromFile(dockInput))
	{
		QMessageBox::information(this, "Please select a valid docking input file", "Please select a valid docking input file");
		return false;
	}	
	else
	{
		return true;
	}
}


bool DockDialog::prepareInput()
{
	QString rmsdFile = _ui->rmsdTextBox->text();

	QString ligandPDB = _ui->ligandPDBTextBox->text();
	QString receptorPDB = _ui->receptorPDBTextBox->text();

	QString ligandPQR = _ui->ligandPQRTextBox->text();
	QString receptorPQR = _ui->receptorPQRTextBox->text();

	QString ligandF2d = _ui->ligandF2dTextBox->text();
	QString receptorF2d = _ui->receptorF2dTextBox->text();

	QString ligandQuad = _ui->ligandQuadTextBox->text();
	QString receptorQuad = _ui->receptorQuadTextBox->text();

	QString ligandRAWN = _ui->ligandRAWNTextBox->text();
	QString receptorRAWN = _ui->receptorRAWNTextBox->text();

	QString ns = _ui->numSolTextBox->text();
	QString nr = _ui->numRotTextBox->text();
	QString p = _ui->peaksPerRotationTextBox->text();
	QString g = _ui->gridSpacingTextBox->text();

	QString ss = _ui->ssWeightTextBox->text();
	QString sc = _ui->scWeightTextBox->text();
	QString cc = _ui->ccWeightTextBox->text();
	QString elec = _ui->elecWeightTextBox->text();
	QString hbond = _ui->hbondWeightTextBox->text();
	QString hydro = _ui->hydrophobicityWeightTextBox->text();
	QString scc = _ui->chargeWeightTextBox->text();

	if(rmsdFile.length()>0)
	{
		if(!dockCont->setRMSDName(rmsdFile.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid rmsd atoms file", "Please select a valid lrmsd atoms file");
			return false;
		}			
	}


	if(ligandPDB.length()>0)
	{
		if(!dockCont->setLigandPDBName(ligandPDB.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
			return false;
		}			
	}

	if(receptorPDB.length()>0)
	{
		if(!dockCont->setReceptorPDBName(receptorPDB.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
			return false;
		}			
	}

	if(ligandF2d.length()>0)
	{
		if(!dockCont->setLigandF2dName(ligandF2d.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand F2d", "Please select a valid ligand F2d");
			return false;
		}			
	}

	if(receptorF2d.length()>0)
	{
		if(!dockCont->setReceptorF2dName(receptorF2d.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor F2d", "Please select a valid receptor F2d");
			return false;
		}			
	}

	if(ligandPQR.length()>0)
	{
		if(!dockCont->setLigandPQRName(ligandPQR.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand PQR", "Please select a valid ligand PQR");
			return false;
		}			
	}

	if(receptorPQR.length()>0)
	{
		if(!dockCont->setReceptorPQRName(receptorPQR.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor PQR", "Please select a valid receptor PQR");
			return false;
		}			
	}

	if(ligandQuad.length()>0)
	{
		if(!dockCont->setLigandQuadName(ligandQuad.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand Quad", "Please select a valid ligand Quad");
			return false;
		}			
	}

	if(receptorQuad.length()>0)
	{
		if(!dockCont->setReceptorQuadName(receptorQuad.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor Quad", "Please select a valid receptor Quad");
			return false;
		}			
	}

	if(ligandRAWN.length()>0)
	{
		if(!dockCont->setLigandRAWNName(ligandRAWN.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid ligand RAWN", "Please select a valid ligand RAWN");
			return false;
		}			
	}

	if(receptorRAWN.length()>0)
	{
		if(!dockCont->setReceptorRAWNName(receptorRAWN.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid receptor RAWN", "Please select a valid receptor RAWN");
			return false;
		}			
	}

	if(ns.length()>0)
	{
		if(!dockCont->setNumSol(ns.toInt()))
		{
			QMessageBox::information(this, "Please select a valid number of solutions", "Please select a valid number of solutions");
			return false;
		}			
	}

	if(nr.length()>0)
	{
		if(!dockCont->setNumRot(nr.toInt()))
		{
			QMessageBox::information(this, "Please select a valid number of rotations", "Please select a valid number of rotations");
			return false;
		}			
	}

	if(g.length()>0)
	{
		if(!dockCont->setGridSpacing(g.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid grid spacing", "Please select a valid grid spacing");
			return false;
		}			
	}

	if(p.length()>0)
	{
		if(!dockCont->setPeaksPerRotation(p.toInt()))
		{
			QMessageBox::information(this, "Please select a valid number of peaks to consider per rotation", "Please select a valid number of peaks to consider per rotation");
			return false;
		}			
	}

	if(ss.length()>0)
	{
		if(!dockCont->setSSWeight(ss.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid skin-skin weight", "Please select a valid skin-skin weight");
			return false;
		}			

	}

	if(sc.length()>0)
	{
		if(!dockCont->setSCWeight(sc.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid skin-core weight", "Please select a valid skin-core weight");
			return false;
		}			

	}

	if(cc.length()>0)
	{
		if(!dockCont->setCCWeight(cc.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid core-core weight", "Please select a valid core-core weight");
			return false;
		}			

	}

	if(elec.length()>0)
	{
		if(!dockCont->setElecWeight(elec.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid electrostatics weight", "Please select a valid electrostatics weight");
			return false;
		}			

	}

	if(hbond.length()>0)
	{
		if(!dockCont->setHBondWeight(hbond.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid hbond weight", "Please select a valid hbond weight");
			return false;
		}			

	}

	if(hydro.length()>0)
	{
		if(!dockCont->setHydrophobicityWeight(hydro.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid hydrophobicity weight", "Please select a valid hydrophobicity weight");
			return false;
		}			

	}
	
	if(scc.length()>0)
	{
		if(!dockCont->setSimpleChargeWeight(scc.toDouble()))
		{
			QMessageBox::information(this, "Please select a valid charge complementarity weight", "Please select a valid charge complementarity weight");
			return false;
		}			

	}

	if(_ui->receptorPQRCheckBox->isChecked()) dockCont->setStoreReceptorPQR();
	if(_ui->ligandPQRCheckBox->isChecked()) dockCont->setStoreLigandPQR();

	if(_ui->receptorF2dCheckBox->isChecked()) dockCont->setStoreReceptorF2d();
	if(_ui->ligandF2dCheckBox->isChecked()) dockCont->setStoreLigandF2d();
	
	if(_ui->receptorRAWNCheckBox->isChecked()) dockCont->setStoreReceptorRAWN();
	if(_ui->ligandRAWNCheckBox->isChecked()) dockCont->setStoreLigandRAWN();

	if(_ui->receptorQuadCheckBox->isChecked()) dockCont->setStoreReceptorQuad();
	if(_ui->ligandQuadCheckBox->isChecked()) dockCont->setStoreLigandQuad();

	dockCont->setApplyClashFilter(_ui->clashFilterCheckBox->isChecked());
	dockCont->setApplyVdWFilter(_ui->vdwFilterCheckBox->isChecked());
	dockCont->setApplyPseudoGsolFilter(_ui->pseudoGsolFilterCheckBox->isChecked());
	dockCont->setApplyDispersionFilter(_ui->dispersionFilterCheckBox->isChecked());
	dockCont->setApplyBasicRerank(_ui->rerankFilterCheckBox->isChecked());

	return true;
}


void DockDialog::submitSlot()
{
	QString title = _ui->titleTextBox->text();
	QString dockInput = _ui->dockingInputTextBox->text();

	bool processFromInput = false;

	if(title.length()>0)
	{
		if(!dockCont->setJobTitle(title.toStdString()))
		{
			QMessageBox::information(this, "Please select a valid title", "Please select a valid title");
			return;
		}			
	}

	if(dockInput.length()>0)
	{
		if(prepareInputFromFile(dockInput.toStdString()))
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
