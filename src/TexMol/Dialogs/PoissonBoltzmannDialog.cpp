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
#include <TexMol/Dialogs/PoissonBoltzmannDialog.h>

#include <QFileDialog>

#include "ui_PoissonBoltzmannDialogBase.h"

PoissonBoltzmannDialog::PoissonBoltzmannDialog(QWidget *parent,Qt::WindowFlags flags) 
  : QDialog(parent, flags) {

  _ui = new Ui::PoissonBoltzmannDialogBase;
  _ui->setupUi(this);
  

  _ui->pbTempLineEdit->setText("300");

  _ui->pbIonConcLineEdit->setText("0.0");

}


PoissonBoltzmannDialog::~PoissonBoltzmannDialog()
{ delete _ui;}


void PoissonBoltzmannDialog::pbPqrFileNameSlot()
{
  _ui->pbPqrLineEdit->setText(QFileDialog::getOpenFileName(this,
                                                         "Load PQR File",
                                                         QString(),
                                                         "PQR (*.pqr)"));
}

void PoissonBoltzmannDialog::pbRawnFileNameSlot()
{
  _ui->pbRawnLineEdit->setText(QFileDialog::getOpenFileName(this,
                                                         "Load RAWN File",
                                                         QString(),
                                                         "RAWN (*.rawn)"));
}


void PoissonBoltzmannDialog::pbOutputPrefixSlot()
{
  _ui->pbOutputLineEdit->setText(QFileDialog::getSaveFileName(this,
                                                         "Output Filename Prefix",
                                                         QString(),
							      QString()));
}



QString PoissonBoltzmannDialog::getPqrFileName(){
  return _ui->pbPqrLineEdit->text();
}

QString PoissonBoltzmannDialog::getRawnFileName(){
  return _ui->pbRawnLineEdit->text();
}

QString PoissonBoltzmannDialog::getOutputPrefix(){
  return _ui->pbOutputLineEdit->text();
}

double PoissonBoltzmannDialog::getTemp(){
  return _ui->pbTempLineEdit->text().toDouble();
}

double PoissonBoltzmannDialog::getIonConc(){
  return _ui->pbIonConcLineEdit->text().toDouble();
}
