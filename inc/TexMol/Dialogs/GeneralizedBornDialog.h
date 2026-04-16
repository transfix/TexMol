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
#ifndef GENERALIZED_BORN_DIALOG_H
#define GENERALIZED_BORN_DIALOG_H


#include <QDialog>
#include <QString>


namespace Ui
{
  class GeneralizedBornDialogBase;
}

#include "ui_GeneralizedBornDialogBase.h"


class GeneralizedBornDialog : public QDialog
{
  Q_OBJECT
    public:
  GeneralizedBornDialog(QWidget *parent=nullptr,Qt::WindowFlags flags={});
  
  virtual ~GeneralizedBornDialog();
  
  public slots:
  
  virtual void gbPqrFileNameSlot();
  virtual void gbRawnFileNameSlot();
  virtual void gbOutputPrefixSlot();

  QString getPqrFileName();
  QString getRawnFileName();
  QString getOutputPrefix();
  double getEpsilon();
  double getThreshold();
  int getSurfaceType();


 protected:
  Ui::GeneralizedBornDialogBase *_ui;

};

#endif
