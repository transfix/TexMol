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
#include <TexMol/Dialogs/GeneralizedBornDialog.h>

#include <QFileDialog>

#include "ui_GeneralizedBornDialogBase.h"

GeneralizedBornDialog::GeneralizedBornDialog(QWidget *parent,Qt::WindowFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::GeneralizedBornDialogBase;
  _ui->setupUi(this);
  

  _ui->gbEpsilonLineEdit->setText("0.1");

  _ui->gbThresholdLineEdit->setText("100");

}


GeneralizedBornDialog::~GeneralizedBornDialog()
{ delete _ui;}


void GeneralizedBornDialog::gbPqrFileNameSlot()
{
  _ui->gbPqrLineEdit->setText(QFileDialog::getOpenFileName(this,
                                                         "Load PQR File",
                                                         QString(),
                                                         "PQR (*.pqr)"));
}

void GeneralizedBornDialog::gbRawnFileNameSlot()
{
  _ui->gbRawnLineEdit->setText(QFileDialog::getOpenFileName(this,
                                                         "Load RAWN File",
                                                         QString(),
                                                         "RAWN (*.rawn)"));
}


void GeneralizedBornDialog::gbOutputPrefixSlot()
{
  _ui->gbOutputLineEdit->setText(QFileDialog::getSaveFileName(this,
                                                         "Output Filename Prefix",
                                                         QString(),
							      QString()));
}



QString GeneralizedBornDialog::getPqrFileName(){
  return _ui->gbPqrLineEdit->text();
}

QString GeneralizedBornDialog::getRawnFileName(){
  return _ui->gbRawnLineEdit->text();
}

QString GeneralizedBornDialog::getOutputPrefix(){
  return _ui->gbOutputLineEdit->text();
}

double GeneralizedBornDialog::getEpsilon(){
  return _ui->gbEpsilonLineEdit->text().toDouble();
}

double GeneralizedBornDialog::getThreshold(){
  return _ui->gbThresholdLineEdit->text().toDouble();
}

int GeneralizedBornDialog::getSurfaceType(){
  return _ui->gbSurfaceTypeSelector->currentIndex();
}

