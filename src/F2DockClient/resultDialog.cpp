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
#include <F2DockClient/resultDialog.h>

#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
#include <string>

#include "ui_resultUI.h"

using namespace std;

ResultDialog::ResultDialog(QWidget *parent,Qt::WindowFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::resultUI;
  _ui->setupUi(this);

  init();
  makeConnections();
}


ResultDialog::~ResultDialog()
{ delete _ui;}



void ResultDialog::makeConnections()
{
	connect( _ui->clearButton, SIGNAL( clicked() ), this, SLOT( clearSlot() ) );
//	connect( _ui->doneButton, SIGNAL( clicked() ), this, SLOT( doneSlot() ) );
	connect( _ui->saveButton, SIGNAL( clicked() ), this, SLOT( saveSlot() ) );

	connect( _ui->saveConformationButton, SIGNAL( clicked() ), this, SLOT( saveConformationSlot() ) );

	connect( _ui->resultTable, SIGNAL( itemSelectionChanged() ), this, SLOT( resultSelectedSlot() ) );

	connect( _ui->renderUofBRadioButton, SIGNAL( toggled(bool) ), this, SLOT( uofbRBSlot(bool)) );
	connect( _ui->renderSmoothSurfRadioButton, SIGNAL( toggled(bool) ), this, SLOT( surfRBSlot(bool) ) );
	connect( _ui->renderSmoothPotentialRadioButton, SIGNAL( toggled(bool) ), this, SLOT( potRBSlot(bool) ) );
	connect( _ui->renderSmoothPotentialCoulombicRadioButton, SIGNAL( toggled(bool) ), this, SLOT( coulRBSlot(bool) ) );
	connect( _ui->renderSmoothPotentialGBRadioButton, SIGNAL( toggled(bool) ), this, SLOT( gbRBSlot(bool) ) );
	connect( _ui->renderSmoothPotentialPBRadioButton, SIGNAL( toggled(bool) ), this, SLOT( pbRBSlot(bool) ) );
}


void ResultDialog::init()
{
	_ui->resultMainTabs->setCurrentIndex(0);
	_ui->detailResultTabs->setCurrentIndex(0);

	rerankedMode = false;
	rmsdMode = false;

	setResultTableHeader();
//	setResContHeader();

	setDefaultButtonStates();
}


void ResultDialog::setDefaultButtonStates()
{
	_ui->renderUofBRadioButton->setDown(true);
	_ui->renderSmoothSurfRadioButton->setDown(false);
	_ui->renderSmoothPotentialRadioButton->setDown(false);

	_ui->renderUofBRadioButton->setEnabled(true);
	_ui->renderSmoothSurfRadioButton->setEnabled(true);
	_ui->renderSmoothPotentialRadioButton->setEnabled(false);

	_ui->renderSmoothPotentialCoulombicRadioButton->setDown(true);
	_ui->renderSmoothPotentialGBRadioButton->setDown(false);
	_ui->renderSmoothPotentialPBRadioButton->setDown(false);

	_ui->renderSmoothPotentialCoulombicRadioButton->setEnabled(false);
	_ui->renderSmoothPotentialGBRadioButton->setEnabled(false);
	_ui->renderSmoothPotentialPBRadioButton->setEnabled(false);

	_ui->indivTransparentCheckBox->setDown(false);
	_ui->indivMultipleLigandCheckBox->setDown(false);

	statsAvailable = false;
}


void ResultDialog::setResultTableHeader()
{
	QStringList labels;

	if(rerankedMode)
	{
		labels.append("New Rank");
		labels.append("Old Rank");
		labels.append("New Score");
		labels.append("Old Score");
		if(rmsdMode)labels.append("RMSD");

		_ui->resultTable->setHorizontalHeaderLabels(labels);

		_ui->resultTable->setColumnWidth(0, 50);
		_ui->resultTable->setColumnWidth(1, 50);
		_ui->resultTable->setColumnWidth(2, 100);
		_ui->resultTable->setColumnWidth(3, 100);
		if(rmsdMode)_ui->resultTable->setColumnWidth(4, 100);

	}
	else
	{
		labels.append("Rank");
		labels.append("Score");
		if(rmsdMode)labels.append("RMSD");

		_ui->resultTable->setHorizontalHeaderLabels(labels);

		_ui->resultTable->setColumnWidth(0, 50);
		_ui->resultTable->setColumnWidth(1, 100);
		if(rmsdMode)_ui->resultTable->setColumnWidth(2, 100);

	}

	QStringList labels2;
	labels2.append("ILE");
	labels2.append("VAL");
	labels2.append("LEU");
	labels2.append("PHE");

	labels2.append("CYS");
	labels2.append("MET");
	labels2.append("ALA");
	labels2.append("GLY");

	labels2.append("THR");
	labels2.append("SER");
	labels2.append("TRP");
	labels2.append("TYR");

	labels2.append("PRO");
	labels2.append("HIS");
	labels2.append("GLU");
	labels2.append("GLN");

	labels2.append("ASP");
	labels2.append("ASN");
	labels2.append("LYS");
	labels2.append("ARG");

	_ui->resresContactTable->setHorizontalHeaderLabels(labels2);
	_ui->resresContactTable->setVerticalHeaderLabels(labels2);
}


bool ResultDialog::setController(ResultControl *rc)
{
	resCont = rc;

	cout<<"inside set controller"<<endl;

	if(!resCont->readResults())
	{
		return false;
	}
	else
	{
//		statsAvailable = resCont->computeStats(1);
		updateDialog();
		return true;
	}
}


void ResultDialog::updateDialog()
{
	cout<<"inside update dialog"<<endl;

	rerankedMode = resCont->getMode();
	rmsdMode = resCont->getRMSDMode();

	setResultTableHeader();

	cout<<"headers set"<<endl;

	updateResultTable();

	cout<<"updated res table"<<endl;
	
	updateSummaryResults();

	cout<<"updated sumres"<<endl;
	
	updateJobDetails();

	cout<<"updated jobdet"<<endl;
}


void ResultDialog::clearSlot()
{
	
}


void ResultDialog::saveSlot()
{
	bool success = true;

	if(!resCont->setUofBVis(_ui->renderUofBRadioButton->isChecked())) success = false;
	if(success){if(!resCont->setSurfVis(_ui->renderSmoothSurfRadioButton->isChecked())) success = false;}
	if(success){if(!resCont->setPotVis(_ui->renderSmoothPotentialRadioButton->isChecked())) success = false;}		

	if(success){if(!resCont->setCoulVis(_ui->renderSmoothPotentialCoulombicRadioButton->isChecked())) success = false;}
	if(success){if(!resCont->setPBVis(_ui->renderSmoothPotentialGBRadioButton->isChecked())) success = false;}
	if(success){if(!resCont->setGBVis(_ui->renderSmoothPotentialPBRadioButton->isChecked())) success = false;}

	if(success){if(!resCont->setMultipleVis(_ui->indivMultipleLigandCheckBox->isChecked())) success = false;}
	if(success){if(!resCont->setTransparentInterface(_ui->indivTransparentCheckBox->isChecked())) success = false;}

	if(!success) 
		QMessageBox::information(this, "Failed to save settings", "Failed to save settings");
}


void ResultDialog::saveConformationSlot()
{
	if(resCont->saveConformation(_ui->resultTable->currentRow()))
		QMessageBox::information(this, "Saved conformation", "Saved conformation");
	else
		QMessageBox::information(this, "Failed to save conformation. Error in ligand pdb or no pdb found.", "Failed to save conformation. Error in ligand pdb or no pdb found.");
}


void ResultDialog::uofbRBSlot(bool b)
{
	if(b)
	{
		_ui->renderSmoothSurfRadioButton->setDown(false);
		_ui->renderSmoothPotentialRadioButton->setDown(false);

	
		_ui->renderSmoothPotentialCoulombicRadioButton->setEnabled(false);
		_ui->renderSmoothPotentialGBRadioButton->setEnabled(false);
		_ui->renderSmoothPotentialPBRadioButton->setEnabled(false);	
	}
	emit vizSettingsChanged();
}

void ResultDialog::surfRBSlot(bool b)
{
	if(b)
	{
		_ui->renderUofBRadioButton->setDown(false);
		_ui->renderSmoothPotentialRadioButton->setDown(false);

	
		_ui->renderSmoothPotentialCoulombicRadioButton->setEnabled(false);
		_ui->renderSmoothPotentialGBRadioButton->setEnabled(false);
		_ui->renderSmoothPotentialPBRadioButton->setEnabled(false);	
	}
	emit vizSettingsChanged();
}

void ResultDialog::potRBSlot(bool b)
{
	if(b)
	{
		_ui->renderUofBRadioButton->setDown(false);
		_ui->renderSmoothSurfRadioButton->setDown(false);
	
		_ui->renderSmoothPotentialCoulombicRadioButton->setEnabled(true);
		_ui->renderSmoothPotentialGBRadioButton->setEnabled(true);
		_ui->renderSmoothPotentialPBRadioButton->setEnabled(true);

		_ui->renderSmoothPotentialCoulombicRadioButton->setDown(true);
		_ui->renderSmoothPotentialGBRadioButton->setDown(false);
		_ui->renderSmoothPotentialPBRadioButton->setDown(false);
	}
	emit vizSettingsChanged();
}

void ResultDialog::coulRBSlot(bool b)
{
	if(b)
	{
		_ui->renderSmoothPotentialGBRadioButton->setDown(false);
		_ui->renderSmoothPotentialPBRadioButton->setDown(false);
	}
	emit vizSettingsChanged();
}

void ResultDialog::gbRBSlot(bool b)
{
	if(b)
	{
		_ui->renderSmoothPotentialCoulombicRadioButton->setDown(false);
		_ui->renderSmoothPotentialPBRadioButton->setDown(false);
	}
	emit vizSettingsChanged();
}

void ResultDialog::pbRBSlot(bool b)
{
	if(b)
	{
		_ui->renderSmoothPotentialCoulombicRadioButton->setDown(false);
		_ui->renderSmoothPotentialGBRadioButton->setDown(false);
	}
	emit vizSettingsChanged();
}



void ResultDialog::resultSelectedSlot()
{
	updateIndivResults(_ui->resultTable->currentRow());
	emit resultSelected(_ui->resultTable->currentRow());
}


double * ResultDialog::getMat(int index)
{
	return resCont->getMat(index);
}


void ResultDialog::updateResultTable()
{
	numRow = resCont->getSize();

	_ui->resultTable->setRowCount(numRow+1);

	for(int i=0; i<numRow; i++)
		updateResultRow(i);
}


void ResultDialog::updateResultRow(int i)
{
	string temp;

	if(rerankedMode)
	{
		_ui->resultTable->setItem(i, 0, new QTableWidgetItem( resCont->getNewRank(i).c_str() ));
		_ui->resultTable->setItem(i, 1, new QTableWidgetItem( resCont->getRank(i).c_str() ));
		_ui->resultTable->setItem(i, 2, new QTableWidgetItem( resCont->getNewScore(i).c_str() ));
		_ui->resultTable->setItem(i, 3, new QTableWidgetItem( resCont->getScore(i).c_str() ));
		if(rmsdMode)_ui->resultTable->setItem(i, 4, new QTableWidgetItem( resCont->getRmsd(i).c_str() ));
	}
	else
	{
		_ui->resultTable->setItem(i, 0, new QTableWidgetItem( resCont->getRank(i).c_str() ));
		_ui->resultTable->setItem(i, 1, new QTableWidgetItem( resCont->getScore(i).c_str() ));
		if(rmsdMode)_ui->resultTable->setItem(i, 2, new QTableWidgetItem( resCont->getRmsd(i).c_str() ));
	}
}

	
void ResultDialog::updateSummaryResults()
{
	_ui->totalPeakTextBox->setText(resCont->getTotalPeaks(false).c_str());
	_ui->peak1TextBox->setText(resCont->getHitsInRange(false,0).c_str());
	_ui->peak10TextBox->setText(resCont->getHitsInRange(false,1).c_str());
	_ui->peak100TextBox->setText(resCont->getHitsInRange(false,2).c_str());
	_ui->peak1000TextBox->setText(resCont->getHitsInRange(false,3).c_str());
	_ui->peak10000TextBox->setText(resCont->getHitsInRange(false,4).c_str());

	_ui->topRankedRmsdTextBox->setText(resCont->getMinRankedPeakRMSD(false).c_str());
	_ui->topRankedScoreTextBox->setText(resCont->getMinRankedPeakScore(false).c_str());

	_ui->topSolutionRankTextBox->setText(resCont->getIndexMinRMSD(false).c_str());
	_ui->topSolutionRmsdTextBox->setText(resCont->getMinRMSD(false).c_str());
	_ui->topSolutionScoreTextBox->setText(resCont->getScoreMinRMSD(false).c_str());

	_ui->timeTextBox->setText(resCont->getTime(false).c_str());

	if(rerankedMode)
	{
		_ui->totalPeakTextBox_2->setText(resCont->getTotalPeaks(true).c_str());
		_ui->peak1TextBox_2->setText(resCont->getHitsInRange(true,0).c_str());
		_ui->peak10TextBox_2->setText(resCont->getHitsInRange(true,1).c_str());
		_ui->peak100TextBox_2->setText(resCont->getHitsInRange(true,2).c_str());
		_ui->peak1000TextBox_2->setText(resCont->getHitsInRange(true,3).c_str());
		_ui->peak10000TextBox_2->setText(resCont->getHitsInRange(true,4).c_str());

		_ui->topRankedRmsdTextBox_2->setText(resCont->getMinRankedPeakRMSD(true).c_str());
		_ui->topRankedScoreTextBox_2->setText(resCont->getMinRankedPeakScore(true).c_str());

		_ui->topSolutionRankTextBox_2->setText(resCont->getIndexMinRMSD(true).c_str());
		_ui->topSolutionRmsdTextBox_2->setText(resCont->getMinRMSD(true).c_str());
		_ui->topSolutionScoreTextBox_2->setText(resCont->getScoreMinRMSD(true).c_str());

		_ui->timeTextBox_2->setText(resCont->getTime(true).c_str());
	}
}

	
void ResultDialog::updateJobDetails()
{
	_ui->receptorPDBTextBox->setText(resCont->getReceptorPDB().c_str());
	_ui->ligandPDBTextBox->setText(resCont->getLigandPDB().c_str());

	_ui->clashFilterCheckBox->setChecked(resCont->getApplyClashFilter());
	_ui->vdwFilterCheckBox->setChecked(resCont->getApplyVdWFilter());
	_ui->pseudoGsolFilterCheckBox->setChecked(resCont->getApplyPseudoGsolFilter());
	_ui->dispersionFilterCheckBox->setChecked(resCont->getApplyDispersionFilter());
	_ui->rerankFilterCheckBox->setChecked(resCont->getApplyBasicRerank());

	_ui->gridSpacingTextBox->setText(resCont->getGridSpacing().c_str());
	_ui->peaksPerRotationTextBox->setText(resCont->getPeaksPerRotation().c_str());

	_ui->ssWeightTextBox->setText(resCont->getSSWeight().c_str());
	_ui->scWeightTextBox->setText(resCont->getSCWeight().c_str());
	_ui->ccWeightTextBox->setText(resCont->getCCWeight().c_str());
	_ui->elecWeightTextBox->setText(resCont->getElecWeight().c_str());
	_ui->hydrophobicityWeightTextBox->setText(resCont->getHydrophobicityWeight().c_str());
	_ui->hbondWeightTextBox->setText(resCont->getHBondWeight().c_str());
	_ui->chargeWeightTextBox->setText(resCont->getSimpleChargeWeight().c_str());

	_ui->rotSamplingTextBox->setText(resCont->getRotSeparation().c_str());
	_ui->complexTypeTextBox->setText(resCont->getComplexType().c_str());

	if(rerankedMode)
	{
		_ui->polarWeightTextBox->setText(resCont->getGpolWeight().c_str());
		_ui->nonPolarWeightTextBox->setText(resCont->getNonpolWeight().c_str());
		_ui->dockWeightTextBox->setText(resCont->getF2dockWeight().c_str());
		_ui->bornErrorTextBox->setText(resCont->getEpsilonBR().c_str());
		_ui->gpolErrorTextBox->setText(resCont->getEpsilonGpol().c_str());
	}
}


void ResultDialog::updateIndivResults(int i)
{
	_ui->indivRankTextBox->setText(resCont->getRank(i).c_str());
	_ui->indivRankTextBox_2->setText(resCont->getNewRank(i).c_str());

	_ui->indivScoreTextBox->setText(resCont->getScore(i).c_str());
	_ui->indivScoreTextBox_2->setText(resCont->getNewScore(i).c_str());

	_ui->indivSSScoreTextBox->setText(resCont->getSsr(i).c_str());
	_ui->indivSCScoreTextBox->setText(resCont->getScr(i).c_str());
	_ui->indivCCScoreTextBox->setText(resCont->getCcr(i).c_str());

	_ui->indivElecScoreTextBox->setText(resCont->getElec(i).c_str());
	_ui->indivHydroScoreTextBox->setText(resCont->getHydro(i).c_str());
	_ui->indivSccScoreTextBox->setText(resCont->getScc(i).c_str());
	_ui->indivHbondScoreTextBox->setText(resCont->getHbond(i).c_str());

	_ui->indivVDWScoreTextBox->setText(resCont->getVdw(i).c_str());
	_ui->indivClashScoreTextBox->setText(resCont->getClashes(i).c_str());
	_ui->indivDispScoreTextBox->setText(resCont->getDeldispeScore(i).c_str());
	_ui->indivPgsolScoreTextBox->setText(resCont->getPgsolScore(i).c_str());
	_ui->indivPgsolHScoreTextBox->setText(resCont->getPgsolhScore(i).c_str());

	_ui->indivMat00->setText(resCont->getMat(i, 0).c_str());
	_ui->indivMat01->setText(resCont->getMat(i, 1).c_str());
	_ui->indivMat02->setText(resCont->getMat(i, 2).c_str());
	_ui->indivMat03->setText(resCont->getMat(i, 3).c_str());

	_ui->indivMat04->setText(resCont->getMat(i, 4).c_str());
	_ui->indivMat05->setText(resCont->getMat(i, 5).c_str());
	_ui->indivMat06->setText(resCont->getMat(i, 6).c_str());
	_ui->indivMat07->setText(resCont->getMat(i, 7).c_str());

	_ui->indivMat08->setText(resCont->getMat(i, 8).c_str());
	_ui->indivMat09->setText(resCont->getMat(i, 9).c_str());
	_ui->indivMat10->setText(resCont->getMat(i, 10).c_str());
	_ui->indivMat11->setText(resCont->getMat(i, 11).c_str());	

	_ui->indivDelGpolScoreTextBox->setText(resCont->getDelGpol(i).c_str());
	_ui->indivAreaPropScoreTextBox->setText(resCont->getAreaProp(i).c_str());

	if(i<resCont->getNumStats())
	{
		_ui->indivInterfaceAtomsTextBox->setText(resCont->getNumInterfaceAtoms(i).c_str());
		_ui->indivInterfaceResidueTextBox->setText(resCont->getNumInterfaceResidues(i).c_str());
		_ui->indivInterfaceNonPolarResTextBox->setText(resCont->getNumNonPolarResidue(i).c_str());
		_ui->indivInterfacePolarResTextBox->setText(resCont->getNumPolarResidue(i).c_str());

		_ui->indivInterfaceAreaTextBox->setText(resCont->getInterfaceArea(i).c_str());
		_ui->indivGapVolIndexTextBox->setText(resCont->getGapIndex(i).c_str());

		_ui->indivInterfaceWidthTextBox->setText(resCont->getInterfaceWidth(i).c_str());
		_ui->indivLocalDensityTextBox->setText(resCont->getLocalDensityIndex(i).c_str());

		_ui->indivPlanarityIndexTextBox->setText(resCont->getPlanarityIndex(i).c_str());
		_ui->indivCircularityIndexTextBox->setText(resCont->getCircularityIndex(i).c_str());

		for(int j=0; j<20; j++)
			for(int k=0; k<20; k++)
				_ui->resresContactTable->setItem(j, k, new QTableWidgetItem( resCont->getResresContact(i,j,k).c_str() ));
	}
	else
	{
		_ui->indivInterfaceAtomsTextBox->setText("0");
		_ui->indivInterfaceResidueTextBox->setText("0");
		_ui->indivInterfaceNonPolarResTextBox->setText("0");
		_ui->indivInterfacePolarResTextBox->setText("0");

		_ui->indivInterfaceAreaTextBox->setText("0");
		_ui->indivGapVolIndexTextBox->setText("0");

		_ui->indivInterfaceWidthTextBox->setText("0");
		_ui->indivLocalDensityTextBox->setText("0");

		_ui->indivPlanarityIndexTextBox->setText("0");
		_ui->indivCircularityIndexTextBox->setText("0");

		for(int j=0; j<20; j++)
			for(int k=0; k<20; k++)
				_ui->resresContactTable->setItem(j, k, new QTableWidgetItem( "0" ));
	}
}


