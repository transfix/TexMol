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
#include <F2DockClient/quadGenDialog.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <iostream>

#include "ui_quadGenUI.h"

using namespace std;

QuadGenDialog::QuadGenDialog(QWidget *parent,Qt::WFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::quadGenUI;
  _ui->setupUi(this);

  init();
  makeConnections();
}


QuadGenDialog::~QuadGenDialog()
{ delete _ui;}



void QuadGenDialog::makeConnections()
{
	connect( _ui->receptorButton, SIGNAL( clicked() ), this, SLOT( receptorPDBFileNameSlot() ) );
	connect( _ui->ligandButton, SIGNAL( clicked() ), this, SLOT( ligandPDBFileNameSlot() ) );
	connect( _ui->submitButton, SIGNAL( clicked() ), this, SLOT( submitSlot() ) );
	connect( _ui->saveQuadButton, SIGNAL( toggled(bool) ), this, SLOT( saveQuadSlot(bool) ) );
	connect( _ui->saveRAWNButton, SIGNAL( toggled(bool) ), this, SLOT( saveRAWNSlot(bool) ) );

}


void QuadGenDialog::init()
{
	rawn = false;

	QStringList labels;
	labels.append("Fine");
	labels.append("Normal");
	labels.append("Coarse");
	_ui->resolutionCombo->addItems(labels);
}


void QuadGenDialog::receptorPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PDB File", QString::null, "PDB (*.pdb)");

	if(!filename.isNull())
	{
		_ui->receptorTextBox->setText(filename);

		if(!quadCont->setReceptorPDBName(_ui->receptorTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void QuadGenDialog::ligandPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PDB File", QString::null, "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->ligandTextBox->setText(filename);

		if(!quadCont->setLigandPDBName(_ui->ligandTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");	
}


void QuadGenDialog::saveQuadSlot(bool b)
{
	if(b)
	{
		_ui->saveRAWNButton->setDown(false);
		rawn = false;
	}
}


void QuadGenDialog::saveRAWNSlot(bool b)
{
	if(b)
	{
		_ui->saveQuadButton->setDown(false);
		rawn = true;
	}
}




void QuadGenDialog::submitSlot()
{
	QString title = _ui->titleTextBox->text();
	if(title.length()>0)
		quadCont->setJobTitle(title.toStdString());

	quadCont->setReceptorPDBName(_ui->receptorTextBox->text().toStdString());
	quadCont->setLigandPDBName(_ui->ligandTextBox->text().toStdString());


	if(!quadCont->setResolution(_ui->resolutionCombo->currentIndex())) return;

	if(quadCont->handleJob(rawn))
	{
//		cout<<"done"<<endl;
		this->done(QDialog::Accepted);
	}
	else
	{
		QMessageBox::information(this, "Please specify all inputs", "Please specify all inputs");
	}	
}
