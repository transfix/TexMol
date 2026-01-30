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
#include <F2DockClient/f2dGenDialog.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include <iostream>

#include "ui_f2dGenUI.h"

using namespace std;

F2dGenDialog::F2dGenDialog(QWidget *parent,Qt::WFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::f2dGenUI;
  _ui->setupUi(this);

  init();
  makeConnections();
}


F2dGenDialog::~F2dGenDialog()
{ delete _ui;}



void F2dGenDialog::makeConnections()
{
	connect( _ui->receptorButton, SIGNAL( clicked() ), this, SLOT( receptorPDBFileNameSlot() ) );
	connect( _ui->ligandButton, SIGNAL( clicked() ), this, SLOT( ligandPDBFileNameSlot() ) );
	connect( _ui->submitButton, SIGNAL( clicked() ), this, SLOT( submitSlot() ) );
	connect( _ui->saveF2dButton, SIGNAL( toggled(bool) ), this, SLOT( saveF2dSlot(bool) ) );
	connect( _ui->savePQRButton, SIGNAL( toggled(bool) ), this, SLOT( savePQRSlot(bool) ) );
}


void F2dGenDialog::init()
{
	pqr = false;	
	QStringList labels;
	labels.append("Fine");
	labels.append("Normal");
	labels.append("Coarse");
	_ui->resolutionCombo->addItems(labels);
}


void F2dGenDialog::receptorPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load receptor PDB File", QString::null, "PDB (*.pdb)");

	if(!filename.isNull())
	{
		_ui->receptorTextBox->setText(filename);

		if(!f2dCont->setReceptorPDBName(_ui->receptorTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid receptor PDB", "Please select a valid receptor PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");
}

void F2dGenDialog::ligandPDBFileNameSlot()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load ligand PDB File", QString::null, "PDB (*.pdb)");

	if(!filename.isNull()) 
	{
		_ui->ligandTextBox->setText(filename);

		if(!f2dCont->setLigandPDBName(_ui->ligandTextBox->text().toStdString()))
			QMessageBox::information(this, "Please select a valid ligand PDB", "Please select a valid ligand PDB");
	}
	else 
		QMessageBox::information(this, "No file selected", "No file selected");	
}


void F2dGenDialog::saveF2dSlot(bool b)
{
	if(b)
	{
		_ui->savePQRButton->setDown(false);
		pqr = false;
	}
}


void F2dGenDialog::savePQRSlot(bool b)
{
	if(b)
	{
		_ui->saveF2dButton->setDown(false);
		pqr = true;
	}
}



void F2dGenDialog::submitSlot()
{
	QString title = _ui->titleTextBox->text();

	if(title.length()>0)
		f2dCont->setJobTitle(title.toStdString());

	f2dCont->setReceptorPDBName(_ui->receptorTextBox->text().toStdString());
	f2dCont->setLigandPDBName(_ui->ligandTextBox->text().toStdString());

	if(!f2dCont->setResolution(_ui->resolutionCombo->currentIndex())) return;

	if(f2dCont->handleJob(pqr))
	{
//		cout<<"done"<<endl;
		this->done(QDialog::Accepted);
	}
	else
	{
		QMessageBox::information(this, "Please specify all inputs", "Please specify all inputs");
	}
}
